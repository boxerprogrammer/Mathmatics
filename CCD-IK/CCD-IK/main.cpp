#include<DxLib.h>
#include<vector>
#include<array>
#include<algorithm>
#include<numeric>
#include"Geometry.h"

using namespace std;
float Clampf(float value, float minV = 0.0f, float maxV = 1.0f) {
	return max(min(value, maxV), minV);
}

Position2f Clamp(Position2f value, Position2f minV = Position2f(0.0f, 0.0f), Position2f maxV = Position2f(1.0f, 1.0f)) {
	return Position2f(Clampf(value.x, minV.x, maxV.x), Clampf(value.y, minV.y, maxV.y));
}

constexpr uint32_t points_num = 4;

void Draw(int rt, int screen_width, int screen_height, 
	vector<Position2f> &positions, Position2f& target) {
	SetDrawScreen(rt);
	DrawBox(0, 0, screen_width, screen_height, 0xffffff, true);

	//影表示
	DrawCircleAA(target.x + 2,
		target.y + 2,
		25, 32,
		0x000000,
		false,
		2);
	for (int i = 0; i < positions.size(); ++i) {

		DrawCircleAA(positions[i].x + 2,
			positions[i].y + 2,
			25, 32,
			0x000000,
			false,
			2);
		if (i < positions.size() - 1) {
			DrawLineAA(positions[i].x + 2,
				positions[i].y + 2,
				positions[i + 1].x + 2,
				positions[i + 1].y + 2,
				0x000000, 5);
		}
	}
	GraphFilter(rt, DX_GRAPH_FILTER_GAUSS, 4, 800);
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, rt, true);

	//通常表示
	for (int i = 0; i < positions.size(); ++i) {

		DrawCircleAA(positions[i].x,
			positions[i].y,
			25, 32,
			0x000000,
			false,
			2);
		if (i < positions.size() - 1) {
			DrawLineAA(positions[i].x,
				positions[i].y,
				positions[i + 1].x,
				positions[i + 1].y,
				0x000000, 5);
		}
		DrawCircleAA(target.x,
			target.y,
			25, 32,
			0xaa0000,
			false,
			2);
	}

}

void MoveControlPoint(vector<Position2f> &positions, 
	array<bool, points_num> &overlapped, 
	int &lastMouseInput, 
	bool &captured, 
	int &capturedNo, 
	std::vector<float> &edgeLens) {
	//端点ドラッグアンドドロップコントロール
	int mx, my;
	GetMousePoint(&mx, &my);
	Position2f mp(mx, my);
	for (int i = 0; i < positions.size(); ++i) {
		auto diff = mp - positions[i];
		auto r = diff.Length();
		if (r <= 20.0f&&i == points_num - 1) {//端点以外は選択しない
			overlapped[i] = true;
		}
		else {
			overlapped[i] = false;
		}
	}
	auto currentMouseInput = GetMouseInput();
	if (currentMouseInput) {
		if (lastMouseInput == 0) {
			for (int i = 0; i < positions.size(); ++i) {
				auto diff = mp - positions[i];
				auto r = diff.Length();
				if (r <= 20.0f&&i == points_num - 1) {//端点のみ選択可能
					captured = true;
					capturedNo = i;
				}
			}
		}
		else {
			if (captured&&capturedNo >= 0) {
				positions[capturedNo] = Clamp(mp, Position2f(0, 0), Position2f(640, 480));
				//もちろん、骨の長さ以上に離れようと、してはいけない(戒め)
				auto linearVec = positions.back() - positions.front();
				auto limitLen = std::accumulate(edgeLens.begin(), edgeLens.end(), 0);

				if (linearVec.Length() >= limitLen) {
					linearVec = linearVec.Normalized()*limitLen;
					if (capturedNo = points_num - 1) {
						positions[capturedNo] = positions[0] + linearVec;
					}
				}

			}
		}
		lastMouseInput = currentMouseInput;
	}
	else {
		lastMouseInput = 0;
		capturedNo = -1;
		captured = false;
	}
}

