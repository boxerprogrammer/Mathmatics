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
struct Limit {
	float minimum=0.0f;//最小値
	float maximum=0.0f;//最大値
	Limit(float mini=0.0f, float maxi=0.0f) :minimum(mini), maximum(maxi) {}
};
struct IKPoint {
	Position2f pos;
	Limit limit;
	IKPoint(const Position2f& inp, Limit& inlim) :pos(inp), limit(inlim) {}
	IKPoint() :pos(0.0f,0.0f), limit(Limit()) {}

};

void Draw(int rt, int screen_width, int screen_height, 
	vector<IKPoint> &positions, Position2f& target) {
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

		DrawCircleAA(positions[i].pos.x + 2,
			positions[i].pos.y + 2,
			25, 32,
			0x000000,
			false,
			2);
		if (i < positions.size() - 1) {
			DrawLineAA(positions[i].pos.x + 2,
				positions[i].pos.y + 2,
				positions[i + 1].pos.x + 2,
				positions[i + 1].pos.y + 2,
				0x000000, 5);
		}
	}
	GraphFilter(rt, DX_GRAPH_FILTER_GAUSS, 4, 800);
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, rt, true);

	//通常表示
	for (int i = 0; i < positions.size(); ++i) {

		DrawCircleAA(positions[i].pos.x,
			positions[i].pos.y,
			25, 32,
			0x000000,
			false,
			2);
		if (i < positions.size() - 1) {
			DrawLineAA(positions[i].pos.x,
				positions[i].pos.y,
				positions[i + 1].pos.x,
				positions[i + 1].pos.y,
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

void MoveControlPoint(vector<IKPoint> &positions,
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
		auto diff = mp - positions[i].pos;
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
				auto diff = mp - positions[i].pos;
				auto r = diff.Length();
				if (r <= 20.0f&&i == points_num - 1) {//端点のみ選択可能
					captured = true;
					capturedNo = i;
				}
			}
		}
		else {
			if (captured&&capturedNo >= 0) {
				positions[capturedNo].pos = Clamp(mp, Position2f(0, 0), Position2f(640, 480));
				//もちろん、骨の長さ以上に離れようと、してはいけない(戒め)
				auto linearVec = positions.back().pos - positions.front().pos;
				auto limitLen = std::accumulate(edgeLens.begin(), edgeLens.end(), 0);

				if (linearVec.Length() >= limitLen) {
					linearVec = linearVec.Normalized()*limitLen;
					if (capturedNo = points_num - 1) {
						positions[capturedNo].pos = positions[0].pos + linearVec;
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
void CCD_IK(Position2f target, vector<IKPoint>& initPositions,vector<IKPoint>& positions, std::vector<float>& edgeLens, unsigned int cyclic = 4) {
	//もしtargetとpositionsの先頭が全骨の長さより長いなら角度は180確定なのでCCD-IKを行わず末端から回転させる
	if (accumulate(edgeLens.begin(), edgeLens.end(), 0) < (target - positions.front().pos).Length()) {
		auto vec = target - positions.front().pos;
		vec.Normalize();
		for (int i = 0; i < edgeLens.size(); ++i) {
			positions[i + 1].pos = positions[i].pos + vec*edgeLens[i];
		}
		return;
	}

	copy(begin(initPositions), end(initPositions), begin(positions));

	//繰り返し回数だけ繰り返す
	for (int c = 0; c < cyclic; ++c) {
		//末端から攻めていく
		auto rit = positions.rbegin();
		++rit;//末端との角度で計算するため、末端は省く
		float accumulateAngle = 0.0f;
		unsigned int i = 0;
		for (;rit!= positions.rend(); ++rit) {
			//回転角度計算
			Vector2f vecToEnd = positions.back().pos - rit->pos;//末端と現在のノードのベクトル
			Vector2f vecToTarget = target - rit->pos;//現在のノードからターゲット(エフェクタ)へのベクトル
			vecToEnd.Normalize();
			vecToTarget.Normalize();
			//2Dだから角度計算は単純(3Dの場合こうはいかないです)
			auto angle = atan2f(Cross(vecToEnd, vecToTarget), Dot(vecToEnd, vecToTarget));
			angle = Clampf(angle, rit->limit.minimum, rit->limit.maximum);

			if (c == 0 && i==0 && angle==0.0f) {
				auto len=(target - rit->pos).Length();
				len = min(edgeLens.back(), len);
				angle= acosf(len / edgeLens.back());
				if (Clampf(angle, rit->limit.minimum, rit->limit.maximum) == 0.0f) {
					angle = -angle;
					angle = Clampf(angle, rit->limit.minimum, rit->limit.maximum);
				}
			}
			++i;
			if (angle == 0.0f)continue;
			//回転行列を得る
			Matrix mat = MultipleMat(TranslateMat(rit->pos.x,rit->pos.y),MultipleMat(RotateMat(angle),TranslateMat(-rit->pos.x,-rit->pos.y)));
			//現在のノードから末端まで回転する
			auto it = rit.base();
			for (; it != positions.end(); ++it) {
				it->pos = MultipleVec(mat, it->pos);
			}

			
		}
	}
}

void InitPositions(std::vector<IKPoint> &positions, std::vector<float> &edgeLens, Position2f &target) {
	for (int i = 0; i < positions.size(); ++i) {
		positions[i].pos = Position2f(50 + i * (500 / (points_num - 1)), 240);
	}
	for (int i = 0; i < edgeLens.size(); ++i) {
		edgeLens[i] = (positions[i + 1].pos - positions[i].pos).Length();
	}
	target = positions.back().pos;
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
	
	vector<IKPoint> positions(points_num);
	vector<IKPoint> initialPositions(points_num);

	std::vector<float> edgeLens(positions.size() - 1);
	Position2f target=Position2f();
	InitPositions(positions, edgeLens, target);
	for (auto& point : positions) {
		point.limit = Limit(-0 / 12.0f, DX_PI_F / 2.0f);
	}
	positions[0].limit = Limit(-DX_PI_F, DX_PI_F);//実質制限なし

	copy(begin(positions), end(positions), begin(initialPositions));

	float angle = 0.0f;
	
	while (ProcessMessage() == 0) {
		GetHitKeyStateAll(keystate);
		//Rを押したらリスタート
		if (keystate[KEY_INPUT_R] && lastKeyState[KEY_INPUT_R]) {
			InitPositions(positions, edgeLens, target);
		}

		//コントロールポイントの移動
		//MoveControlPoint(positions, overlapped, lastMouseInput, captured, capturedNo, edgeLens);
		MoveTarget(target, lastMouseInput, captured, targetOverlapped);

		CCD_IK(target, initialPositions, positions,edgeLens,8);


		//描画
		Draw(rt, screen_width, screen_height, positions, target);

		ScreenFlip();
		copy(begin(keystate), end(keystate), begin(lastKeyState));
	}
	DxLib_End();
}
