#include<DxLib.h>
#include"geometry.h"
#include"Graphics2D.h"
#include<array>
#include<random>
#include<cassert>
#include<vector>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	ChangeWindowMode(TRUE);
	SetGraphMode(640, 480, 32);
	if (DxLib_Init() == -1) {
		return -1;
	}
	int RT=MakeScreen(640, 480,true);
	int RTShrink=MakeScreen(320, 240,true);
	int RT2=MakeScreen(640, 480,true);
	int RT3 = MakeScreen(640, 480, true);
	int RTNormal = MakeScreen(640, 480);
	struct Ball {
		Circle c;
		Vector2 v;
	};
	std::array<Ball, 600> balls;
	SRand(GetTickCount64());
	for (int i = 0; i < balls.size(); i++) {
		balls[i].c.pos.x = rand() % 640;
		balls[i].c.pos.y = 0-rand() % 480;
		balls[i].c.radius = 10;
		balls[i].v = {};
	}
	auto psH=LoadPixelShader(L"Hight2Normal.pso");
	auto psRender = LoadPixelShader(L"Rendering2D.pso");
	auto psNoise = LoadPixelShader(L"Noise.pso");
	auto backH = LoadGraph(L"img/back.jpg");
	struct CBuffer {
		float time;
		float padding[3];
	};
	static auto lastTick = GetTickCount64();
	int cbuffer = CreateShaderConstantBuffer(sizeof(CBuffer));
	auto pCBuff = static_cast<CBuffer*>(GetBufferShaderConstantBuffer(cbuffer));
	pCBuff->time = ((GetTickCount64() - lastTick)) / 1000.0f;
	UpdateShaderConstantBuffer(cbuffer);

	WaitKey();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		pCBuff->time = ((GetTickCount64() - lastTick)) / 1000.0f;
		UpdateShaderConstantBuffer(cbuffer);
		SetDrawScreen(RT);
		FillGraph(RTShrink,0, 0, 0);
		ClearDrawScreen();

		std::sort(balls.begin(), balls.end(), [](const Ball& a, const Ball& b) {
			return a.c.pos.y < b.c.pos.y;
			});

		//更新処理
		for (int i = 0; i < balls.size(); i++) {
			balls[i].v += Vector2(0, 0.1f);
			balls[i].c.pos += balls[i].v;
			if (IsHitFloor(balls[i].c, 480)) {
				balls[i].v += GetHitFloorNormal(balls[i].c, 480)*0.1;
				balls[i].c.pos += GetHitFloorNormal(balls[i].c, 480);
			}
			if (IsHitWallL(balls[i].c, 0)) {
				balls[i].v += GetHitWallLNormal(balls[i].c, 0)*0.1;
				balls[i].c.pos += GetHitWallLNormal(balls[i].c, 0);
			}
			if (IsHitWallR(balls[i].c, 640)) {
				balls[i].v += GetHitWallRNormal(balls[i].c, 640)*0.1;
				balls[i].c.pos += GetHitWallRNormal(balls[i].c, 640);
			}
		}
		//ボールとボールの衝突判定
		for (int i = 0; i < balls.size(); i++) {
			for (int j = i + 1; j < balls.size(); j++) {
				if (IsHit(balls[i].c, balls[j].c)) {
					auto overlap = GetOverlapVector(balls[i].c, balls[j].c);
					balls[i].c.pos -= overlap * 0.5;
					balls[j].c.pos += overlap * 0.5;
				}
			}
		}

		//描画処理
		for (int i = 0; i < balls.size(); i++) {
			balls[i].c.Draw();
		}
		SetDrawScreen(RT2);
		ClearDrawScreen();
		GraphFilterBlt(RT, RTShrink, DX_GRAPH_FILTER_DOWN_SCALE, 2);
		
		//DrawGraph(0, 0, RT, true);
		GraphFilter(RT, DX_GRAPH_FILTER_GAUSS, 16,5000);
		GraphFilter(RTShrink, DX_GRAPH_FILTER_GAUSS, 16,5000);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawGraph(0, 0, RT, true);
		DrawExtendGraph(0, 0, 640, 480, RTShrink, true);
		DrawGraph(0, 0, RT, true);
		DrawExtendGraph(0, 0, 640, 480, RTShrink, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		std::vector<int> textures = {
			RT2
		};
		SetDrawScreen(RT3);
		ClearDrawScreen();
		Graphics2D::DrawGraph(0, 0, textures, psNoise, true);

		SetDrawScreen(RTNormal);
		ClearDrawScreen();
		textures = {
			RT3
		};
		SetShaderConstantBuffer(cbuffer,DX_SHADERTYPE_PIXEL,4);
		Graphics2D::DrawGraph(0, 0, textures, psH, true);
		SetShaderConstantBuffer(-1,DX_SHADERTYPE_PIXEL,4);

		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		DrawExtendGraph(0, 0, 640, 480, backH, true);
		GraphFilter(RT2, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 128, TRUE, GetColor( 0, 0, 0 ), 0 ) ;
		GraphFilter( RT2, DX_GRAPH_FILTER_TWO_COLOR, 128, GetColor( 0, 0, 0), 0, GetColor( 128, 255, 192 ), 255 ) ;
		textures.clear();
		textures = {
			RT2,
			RTNormal,
			backH
		};
		Graphics2D::DrawGraph(0, 0, textures, psRender,true);
		//DrawGraph(0, 0, RTNormal, false);
		//描画処理ここまで
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}