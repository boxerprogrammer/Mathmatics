#include<DxLib.h>
#include<vector>
#include"Geometry.h"

using namespace std;
float Clampf(float value, float minV = 0.0f, float maxV = 1.0f) {
	return max(min(value, maxV), minV);
}

Position3f Clamp(Position3f value, Position3f minV = Position3f(0.0f, 0.0f,0.0f), Position3f maxV = Position3f(1.0f, 1.0f,1.0f)) {
	return Position3f(Clampf(value.x, minV.x, maxV.x), Clampf(value.y, minV.y, maxV.y), Clampf(value.z, minV.z, maxV.z));
}

VECTOR ToVECTOR(Vector3f v) {
	return VGet(v.x, v.y, v.z);
}

Vector3f ToVector3f(VECTOR v) {
	return Vector3f(v.x,v.y,v.z);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetWindowText("余弦定理IK3D");
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
	vector<Position3f> positions(3);
	for (int i = 0; i < positions.size(); ++i) {
		positions[i] = Position3f(50 + i * 250, 240,0.0f);
	}
	std::vector<float> edgeLens(positions.size() - 1);
	for (int i = 0; i < edgeLens.size(); ++i) {
		edgeLens[i] = (positions[i + 1] - positions[i]).Length();
	}

	DxLib::SetUseZBuffer3D(true);
	DxLib::SetWriteZBuffer3D(true);

	float angle = 0.0f;

	VECTOR campos = GetCameraPosition();
	float angleH = GetCameraAngleHRotate();
	float angleAxis = 0.0f;
	Vector3f rotV(0, 0, 1);//IK中心軸
	//VECTOR rotV = VGet(0, 0, 1);///IK中心軸
	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		SetCameraPositionAndAngle(campos, 0, angleH, 0);
		

		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_LEFT]) {
			angleH -= 0.01f;
		}
		if (keystate[KEY_INPUT_RIGHT]) {
			angleH += 0.01f;
		}
		if (keystate[KEY_INPUT_A]) {
			campos.x -= 1.f;
		}
		if (keystate[KEY_INPUT_D]) {
			campos.x += 1.f;
		}

		if (keystate[KEY_INPUT_W]) {
			campos.z += 1.f;
		}
		if (keystate[KEY_INPUT_S]) {
			campos.z -= 1.f;
		}
		if (keystate[KEY_INPUT_R]) {
			angleAxis += 0.01f;
		}


		auto tmpRotV = ToVector3f(VTransformSR(ToVECTOR(rotV), MGetRotX(angleAxis)));

		DrawBox(0, 0, screen_width, screen_height, 0xffffff, true);


		positions[2] = Position3f(425 + cos(angle) * 120, 240 - sin(angle) * 120,0);
		angle += 0.05f;

		int mx, my;
		GetMousePoint(&mx, &my);
		Position3f mp(mx, my,0);

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
					positions[capturedNo] = Clamp(mp, Position3f(0, 0,0), Position3f(640, 480,0));
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
		auto linearVec = positions[2] - positions[0];
		float A = linearVec.Length();
		float B = edgeLens[0];
		float C = edgeLens[1];
		linearVec = linearVec.Normalized();
		
		float theta = acosf((A*A + B * B - C * C) / (2 * A*B));

		auto m = MGetRotAxis(ToVECTOR(Cross(tmpRotV,linearVec)), theta);
		auto v = VTransformSR(ToVECTOR(linearVec), m);

		if (A < B + C) {
			positions[1] = positions[0] + ToVector3f(v).Normalized() *edgeLens[0];
		}
		else {
			positions[1] = positions[0] + linearVec * edgeLens[0];
		}


		for (int i = 0; i < positions.size(); ++i) {
			DrawSphere3D(ToVECTOR(positions[i]), 20, 32, 0xeeeeee, 0xffffff, true);
			if (i < positions.size() - 1) {
				DrawCapsule3D(ToVECTOR(positions[i]),
					ToVECTOR(positions[i+1]),5,32,
					0xeeffee,0xffffff,true);
			}
		}
		DrawCone3D(ToVECTOR(positions[0] + tmpRotV * 50),
			ToVECTOR(positions[0]), 20, 32, 0xff0000, 0xffaaaa, true);
		ScreenFlip();
	}
	DxLib_End();
}
