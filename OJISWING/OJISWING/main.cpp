#include<DxLib.h>
#include"Monkey.h"
#include"Ojisan.h"
#include"World.h"

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText("‚¨‚â‚À“Š‚°_ì–ì—³ˆê");
	if(DxLib::DxLib_Init()){
		return -1;
	}
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	DxLib::SetGraphMode(1200,480,32);
	int bgH = LoadGraph("img/background.png");

	Ojisan Ojisan("img/Ojisan.png",1,1);
	Ojisan.SetStartPosition(Ojisan.GetEndPoint().x,0);
	Ojisan.AddAnimInfo(0,0);

	World w(Ojisan);
	int scroll=0;
	bool isStarted = false;
	while(DxLib::ProcessMessage()==0){
		DxLib::ClearDrawScreen();
		float x=Ojisan.GetX();
		if(x>960){
			scroll=960-x;
			//”wŒi‚Ì•\Ž¦
			DrawGraph(scroll,0,bgH,false);
			DrawRotaGraph2(640+scroll,0,0,0,1.0,0.0,bgH,false,true);
			DrawGraph(scroll+1280,0,bgH,false);
			DrawRotaGraph2(1920 + scroll, 0, 0, 0, 1.0, 0.0, bgH, false, true);
			Ojisan.SetScroll(scroll,0);
		}else{
			//”wŒi‚Ì•\Ž¦
			DrawGraph(0,0,bgH,false);
			DrawRotaGraph2(640,0,0,0,1.0,0.0,bgH,false,true);
		}
		if (DxLib::CheckHitKey(KEY_INPUT_RETURN)) {
			isStarted = true;
		}
		if (isStarted) {
			Ojisan.Update();
		}
		DxLib::ScreenFlip();
	}
	return 0;
}