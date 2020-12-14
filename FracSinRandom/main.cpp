#include<iostream>
#include<cmath>
#include<Dxlib.h>
#include<functional>
using namespace std;

float Frac(float val) {
	val=val - static_cast<int>(val);
	return val<0.0f?1.0f+val:val;
}

int screen_width = 800;
int screen_height = 600;

void DrawFunction(float minX,float minY,float maxX,float maxY,
	function<float(float)> func,float scale,size_t div) {
	float stepX = (maxX - minX) / static_cast<float>(div);
	float x = minX;
	float y = 0.0f;
	for (size_t i = 0; i < div; ++i) {
		y = func(x);
		int px = x * scale + screen_width / 2;
		int py = screen_height / 2+200 - y * scale*2;
		DrawPixel(px,py,0xffffff);
		x += stepX;
	}
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, 32);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	char keystate[256] = {};
	char lastKeystate[256] = {};
	float n = 0.0f;
	float scale = 1.0f;
	while (ProcessMessage()!=-1) {
		ClearDrawScreen();
		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_UP] && !lastKeystate[KEY_INPUT_UP]) {
			n = min(n + 1, 5);
		}
		if (keystate[KEY_INPUT_DOWN] && !lastKeystate[KEY_INPUT_DOWN]) {
			n = max(n - 1, 0);
		}
		scale = pow(10.0f, n);
		DrawFunction(-4, -1, 4, 1, 
			[scale](float x)->float {
				return Frac(sin(x)*scale);
			}, 200.0f, 100000);
		DrawFormatString(10, 10, 0xffaaaa, L"frac(sin(x)*%d)", (int)scale);
		ScreenFlip();
		copy(begin(keystate), end(keystate), begin(lastKeystate));
	}
	DxLib_End();

}