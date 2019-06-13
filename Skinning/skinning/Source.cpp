#include<DxLib.h>
#include"Geometry.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	DxLib::SetMainWindowText("スキニング");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		Position2 startPos(0, 0);
		Position2 endPos(100, 100);



		DrawQuadrangle(0, 0, 30, 10, 100, 100, 20, 40, 0xffffff, false);

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}