void MoveTarget(Position2f& target, int &lastMouseInput, bool &captured, bool& targetOverlapped){
	//端点ドラッグアンドドロップコントロール
	int mx, my;
	GetMousePoint(&mx, &my);
	Position2f mp(mx, my);
	auto diff = mp - target;
	auto r = diff.Length();
	if (r <= 20.0f) {//端点以外は選択しない
		targetOverlapped = true;
	}
	else {
		targetOverlapped = false;
	}
	
	auto currentMouseInput = GetMouseInput();
	if (currentMouseInput) {
		if (lastMouseInput == 0) {
			auto diff = mp - target;
			auto r = diff.Length();
			if (r <= 20.0f) {//端点のみ選択可能
				captured = true;
			}
		}
		else {
			if (captured) {
				target = Clamp(mp, Position2f(0, 0), Position2f(640, 480));
				//もちろん、骨の長さ以上に離れようと、してはいけない(戒め)
				

			}
		}
		lastMouseInput = currentMouseInput;
	}
	else {
		lastMouseInput = 0;
		captured = false;
	}
}

///positionsの末端がtargetに一致し、かつ先頭を動かさないようにpositionsを書き換える
void CCD_IK(Position2f target, vector<Position2f>& positions, std::vector<float>& edgeLens,unsigned int cyclic = 4) {
	//もしtargetとpositionsの先頭が全骨の長さより長いなら角度は180確定なのでCCD-IKを行わず末端から回転させる
	if (accumulate(edgeLens.begin(), edgeLens.end(), 0) < (target - positions.front()).Length()) {
		auto vec = target - positions.front();
		vec.Normalize();
		for (int i = 0; i < edgeLens.size(); ++i) {
			positions[i + 1] = positions[i] + vec*edgeLens[i];
		}
		return;
	}

	//繰り返し回数だけ繰り返す
	for (int c = 0; c < cyclic; ++c) {
		//末端から攻めていく
		auto rit = positions.rbegin();
		++rit;//末端との角度で計算するため、末端は省く
		for (;rit!= positions.rend(); ++rit) {
			//回転角度計算
			Vector2f vecToEnd = positions.back() - *rit;//末端と現在のノードのベクトル
			Vector2f vecToTarget = target - *rit;//現在のノードからターゲット(エフェクタ)へのベクトル
			vecToEnd.Normalize();
			vecToTarget.Normalize();
			//2Dだから角度計算は単純(3Dの場合こうはいかない)
			auto cross = Cross(vecToEnd, vecToTarget);
			auto dot = Dot(vecToEnd, vecToTarget);
			auto angle = atan2f(Cross(vecToEnd, vecToTarget), Dot(vecToEnd, vecToTarget));
			//回転行列を得る
			Matrix mat = MultipleMat(TranslateMat(rit->x,rit->y),MultipleMat(RotateMat(angle),TranslateMat(-rit->x,-rit->y)));
			//現在のノードから末端まで回転する
			auto it = rit.base();
			for (; it != positions.end(); ++it) {
				*it = MultipleVec(mat, *it);
			}


		}
	}
}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetWindowText("CCD-IK 2D");
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	int screen_width, screen_height;
	GetWindowSize(&screen_width, &screen_height);
	SetDrawScreen(DX_SCREEN_BACK);
	auto rt = MakeScreen(screen_width, screen_height);


	bool targetOverlapped = false;
	int currentX = 512;
	int capturedNo = -1;
	int lastMouseInput = 0;
	bool captured = false;
	char keystate[256];
	char lastKeyState[256];
	
	array<bool, points_num> overlapped = {};
	vector<Position2f> positions(points_num);
	for (int i = 0; i < positions.size(); ++i) {
		positions[i] = Position2f(50 + i * (500/(points_num-1)), 240);
	}
	std::vector<float> edgeLens(positions.size() - 1);
	for (int i = 0; i < edgeLens.size(); ++i) {
		edgeLens[i] = (positions[i + 1] - positions[i]).Length();
	}

	float angle = 0.0f;
	Position2f target = positions.back();
	while (ProcessMessage() == 0) {

		//コントロールポイントの移動
		//MoveControlPoint(positions, overlapped, lastMouseInput, captured, capturedNo, edgeLens);
		MoveTarget(target, lastMouseInput, captured, targetOverlapped);

		CCD_IK(target, positions,edgeLens);


		//描画
		Draw(rt, screen_width, screen_height, positions, target);

		ScreenFlip();

	}
	DxLib_End();
}
