#include<DxLib.h>
#include<cmath>
#include<vector>
#include<array>

using namespace std;

struct Position2 {
	float x, y;
	Position2() :x(0), y(0) {}
	Position2(float inx, float iny) :x(inx), y(iny) {}
};

using Vector2=Position2;

Position2 operator+(const Position2& lval, const Position2& rval) {
	return Position2(lval.x + rval.x, lval.y + rval.y);
}

Position2 operator-(const Position2& lval, const Position2& rval) {
	return Position2(lval.x - rval.x, lval.y - rval.y);
}

Position2 operator*(const Position2& lval, const Vector2& rval) {
	return Position2(lval.x *rval.x, lval.y*rval.y);
}

Position2 operator*(const Position2& lval, float scale) {
	return Position2(lval.x *scale, lval.y*scale);
}

Position2 operator/(const Position2& lval, float div) {
	return Position2(lval.x /div, lval.y/div);
}

vector<Position2> CalculateInterporatedPoints(const array<Position2, 4>& cpnt, int divNum = 10) {
	vector<Position2> points(divNum);
	for (int i = 0; i < divNum; ++i) {
		float t = static_cast<float>(i) / static_cast<float>(divNum);
		float invt = 1.0 - t;
		points[i] = cpnt[0] * invt*invt*invt +
			cpnt[1] * 3 * invt*invt*t +
			cpnt[2] * 3 * invt*t*t +
			cpnt[3] * t*t*t;
	}
	return points;
}

void DrawBezier(const array<Position2, 4>& cpnt, int divNum = 10) {
	auto points = CalculateInterporatedPoints(cpnt, divNum);
	for (int i = 0; i < points.size() - 1; ++i) {
		DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, 0xaaaaff, 3);
	}
	auto& p = points[points.size() - 1];
	DrawLine(p.x, p.y, cpnt[3].x, cpnt[3].y, 0xaaaaff, 3);
}

float Clamp(float value, float minV = 0.0f, float maxV = 1.0f) {
	return max(min(value, maxV), minV);
}

Position2 Clamp(const Position2& value, Position2 minV=Position2(0.0f,0.0f), Position2 maxV = Position2(1.0f, 1.0f)) {
	return Position2(Clamp(value.x, minV.x, maxV.x), Clamp(value.y, minV.y, maxV.y));
}

///��ԃx�W�F��X���猋�ʂ�Y��Ԃ�
///�j���[�g���@
float GetYFromXOnBezierNT(float x,  Position2 a, Position2 b, unsigned int n = 8) {
	if (a.x == a.y&&b.x == b.y)return x;//�v�Z�s�v
	float t = x;
	float k0 = 1 + 3 * a.x - 3 * b.x;//t^3�̌W��
	float k1 = 3 * b.x - 6 * a.x;//t^2�̌W��
	float k2 = 3 * a.x;//t�̌W��
	//�덷�͈͓̔����ǂ����Ɏg�p����萔
	constexpr float epsilon = 0.0005f;
	//�j���[�g���@
	for (int i = 0; i < n; ++i) {
		//f(t)���߂܁[��
		auto ft = k0 * t*t*t + k1 * t*t + k2 * t - x;
		//�������ʂ�0�ɋ߂�(�덷�͈͓̔�)�Ȃ�ł��؂�
		if (ft <= epsilon && ft >= -epsilon)break;
		//f'(t)���߂܁[��
		auto fdt = 3 * k0*t*t + 2 * k1*t + k2;//��������
		if (fdt == 0)break;
		t -= ft / fdt;
	}
	//���ɋ��߂���t�͋��߂Ă���̂�y���v�Z����
	auto r = 1 - t;
	return t * t*t + 3 * t*t*r*b.y + 3 * t*r*r*a.y;
}

///�񕪖@
float GetYFromXOnBezierBin(float x, Position2 a, Position2 b, unsigned int n = 8) {
	if (a.x == a.y&&b.x == b.y)return x;//�v�Z�s�v
	float t = x;
	float k0 = 1 + 3 * a.x - 3 * b.x;//t^3�̌W��
	float k1 = 3 * b.x - 6 * a.x;//t^2�̌W��
	float k2 = 3 * a.x;//t�̌W��

	//�덷�͈͓̔����ǂ����Ɏg�p����萔
	constexpr float epsilon = 0.0005f;
	float at = 0.0f;
	float bt = 1.0f;
	//�񕪖@
	for (int i = 0; i < n; ++i) {
		//f(t)���߂܁[��
		auto fat = k0 * at*at*at + k1 * at*at + k2 * at - x;
		auto fbt = k0 * bt*bt*bt + k1 * bt*bt + k2 * bt - x;
		//�������ʂ�0�ɋ߂�(�덷�͈͓̔�)�Ȃ�ł��؂�
		if (fat <= epsilon && fat >= -epsilon) {
			t = at;
			break;
		}
		if (fbt <= epsilon && fbt >= -epsilon) {
			t = bt;
			break;
		}
		//f((a+b)/2)���߂܁[��
		auto abt = (at + bt) / 2.0f;
		auto fabt = k0 * abt*abt*abt + k1 * abt*abt + k2 * abt - x;
		if (fabt*fat >= 0) {//���������������������ւ���
			at = abt;
		}
		else {
			bt = abt;
		}
	}
	//���ɋ��߂���t�͋��߂Ă���̂�y���v�Z����
	auto r = 1 - t;
	return t * t*t + 3 * t*t*r*b.y + 3 * t*r*r*a.y;
}


