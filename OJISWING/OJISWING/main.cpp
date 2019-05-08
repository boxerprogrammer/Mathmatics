#include<DxLib.h>
#include"Monkey.h"
#include"Ojisan.h"
#include"World.h"
#include"ScreenShaker.h"

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText("Ç®Ç‚Ç¿ìäÇ∞_êÏñÏó≥àÍ");
	if(DxLib::DxLib_Init()){
		return -1;
	}
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	DxLib::SetGraphMode(960,480,32);
	int bgH = LoadGraph("img/background.png");
	struct Size {
		int w, h;
	};
	Size bgSize;
	GetGraphSize(bgH, &bgSize.w, &bgSize.h);
	ScreenShaker shaker;
	Ojisan Ojisan("img/Ojisan.png",1,1,shaker);
	Ojisan.SetStartPosition(Ojisan.GetEndPoint().x,0);
	Ojisan.AddAnimInfo(0,0);

	World w(Ojisan);
	int scroll=0;
	bool isStarted = false;
	
	while(DxLib::ProcessMessage()==0){
		shaker.PrepareShake();
		DxLib::ClearDrawScreen();
		float x=Ojisan.GetX()-320;

		scroll=max(0,x);
		DrawGraph(-(scroll %(2* bgSize.w)), 0, bgH, false);
		DrawRotaGraph2(-(scroll % (2 * bgSize.w))+bgSize.w, 0, 0, 0, 1.0, 0.0, bgH, false, true);
		DrawGraph(-(scroll % (2 * bgSize.w))+bgSize.w*2, 0, bgH, false);
		DrawRotaGraph2(-(scroll % (2 * bgSize.w)) + bgSize.w*3, 0, 0, 0, 1.0, 0.0, bgH, false, true);
			
		DrawFormatString(-scroll + bgSize.w * (scroll / (4 * bgSize.w)) * 4, 400, 0xff0000, "%d", 0);
				
		Ojisan.SetScroll(scroll,0);

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN)) {
			isStarted = true;
		}
		if (isStarted) {
			Ojisan.Update();
		}
		
		shaker.UpdateShake();

		DxLib::ScreenFlip();
	}
	return 0;
}