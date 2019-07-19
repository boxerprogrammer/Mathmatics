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
	int skyH = LoadGraph("img/sky.png");
	struct Size {
		int w, h;
	};
	Size bgSize;
	GetGraphSize(bgH, &bgSize.w, &bgSize.h);

	Size skySize;
	GetGraphSize(skyH, &skySize.w, &skySize.h);

	ScreenShaker shaker;
	Ojisan Ojisan("img/Ojisan.png",1,1,shaker);
	Ojisan.SetStartPosition(Ojisan.GetEndPoint().x,0);
	Ojisan.AddAnimInfo(0,0);

	World w(Ojisan);
	int scrollx=0;
	int scrolly = 0;
	bool isStarted = false;
	
	while(DxLib::ProcessMessage()==0){
		shaker.PrepareShake();
		DxLib::ClearDrawScreen();
		auto& ojipos = Ojisan.GetPosition();
		float x=ojipos.x-320;
		float y = ojipos.y -430;

		scrollx=max(0,x);
		if (Ojisan.IsFlying()) {
			scrolly = y;//// max(0, y);
		}
		DrawGraph(-(scrollx %(2* bgSize.w)), -scrolly, bgH, false);
		DrawRotaGraph2(-(scrollx % (2 * bgSize.w))+bgSize.w, -scrolly, 0, 0, 1.0, 0.0, bgH, false, true);
		DrawGraph(-(scrollx % (2 * bgSize.w))+bgSize.w*2, -scrolly, bgH, false);
		DrawRotaGraph2(-(scrollx % (2 * bgSize.w)) + bgSize.w*3, -scrolly, 0, 0, 1.0, 0.0, bgH, false, true);

		for (int i = 1; i <= 4;++i) {
			int baseY = -512 * i;
			DrawGraph(-(scrollx % (2 * skySize.w)), baseY - scrolly, skyH, false);
			DrawGraph(-(scrollx % (2 * skySize.w)) + skySize.w, baseY - scrolly, skyH, false);
			DrawGraph(-(scrollx % (2 * skySize.w)) + skySize.w * 2, baseY - scrolly, skyH, false);
			DrawGraph(-(scrollx % (2 * skySize.w)) + skySize.w * 3, baseY - scrolly, skyH, false);
		}
			
		DrawFormatString(-scrollx + bgSize.w * (scrollx / (4 * bgSize.w)) * 4, 400, 0xff0000, "%d", 0);
				
		Ojisan.SetScroll(scrollx, scrolly);

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