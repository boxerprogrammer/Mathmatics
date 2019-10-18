#include<DxLib.h>
#include<vector>
#include"Geometry.h"

using namespace std;
float Clampf(float value, float minV = 0.0f, float maxV = 1.0f) {
	return max(min(value, maxV), minV);
}

Position2f Clamp(Position2f value, Position2f minV = Position2f(0.0f, 0.0f), Position2f maxV = Position2f(1.0f, 1.0f)) {
	return Position2f(Clampf(value.x, minV.x, maxV.x), Clampf(value.y, minV.y, maxV.y));
}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetWindowText("余弦定理IK");
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	int screen_width, screen_height;
	GetWindowSize(&screen_width, &screen_height);
	SetDrawScreen(DX_SCREEN_BACK);
	auto rt = MakeScreen(screen_width, screen_height);



	int currentX = 512;
	int capturedNo = -1;
	int lastMouseInput = 0;
	bool captured = false;
	char keystate[256];
	char lastKeyState[256];
	bool overlapped[3] = {};
	vector<Position2f> positions(3);
	for (int i = 0; i < positions.size(); ++i) {
		positions[i] = Position2f(50 + i * 250, 240);
	}
	std::vector<float> edgeLens(positions.size() - 1);
	for (int i = 0; i < edgeLens.size(); ++i) {
		edgeLens[i] = (positions[i + 1] - positions[i]).Length();
	}

	float angle = 0.0f;
	while (ProcessMessage() == 0) {
		SetDrawScreen(rt);
		DrawBox(0, 0, screen_width, screen_height, 0xffffff, true);

		positions[2] = Position2f(425 + cos(angle) * 120, 240 - sin(angle) * 120);
		angle += 0.05f;

		int mx, my;
		GetMousePoint(&mx, &my);
		Position2f mp(mx, my);

		for (int i = 0; i < positions.size(); ++i) {
			auto diff = mp - positions[i];
			auto r = diff.Length();
			if (r <= 20.0f&&i != 1) {//中間地点は選択しない
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
					if (r <= 20.0f&&i != 1) {//中間地点は選択しない
						captured = true;
						capturedNo = i;
					}
				}
			}
			else {
				if (captured&&capturedNo >= 0) {
					positions[capturedNo] = Clamp(mp, Position2f(0, 0), Position2f(640, 480));
					//もちろん、骨の長さ以上に離れようと、してはいけない(戒め)
					auto linearVec = positions[2] - positions[0];
					auto limitLen = edgeLens[0] + edgeLens[1];
					if (linearVec.Length() >= limitLen) {
						linearVec = linearVec.Normalized()*limitLen;
						if (capturedNo == 0) {
							positions[0] = positions[2] - linearVec;
						}
						else {
							positions[2] = positions[0] + linearVec;
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

		//position[0]とposition[2]の位置関係からposition[1]を求める
		Vector2f linearVec = positions[2] - positions[0];
		float A = linearVec.Length();
		float B = edgeLens[0];
		float C = edgeLens[1];
		linearVec = linearVec.Normalized();
		float angleFromX = atan2(linearVec.y, linearVec.x);
		float theta = acosf((A*A + B * B - C * C) / (2 * A*B));
		angleFromX += theta;

		if (A < B + C) {
			positions[1] = positions[0] + Vector2f(cosf(angleFromX), sinf(angleFromX))*edgeLens[0];
		}
		else {
			positions[1] = positions[0] + linearVec * edgeLens[0];
		}

		//影表示
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

		DrawCircleAA(425, 240, 120, 32, 0x00aa00, false, 1);

		for (int i = 0; i < positions.size(); ++i) {
			uint32_t col = 0x000000;
			if (overlapped[i]) {
				col = 0x0000aa;
			}
			if (i == capturedNo) {
				col = 0xaa0000;
			}
			DrawCircleAA(positions[i].x,
				positions[i].y,
				25, 32,
				col,
				false,
				2);
			if (i < positions.size() - 1) {
				DrawLineAA(positions[i].x,
					positions[i].y,
					positions[i + 1].x,
					positions[i + 1].y,
					0x000000, 5);
			}
		}

		ScreenFlip();
	}
	DxLib_End();
}
