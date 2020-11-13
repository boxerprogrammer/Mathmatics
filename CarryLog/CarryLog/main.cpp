#include<DxLib.h>
#include<cmath>
#include"Geometry.h"


using namespace std;

///�J�v�Z���`��̕`��
void DrawCapsule(const Capsule& cap,unsigned int color) {
	DrawCircle(cap.posA.x, cap.posA.y, cap.radius, color);
	DrawCircle(cap.posB.x, cap.posB.y, cap.radius, color);

	auto v = cap.posB - cap.posA;
	auto angle = atan2(v.y, v.x)-DX_PI_F/2.0f;
	auto w = cos(angle)*cap.radius ;
	auto h = sin(angle)*cap.radius ;

	DrawQuadrangle(
		cap.posA.x - w, cap.posA.y - h,
		cap.posB.x - w, cap.posB.y - h,
		cap.posB.x + w, cap.posB.y + h,
		cap.posA.x + w, cap.posA.y + h, color, true);
}

void DrawWood(const Capsule& cap, int handle) {


	auto v = cap.posB - cap.posA;
	auto angle = atan2(v.y, v.x) + DX_PI_F / 2.0f;
	auto w = cos(angle)*cap.radius;
	auto h = sin(angle)*cap.radius;

	DrawModiGraph(
		cap.posA.x - w, cap.posA.y - h,
		cap.posB.x - w, cap.posB.y - h,
		cap.posB.x + w, cap.posB.y + h,
		cap.posA.x + w, cap.posA.y + h, handle,true);
}


///�Ƃ���_�𒆐S�ɉ�]���āA���̉�]��̍��W��Ԃ��܂��B
///@param center ��]���S�_
///@param angle ��]�p�x
///@param pos ���̍��W
///@return �ϊ���̍��W
Matrix RotatePosition(const Position2& center, float angle) {
	//�@���S�����_�ɕ��s�ړ�����
	//�A���_���S�ɉ�]����
	//�B���S�����̍��W�֖߂�

	Matrix mat = MultipleMat(TranslateMat(center.x, center.y),
		MultipleMat(RotateMat(angle),
			TranslateMat(-center.x, -center.y)));
	return mat;
	//��������������āA����̓_�𒆐S�ɉ�]���s���悤�ɂ��Ă��������B
}

