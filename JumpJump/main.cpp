#include<DxLib.h>
#include<memory>
#include<vector>
#include"Jumper.h"

using namespace std;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	int bg = LoadGraph(L"img/bg.png");

	vector<shared_ptr<Actor>> jumpers;
	jumpers.push_back(make_shared<Jumper>());


	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		DrawGraph(0, 0, bg, true);

		for (auto& jmp : jumpers) {
			jmp->Update();
			jmp->Draw();
		}

		ScreenFlip();
	}
	DxLib_End();
}