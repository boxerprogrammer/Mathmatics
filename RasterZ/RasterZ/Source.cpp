#include<Dxlib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	int sw, sh, dep;
	GetScreenState(&sw,&sh,&dep);
	auto map=LoadGraph("map.png");

	
	

	int gw, gh;
	GetGraphSize(map, &gw, &gh);
	
	auto base=MakeScreen(gw, gh);

	char keystate[256];
	int x=0, z=0;
	float angle = 0.0f;
	while (ProcessMessage() != -1) {
		GetHitKeyStateAll(keystate);
		SetDrawScreen(base);
		if (keystate[KEY_INPUT_LEFT]) {
			x--;
		}
		if (keystate[KEY_INPUT_RIGHT]) {
			x++;
		}
		if (keystate[KEY_INPUT_UP]) {
			z--;
		}
		if (keystate[KEY_INPUT_DOWN]) {
			z++;
		}
		if (keystate[KEY_INPUT_Z]) {
			angle += 0.01f;
		}
		if (keystate[KEY_INPUT_X]) {
			angle -= 0.01f;
		}
		//DrawGraph(0, 0, map, false);
		DrawRotaGraph2(x, z, x, z, 1.0, angle, map, true);
		DrawCircle(x, z, 10, 0xff0000);
		SetDrawScreen(DX_SCREEN_BACK);

		int y = 0;
		int h = 1;
		int dy = 1;
		int sy = 0;
		while (y < sh) {
			int startX = -x + sw / 2;
			int startY = -z + sh / 2;
			int mw = gw;
			int mh = gh;
			float rate = (64.0f - (float)h) / 64.0f;
			DrawRectExtendGraph(
				0, y,
				sw,y+h,
				//sw / 2 + h * gw / 2, y + h,
				//0, 0 + sy,
				//gw, 1,
				x-sw/2*rate*0.5,sy+z,
				sw*rate*0.5, 4,
				base, true);
			y += h;
			//Œ»Ý’n‚ª‰æ–Ê‚Ì^‚ñ’†‚É—ˆ‚é‚æ‚¤‚É
			//DrawGraph(-x+sw/2, -z+sh/2, base,false);
			h += 1;
			sy+=4;
		}
		DrawRotaGraph(0.1*gw/2, 0.1*gh/2, 0.1, 0.0, base, true);
		//DrawCircle(x/5, z/5, 5, 0xff0000);
		ScreenFlip();
	}

	DxLib_End();
}