//�J�v�Z���Ɖ~�������������H
bool IsHit(const Capsule& cap, const Circle& cc) {
	auto L = cap.posB - cap.posA;
	auto T = cc.pos - cap.posA;
	auto t = Dot(T, L) / Dot(L, L);
	t = min(max(0.0f, t), 1.0f);

	auto pos = cap.posA + L * t;

	return (pos - cc.pos).Magnitude() <= cap.radius + cc.radius;
	
}
void OnDead(Position2& exppos, Position2& pos, int& expFrame, bool& isDead, Vector2& vel, int explodeSE);
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetGraphMode(512, 768, 32);
	SetWindowText("0000000_�Z���Z�[");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	int sw, sh;//�X�N���[�����A����
	GetDrawScreenSize(&sw, &sh);

	auto woodH = LoadGraph("img/wood.png");
	int wdW, wdH;
	GetGraphSize(woodH, &wdW, &wdH);
	wdW = 200;

	auto cascadeH = LoadGraph("img/cascade_chip.png");
	auto chipH = LoadGraph("img/atlas0.png");
	auto rockH = LoadGraph("img/rock.png");
	int explosionH[12];
	LoadDivGraph("img/explosion.png", 12, 12, 1, 128, 128, explosionH);
	Position2 expPos;
	int expFrame = 0;
	int marioH = LoadGraph("img/mario.png");
	int luigeH = LoadGraph("img/luigi.png");
	int clearH = LoadGraph("img/clear.png");
	int clearSE = LoadSoundMem("se/clear.wav");
	int bgm = LoadSoundMem("se/bgm.wav");
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
	int explodeSE = LoadSoundMem("se/explosion.wav");
	int downSE = LoadSoundMem("se/down.wav");

	Capsule cap(20,Position2((sw-wdW)/2,sh-100),Position2((sw - wdW) / 2+wdW,sh-100));

	Position2 rockPos(200,-10);

	char keystate[256];
	
	float angle = 0.0f;

	int frame = 0;
	bool isLeft = false;

	bool isDead = false;
	bool isDown = false;
	bool isClear = false;

	float downangle = 0.0f;

	Vector2 _vel;
	SetFontSize(24);

	Position2 center=(cap.posA+cap.posB)*0.5f;

	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		GetHitKeyStateAll(keystate);

		DrawBox(0, 0, sw, sh, 0xaaffff, true);

		int mx=0, my=0;
		if (!isDead) {
			if (keystate[KEY_INPUT_LEFT]) {
				isLeft = true;
			}
			else if (keystate[KEY_INPUT_RIGHT]) {
				isLeft = false;
			}

			if (isLeft) {
				mx = cap.posA.x;
				my = cap.posA.y;
			}
			else {
				mx = cap.posB.x;
				my = cap.posB.y;
			}

			if (keystate[KEY_INPUT_Z]) {

				angle = -0.05f;
			}
			else if (keystate[KEY_INPUT_X]) {

				angle = 0.05f;
			}
			else {
				angle = 0.0f;
			}

			//�J�v�Z����]
			Matrix rotMat = RotatePosition(Position2(mx, my), angle);
			cap.posA = MultipleVec(rotMat, cap.posA);
			cap.posB = MultipleVec(rotMat, cap.posB);
		}
		//�w�i�̕`��
		//��
		int chipIdx = (frame/4) % 3;
		constexpr int dest_chip_size = 32;
		int destY = 48;
		while (destY < sh) {
			int destX = 0;
			while (destX < sw) {
				DrawRectExtendGraph(
					destX, destY, destX + dest_chip_size, destY+dest_chip_size,
					chipIdx * 16, 0,
					16, 16,
					cascadeH, false);
				destX += dest_chip_size;
			}
			destY += dest_chip_size;
		}

		//���̃A��������
		DrawRectGraph(0, 0, 96, 0, 32, 32, chipH, true);
		DrawRectGraph(sw - 32, 0, 32, 0, 32, 32, chipH, true);
		DrawRectGraph(0, 32, 96, 32, 32, 32, chipH, true);
		DrawRectGraph(sw-32, 32, 32, 32, 32, 32, chipH, true);
		destY = 64;
		while (destY < sh) {
			DrawRectGraph(0, destY, 96, 64, 32, 32, chipH, true);
			DrawRectGraph(sw - 32, destY, 32, 64, 32, 32, chipH, true);
			destY += dest_chip_size;
		}

		//���I�I
		if (isDead) {

			DrawRotaGraph(cap.posA.x, cap.posA.y, 3.0f, 0.0f, marioH, true);
			DrawRotaGraph(cap.posB.x, cap.posB.y, 3.0f, 0.0f, luigeH, true);
			if (expFrame < 12) {
				DrawRotaGraph2(expPos.x, expPos.y, 64, 100, 1.0f, 0.0f, explosionH[expFrame], true);
				++expFrame;
			}
			cap.posA += _vel;
			cap.posB += _vel;
			_vel.y += 0.3f;
			if (cap.posA.y > sh&&cap.posB.y > sh&&expFrame >= 12) {
				//���Z�b�g
				cap = Capsule(20, Position2((sw - wdW) / 2, sh - 100), Position2((sw - wdW) / 2 + wdW, sh - 100));
				isDead = false;
				rockPos = Position2(200, -10);
			}

		}
		else if (isDown) {
			DrawRotaGraph(center.x, center.y,0.6f, downangle, woodH, true);
			downangle += 0.2f;
			center+=_vel;
			_vel.y += 0.2f;
			if (center.y > sh+500) {
				isDown = false;
				//���Z�b�g
				cap = Capsule(20, Position2((sw - wdW) / 2, sh - 100), Position2((sw - wdW) / 2 + wdW, sh - 100));
				isDown = false;
				downangle = 0.0f;
				rockPos = Position2(200, -10);
				_vel.y = 0;
			}
		}
		else if (isClear) {
			int cw, ch;
			GetGraphSize(clearH, &cw, &ch);
			DrawGraph((sw - cw) / 2, (sh - ch) / 2, clearH, true);
			DrawFormatString((sw - cw) / 2+130+2, (sh + ch) / 2+20+2, 0x000000, "�N���A�t���[��=%d", frame);
			DrawFormatString((sw - cw) / 2+130, (sh + ch) / 2+20,0xffffff,"�N���A�t���[��=%d",frame);
		}
		else{
			DrawWood(cap, woodH);
			DrawCircle(mx, my, 30, 0xff0000, false, 3);
			rockPos.y += 1.5f;
			if (rockPos.y >= sh) {
				rockPos.y = -10;
				rockPos.x = (cap.posA.x + cap.posB.x) / 2;
			}
			DrawRotaGraph(rockPos.x, rockPos.y, 2.0f, 0.0f, rockH, true);
			//�����蔻��(�P����)
			Circle c(5, rockPos);
			if (IsHit(cap, c)){ 
				OnDead(expPos, c.pos,expFrame,isDead,_vel,explodeSE);
			}
			else if (min(cap.posA.x, cap.posB.x) < 32) {
				Position2 pos = cap.posA.x < cap.posB.x ? cap.posA : cap.posB;
				OnDead(expPos, pos, expFrame, isDead, _vel, explodeSE);
			}else if(max(cap.posA.x, cap.posB.x) > sw - 32) {
				Position2 pos = cap.posA.x > cap.posB.x ? cap.posA : cap.posB;
				OnDead(expPos, pos, expFrame, isDead, _vel, explodeSE);
			}
			else if (cap.posA.y > sh + 32 && cap.posB.y > sh + 32) {
				Position2 pos = Position2((cap.posA.x+cap.posB.x)/2,sh);
				OnDead(expPos, pos, expFrame, isDead, _vel, explodeSE);
			}
			if (max(cap.posA.y, cap.posB.y) < 32 - cap.radius) {
				isClear = true;
				StopSoundMem(bgm);
				PlaySoundMem(clearSE, DX_PLAYTYPE_BACK);
			}

			if (cap.posA.x > cap.posB.x) {
				isDown = true;
				angle = 0.0f;
				center = (cap.posA + cap.posB)*0.5f;
				PlaySoundMem(downSE, DX_PLAYTYPE_BACK);
			}

			cap.posA.y += 1.0f;
			cap.posB.y += 1.0f;

			DrawFormatString(12,12, 0xffffff, "�o�߃t���[����=%d", frame);
			DrawFormatString(10,10, 0x000000, "�o�߃t���[����=%d", frame);
		}
		if (!isClear) {
			++frame;
		}
		ScreenFlip();
	}

	DxLib_End();
}

void OnDead(Position2& exppos,Position2& pos,int& expFrame, bool& isDead,Vector2& vel,int explodeSE) {
	exppos = pos;
	expFrame = 0;
	isDead = true;
	vel = Vector2(0, -3);
	PlaySoundMem(explodeSE, DX_PLAYTYPE_BACK);
}