///�������ݖ@(����ɖ���)
float GetYFromXOnBezierHalfSolve(float x, Position2 a, Position2 b, unsigned int n = 8) {
	if (a.x == a.y&&b.x == b.y)return x;//�v�Z�s�v
	float t = x;
	float k0 = 1 + 3 * a.x - 3 * b.x;//t^3�̌W��
	float k1 = 3 * b.x - 6 * a.x;//t^2�̌W��
	float k2 = 3 * a.x;//t�̌W��

	//�덷�͈͓̔����ǂ����Ɏg�p����萔
	const float epsilon = 0.0005f;
	for (int i = 0; i < n; ++i) {
		//f(t)���߂܁[��
		auto ft = k0 * t*t*t + k1 * t*t + k2 * t - x;
		//�������ʂ�0�ɋ߂�(�덷�͈͓̔�)�Ȃ�ł��؂�
		if (ft <= epsilon && ft >= -epsilon)break;
		t -= ft / 2;
	}
	//���ɋ��߂���t�͋��߂Ă���̂�y���v�Z����
	auto r = 1 - t;
	return t * t*t + 3 * t*t*r*b.y + 3 * t*r*r*a.y;
}


using BezierTimeFunc_t = float (*)(float , Position2 , Position2 , unsigned int );

struct Solver {
	string name;
	BezierTimeFunc_t func;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetGraphMode(640, 512, 32);
	ChangeWindowMode(true);
	SetWindowText("�R���x�W�F(����)����");

	array<Position2, 4> cpoints = { Position2(0.0f,512.0f),{170,512-170},{340,512-340},{512,0} };


	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	int currentX = 512;
	int capturedNo = -1;
	int lastMouseInput = 0;
	bool captured = false;
	char keystate[256];
	char lastKeyState[256];
	Solver bzFuncs[] = { {"�j���[�g���@",GetYFromXOnBezierNT} ,
							{"�񕪖@"	,GetYFromXOnBezierBin},
							{"�������ݖ@"	,GetYFromXOnBezierHalfSolve}
							};

	uint8_t currentBzFuncNo = 0;
	uint8_t tryCount = 8;

	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_UP]&&!lastKeyState[KEY_INPUT_UP]) {
			currentBzFuncNo=(currentBzFuncNo-1 + _countof(bzFuncs)) % _countof(bzFuncs);
		}
		if (keystate[KEY_INPUT_DOWN] && !lastKeyState[KEY_INPUT_DOWN]) {
			currentBzFuncNo = (currentBzFuncNo +1) % _countof(bzFuncs);
		}
		if (keystate[KEY_INPUT_RIGHT] && !lastKeyState[KEY_INPUT_RIGHT]) {
			tryCount = min(tryCount + 4, 128);
		}
		if (keystate[KEY_INPUT_LEFT] && !lastKeyState[KEY_INPUT_LEFT]) {
			tryCount = max(tryCount - 4, 4);
		}
		copy(begin(keystate),end(keystate), lastKeyState);

		auto currentMouseInput = GetMouseInput();

		int mx, my;
		GetMousePoint(&mx, &my);
		Position2 mp(mx, my);
		if (currentMouseInput) {
			if (lastMouseInput == 0) {
				for (int i = 1; i < cpoints.size()-1; ++i) {
					auto diff = mp - cpoints[i];
					auto r = hypot(diff.x, diff.y);
					if (r <= 10.0f) {
						captured = true;
						capturedNo = i;
					}
				}
			}
			else {
				if (captured&&capturedNo >= 0) {
					cpoints[capturedNo] = Clamp(mp,Position2(0,0),Position2(512,512));
				}
			}
			lastMouseInput = currentMouseInput;
		}
		else {
			lastMouseInput = 0;
			capturedNo = -1;
			captured = false;
		}

		for (int i = 0; i < cpoints.size() - 1; ++i) {
			DrawLine(cpoints[i].x, cpoints[i].y, cpoints[i + 1].x, cpoints[i + 1].y, 0xff8888);
		}

		DrawBezier(cpoints, 20);

		float x = abs(currentX - 512);

		auto& bzfunc = bzFuncs[currentBzFuncNo];
		DrawFormatString(10, 10, 0xffaaaa, "����=%s", bzfunc.name.c_str());
		DrawFormatString(10, 30, 0xffaaaa, "���s��=%d", tryCount);
		//�x�W�F�֐���0�`1�̑O��ō���Ă��邽�߁A����y���t�����̂��߈����������ʓ|�ɂȂ��Ă���
		auto y = bzfunc.func(x / 512.0f,
									Position2(0, 1) + (cpoints[1] / 512.0f)*Vector2(1, -1),
									Position2(0, 1) + (cpoints[2] / 512.0f)*Vector2(1, -1),
									tryCount);

		y = 512.0f-(512.f*y);//0�`512�֖߂�

		DrawLine(x, 0, x, 512, 0xff0000, 2);
		DrawLine(0, y, 512, y, 0xffff00, 2);
		DrawCircle(x, y, 10, 0xff0000);


		for (const auto& p : cpoints) {
			DrawCircle(p.x, p.y, 10, 0xffffff,false,3);
		}

		DrawBox(512, 0, 640, 512, 0xaaaaaa, true);
		DrawLine(512, y, 640, y, 0x0000ff, 2);
		DrawCircle(580, y, 10, 0xff0000);

		currentX = (currentX + 4) % 1024;
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}