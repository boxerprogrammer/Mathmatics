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

Position2 operator+(const Position2& lval, const Position2& rval) {
	return Position2(lval.x + rval.x, lval.y + rval.y);
}

Position2 operator-(const Position2& lval, const Position2& rval) {
	return Position2(lval.x - rval.x, lval.y - rval.y);
}


Position2 operator*(const Position2& lval, float scale) {
	return Position2(lval.x *scale,lval.y*scale);
}

vector<Position2> CalculateInterporatedPoints(const array<Position2, 4>& cpnt,int divNum=10) {
	vector<Position2> points(divNum);
	for (int i = 0; i < divNum; ++i) {
		float t = static_cast<float>(i) / static_cast<float>(divNum);
		float invt = 1.0-t;
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
		DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, 0xaaaaff,3);
	}
	auto& p = points[points.size() - 1];
	DrawLine(p.x,p.y, cpnt[3].x, cpnt[3].y, 0xaaaaff, 3);
}



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetGraphMode(512, 512, 32);
	ChangeWindowMode(true);
	SetWindowText("‚RŽŸƒxƒWƒFŽÀŒ±");

	array<Position2,4> cpoints = { Position2(10.0f,10.0f),{170,170},{340,340},{502,502} };

	int capturedNo = -1;
	int lastMouseInput = 0;
	bool captured = false;
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	while (ProcessMessage()==0) {
		ClearDrawScreen();
		
		auto currentMouseInput = GetMouseInput();
		
		int mx, my;
		GetMousePoint(&mx, &my);
		Position2 mp(mx,my);
		if (currentMouseInput) {
			if (lastMouseInput == 0) {
				for (int i = 0; i < cpoints.size() ; ++i) {
					auto diff = mp - cpoints[i];
					auto r = hypot(diff.x, diff.y);
					if (r <= 10.0f) {
						captured = true;
						capturedNo = i;
					}
				}
			}
			else {
				if (captured&&capturedNo>=0) {
					cpoints[capturedNo] = mp;
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
			DrawLine(cpoints[i].x, cpoints[i].y, cpoints[i + 1].x, cpoints[i + 1].y, 0xff8888,2);
		}
		
		DrawBezier(cpoints,20);
		
		for (const auto& p : cpoints) {
			DrawCircle(p.x, p.y, 10, 0xffffff);
		}
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}