#include<DxLib.h>
#include<cmath>
#include"Geometry.h"

///当たり判定関数
///@param posA Aの座標
///@param radiusA Aの半径
///@param posB Bの座標
///@param radiusB Bの半径
bool IsHit(const Position2& posA, float radiusA, const Position2& posB,  float radiusB); {
	//当たり判定を実装してください
	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetMainWindowText("弾幕だよ～");
	if (DxLib_Init() != 0) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	auto bulletH=LoadGraph("img/bullet.png");
	int playerH[10];
	LoadDivGraph("img/player.png", 10, 5, 2, 16, 24,playerH );

	int enemyH[2];
	LoadDivGraph("img/enemy.png", 2, 2, 1, 32, 32, enemyH);

	struct Bullet {
		Position2 pos;//座標
		Vector2 vel;//速度
		bool isActive = false;//生きてるか～？
	};

	//弾の半径
	float bulletRadius = 5.0f;

	//自機の半径
	float playerRadius = 10.0f;

	//適当に256個くらい作っとく
	Bullet bullets[256];

	Position2 enemypos(320,25);//敵座標
	Position2 playerpos(320, 400);//自機座標

	unsigned int frame = 0;//フレーム管理用

	char keystate[256];
	bool isDebugMode = false;
	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);

		isDebugMode = keystate[KEY_INPUT_P];

		//プレイヤー
		if (keystate[KEY_INPUT_RIGHT]) {
			playerpos.x = min(640,playerpos.x+4);
		}
		else if (keystate[KEY_INPUT_LEFT]) {
			playerpos.x = max(0,playerpos.x-4);
		}
		if (keystate[KEY_INPUT_UP]) {
			playerpos.y = max(0,playerpos.y-4);
		}else if (keystate[KEY_INPUT_DOWN]) {
			playerpos.y = min(480,playerpos.y+4);
		}

		int pidx = (frame/4 % 2)*5+3;
		DrawRotaGraph(playerpos.x, playerpos.y, 2.0f, 0.0f, playerH[pidx], true);
		if (isDebugMode) {
			//自機の本体(当たり判定)
			DrawCircle(playerpos.x, playerpos.y, playerRadius, 0xffaaaa, false, 3);
		}

		//弾発射
		if (frame % 12 == 0) {
			for (auto& b : bullets) {
				if (!b.isActive) {
					b.pos = enemypos;
					b.vel = Vector2(0, 5);

					b.vel = (playerpos - b.pos).Normalized()*5.0f;

					b.isActive = true;
					break;
				}
			}
		}

		//弾の更新および表示
		for (auto& b : bullets) {
			if (!b.isActive) {
				continue;
			}

			//弾の現在座標に弾の現在速度を加算してください
			b.pos += b.vel;
			float angle = 0.0f;
			//弾の角度をatan2で計算してください。
			angle = atan2(b.vel.y, b.vel.x);
			DrawRotaGraph(b.pos.x, b.pos.y,1.0f,angle, bulletH, true);
			
			if (isDebugMode) {
				//弾の本体(当たり判定)
				DrawCircle(b.pos.x, b.pos.y, bulletRadius, 0x0000ff, false, 3);
			}
			//弾を殺す
			if (b.pos.x + 16 < 0 || b.pos.x - 16 > 640 ||
				b.pos.y + 24 < 0 || b.pos.y - 24 > 480) {
				b.isActive = false;
			}

			//あたり！
			if (IsHit(b.pos, bulletRadius, playerpos, playerRadius)) {
				//当たった反応を書いてください。
			}
		}

		//敵の表示
		enemypos.x = abs((int)((frame+320) % 1280) - 640);
		int eidx = (frame / 4 % 2);
		DrawRotaGraph(enemypos.x, enemypos.y, 2.0f, 0.0f, enemyH[eidx],true);

		if (isDebugMode) {
			//敵の本体(当たり判定)
			DrawCircle(enemypos.x, enemypos.y, 5, 0xffffff, false, 3);
		}
		++frame;
		ScreenFlip();
	}

	DxLib_End();

	return 0;
}