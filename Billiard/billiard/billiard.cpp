#include<DxLib.h>
#include<vector>
#include<list>
#include<algorithm>
#include<cmath>
#include"Ball.h"
#include"Cue.h"
#include"HitEffect.h"
#include"ScreenShaker.h"

int _ballCollisionSound = 0;

std::list<HitEffect> _hiteffectList;
int _hitEffectH = 0;

//���܂��A�{�[�����g�̋������L�q���邽��Ball.cpp��Update�֐����炢�����Ă��������B


//���֗��\���̂Ƃ���Vector2�Ƃ����̂�p�ӂ��Ă��܂��B
//Normalize()�Ő��K�����ANormalized()�Ő��K���ς݃x�N�g����Ԃ��܂��B
//Dot(a,b)�͓��ϒl��Ԃ��܂��B�����𗘗p���ăr�����[�h�Q�[�������������Ă��������B
//
//�Œ���A�{�[�����m�̏Փ˔���ƁA�Փ˂ɂ�鑬�x�x�N�g���̎󂯓n�����ł��Ă��邱�Ƃ�
//�]���v���ł��B
//
//�Ƃ肠����������̂�billiard.cpp�����ŏ\���Ȃӂ��ɍ���Ă��܂����A�ł���l�͐F�X�Ƃ�������OK
//
//�ł���l�̓{�[���𑝂₵����A���Q�[�������グ����A�H�v���ăJ�b�R�悭�����肵�Ă�������
//

///�~�ƁA�~�̂����蔻��
///@param a �~A
///@param b �~B
///@retval true ������
///@retval false �͂���
bool IsHit(const Circle& a,const Circle& b) {
	//�������Ă���true�A�O��Ă���false��Ԃ��΂�������


	float xdiff = b.pos.x - a.pos.x;//X��������
	float ydiff = b.pos.y - a.pos.y;//Y��������
	float totalRadius = a.radius + b.radius;//��̉~�̔��a�𑫂�������

	//�B�����蔻��̃R�[�h�������Ă�������
	return totalRadius*totalRadius>=xdiff*xdiff + ydiff*ydiff;//������OC�ł͏����܂�

	bool ret = false;
	if (false) {//�B�����ɓ����邽�߂̏����������Ă�������
		//�B����������ɍ��v������ret��true�������Ă�������
		//�~�̓����蔻��Ƃ����̂́A
		//���݂��̉~�̔��a�𑫂������̂�蒆�S�_�Ԃ̋������߂���Γ������Ă���
		//�����łȂ���Γ������Ă��Ȃ��Ƃ������̂ł��B

	}

	return ret;
}

///�{�[���̏Փ˃`�F�b�N
///@param a �{�[��A
///@param b �{�[��B
///@remarks �{�[��A�ƃ{�[��B�̏Փ˂��`�F�b�N���A
///�����Փ˂��Ă���Α��x�x�N�g���̎󂯓n�����s���܂�
void CheckBallCollision(Ball& a,Ball& b) {
	if (!IsHit(a.GetCircle(), b.GetCircle())) {
		return;//�������ĂȂ������炳�����ƃ��^�[��()
	}

	//�������Ă����瑬�x�x�N�g���̎󂯓n�����s���Ă�������
	//A�̖@������������B�ɁAB�̖@������������A�ɓn���Ă�������

	//�Փ˂������Ƃ̏Փˌ��ƏՓː�̑��x�x�N�g�����v�Z���Ă����܂��B
	//�P�D�Փ˒n�_�ł̖@���������v�Z
	//�Q�D�P�̃x�N�g���𐳋K��
	//�R�DA�̑��x�x�N�g���̖@������������B��(���ς��g�p)
	//		��B�ɂ��������̃x�N�g���͈����Ă�������
	//�S�DB�̑��x�x�N�g���̋t�@������������A��(���ς��g�p)
	//		��A�ɂ������Ԃ�̃x�N�g���͈����Ă�������
	//�����K����Normalize�֐��A���ς�Dot�֐����g�p�ł��܂�
	
	//�C�@�������̃x�N�g�����v�Z
	Vector2 normal= b.GetCircle().pos - a.GetCircle().pos;
	//���ꂼ��̒��S�_�������Z���Ă��������B���ꂼ��̒��S�_��GetCircle().pos�œ����܂��B

	float diff = normal.Magnitude();
	//�H�␳�p�ɂ߂荞�ݗʂ��v�Z�c�Ō�A���Ԃ��]��������
	diff=a.GetRadius() + b.GetRadius() - diff;


	if (diff >= 1.0f) {
		//�������Ă�̂ŉ���炷���A�G�t�F�N�g���o��
		DxLib::ChangeVolumeSoundMem(min(255, (int)(diff * 80)), _ballCollisionSound);
		DxLib::PlaySoundMem(_ballCollisionSound, DX_PLAYTYPE_BACK);
		_hiteffectList.push_back(HitEffect(_hitEffectH,(a.GetCircle().pos + b.GetCircle().pos)*0.5f,(diff*80.0f/256.0f)));
	}


	//�D-1�Փˌ�̓������v�Z���܂�
	//�v�Z�̓s����A�@���x�N�g���̑傫�����P�ɂ��Ă����܂��B
	//�傫�����P�ɂ���ɂ́u�x�N�g���̐��K���v�Ƃ����̂��s���܂��B
	//�{���̓x�N�g���̑傫������(x*x+y*y)�Ōv�Z���āA���̑傫���Ńx�N�g���̂��ꂼ���
	//�����������č��̂ł����A�ꕔ�̐l�ɂ͓����������Ȃ��̂�Normalize�Ƃ����֐���
	//����Ă��܂��̂ŁANormalize()�Ə����Ă��������B
	normal.Normalize();

	//�D-2�߂荞��ł���ꍇ�ca�̍��W��␳����
	//�Q�[���̐��E�͌����ƈႤ���߁A�v�Z��Q�̕��̂��d�Ȃ邱�Ƃ�����܂��B
	//���R�̂��ƂȂ��炠�蓾�Ȃ����ۂ̂��߁A�����͓����̏����ŏd�Ȃ��␳���܂�
	//�␳�ƌ������t�������������܂��񂪁A�v�͏d�Ȃ��ĂȂ��󋵂ɂ��邽�߂�
	//���ꂼ��̃{�[���̍��W���u�߂荞�ݔ��Ε����v�Ɉړ������Ă����܂��B
	a.SetPosition(a.GetPosition() - normal*diff*0.5f);
	b.SetPosition(b.GetPosition() + normal*diff*0.5f);


	//�D-3���ꂼ��̑��x�x�N�g���Ɩ@���x�N�g���̓��ς��Ƃ�܂��B
	Vector2 velA = a.GetVelocity();
	Vector2 velB = b.GetVelocity();
	
	float dotA = Dot(velA,normal);
	float dotB = Dot(velB, -normal);
	//�Ȃ��A�@���x�N�g���͂����ԁ��Ōv�Z����normal�ɓ����Ă����Ԃł��B
	//���ς�Dot�Ƃ����֐��Ōv�Z���܂��B���̌v�Z�œ���ꂽ�l��
	//�@�������̑����̑傫���ɓ�����܂�



	//�E���œ���ꂽ�@�������傫�����炻�ꂼ��̑��x�x�N�g�����v�Z
	//�����Ă����AddVelocity�֐��ł��ꂼ��̃{�[���ɓ`���Ă����܂��B
	b.AddVelocity(normal*dotA);
	a.AddVelocity(-normal*dotA);

	//dotA��A����B�ɑ΂��Ĉړ����鑬�x�x�N�g���̑傫���������Ă��邽��
	//����ɖ@�������x�N�g����normal�������邱�ƂŁAB�ɒD���鑬�x�x�N�g����������܂��B
	//�Ȃ̂ŁAb.AddVelocity�֐��ŁAB�ɑ��x�����Z���Ă����܂��B
	//�܂��Aa.AddVelocity�֐��ŁAB�ɒD��ꂽ���x�Ƀ}�C�i�X�������邱�ƂŎc�����x�N�g����
	//A�̃x�N�g���Ƃ��邱�ƂɂȂ�܂��B���������̏�����B���~�܂��Ă��鎞�ɂ̂ݗL���ł��B

	//�F���݂��ɓ����Ă���ꍇ�́��̏������K�v�ɂȂ�܂��B
	b.AddVelocity(normal*dotB);
	a.AddVelocity(-normal*dotB);


}

//���C���֐�
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
//int main() {
	
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText(_T("�r�����[�h������Ă݂悤"));
	DxLib_Init();
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	const int boardWidth = 550;
	const int boardHeight = 350;

	std::vector<Ball> balls;
	balls.push_back(Ball(0xdddddd,70,200,"self"));//�����{�[��

	balls.push_back(Ball(0xeeee33,300,200));//1�ԃ{�[��(���F)

	//�G�{�[���𑝂₵�Ă݂�
	balls.push_back(Ball(0x4444ff, 319, 188));//2�ԃ{�[��(�F)
	balls.push_back(Ball(0xff4444, 319, 212));//3�ԃ{�[��(�ԐF)
	balls.push_back(Ball(0xff44ff, 338, 176));//4�ԃ{�[��(���F)
	balls.push_back(Ball(0xffaa44, 338, 200));//5�ԃ{�[��(�I�����W�F)
	balls.push_back(Ball(0x00ff00, 338, 224));//6�ԃ{�[��(�ΐF)
	balls.push_back(Ball(0xaa0000, 357, 164));//7�ԃ{�[��(�������F)
	balls.push_back(Ball(0x444444, 357, 188));//8�ԃ{�[��(���F)
	balls.push_back(Ball(0xffaaaa, 357, 212));//9�ԃ{�[��(�s���N�F)
	balls.push_back(Ball(0x88eeee, 357, 236));//10�ԃ{�[��(���F)

	auto cueH=LoadGraph(L"Q.png");
	auto gaugeH = LoadGraph(L"gauge.png");
	auto gaugeRH = LoadGraph(L"gauge_r.png");
	//auto kiminotanH = LoadGraph(L"kiminota-n.png");
	
	_ballCollisionSound = LoadSoundMem(L"billiard-ball1.mp3",6);
	auto _ballInHoleSound = LoadSoundMem(L"billiard-pocket1.mp3");

	auto gameoverH = LoadGraph(L"gameover.png");
	_hitEffectH = LoadGraph(L"hiteffect.png");

	Cue cue(balls[0],cueH);

	bool isGameOver = false;

	bool leftClickTrigger = false;

	int shotCount = 0;

	{
		int powerGauge = 0;
		bool chargingGo = false;
		ScreenShaker shaker;

		while (ProcessMessage() == 0) {
			shaker.PrepareShake();

			ClearDrawScreen();
			//DrawGraph(0, 422, kiminotanH, true);
			if (DxLib::CheckHitKey(KEY_INPUT_R)) {
				for (auto& ball : balls) {
					StopMusic();
					ball.Reset();
					isGameOver = false;
					shotCount = 0;
				}
			}
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) {
				if (!leftClickTrigger) {
					powerGauge = 0;
					chargingGo = true;
				}
				else {
					if (chargingGo) {
						powerGauge = min(powerGauge+4,590);
					}
				}
				leftClickTrigger = true;

			}
			else {
				if (leftClickTrigger&&chargingGo) {
					int mx, my;
					GetMousePoint(&mx, &my);
					Vector2 target(mx, my);
					cue.Shot(target, static_cast<float>(powerGauge)*1.5f);
					++shotCount;
				}
				chargingGo = false;
				leftClickTrigger = false;
			}
			if (DxLib::CheckHitKey(KEY_INPUT_ESCAPE)) {
				if (MessageBox(nullptr, L"�I�����܂����H", L"�����H�����H", MB_OKCANCEL | MB_ICONQUESTION) == IDOK) {
					break;
				}
			}
			//�r�����[�h��`��
			DrawBox(50 - 22, 50 - 22, boardWidth + 50 + 22, boardHeight + 50 + 22, 0x00aa00, true);
			DrawBox(50, 50, boardWidth + 50, boardHeight + 50, 0x000000, false);
			DrawCircle(50, 50, 10, 0x000000);
			DrawCircle(boardWidth + 50, 50, 10, 0x000000);
			DrawCircle(50, boardHeight + 50, 10, 0x000000);
			DrawCircle(boardWidth + 50, boardHeight + 50, 10, 0x000000);
			DrawBox(boardWidth / 2 + 50 - 12, 35, boardWidth / 2 + 50 + 12, 55, 0x000000, true);
			DrawBox(boardWidth / 2 + 50 - 12, boardHeight + 45, boardWidth / 2 + 50 + 12, boardHeight + 65, 0x000000, true);

			const float left_wall = 50;
			const float right_wall = boardWidth + 50;
			const float top_wall = 50;
			const float bottom_wall = boardHeight + 50;

			//�������蔻��
			for (auto& ball : balls) {
				if (!ball.IsAvailable())continue;
				if (IsHit(ball.GetCircle(), Circle(10, Position2(50, 50))) ||//����
					IsHit(ball.GetCircle(), Circle(10, Position2(boardWidth + 50, 50))) ||//�E��
					IsHit(ball.GetCircle(), Circle(10, Position2(50, boardHeight + 50))) ||//����
					IsHit(ball.GetCircle(), Circle(10, Position2(boardWidth + 50, boardHeight + 50))) ||//�E��
					IsHit(ball.GetCircle(), Circle(10, Position2(boardWidth / 2 + 50, 45))) ||
					IsHit(ball.GetCircle(), Circle(10, Position2(boardWidth / 2 + 50, boardHeight + 55)))) {
					if (ball.GetTag() == "self") {
						DxLib::PlayMusic(L"gameover.wav", DX_PLAYTYPE_BACK);
						isGameOver = true;
						shaker.Shake();
					}
					ball.Kill();
					DxLib::PlaySoundMem(_ballInHoleSound, DX_PLAYTYPE_BACK);
				}
			}

			//�ʓ��m�����蔻��(��������)
			for (int i = 0; i < balls.size() - 1; ++i) {
				if (!balls[i].IsAvailable())continue;
				for (int j = i + 1; j < balls.size(); ++j) {
					if (!balls[j].IsAvailable())continue;
					CheckBallCollision(balls[i], balls[j]);
				}
			}

			//�Ǔ�����
			for (auto& ball : balls) {
				if (!ball.IsAvailable())continue;
				if (abs(ball.GetPosition().x - left_wall) <= ball.GetRadius() ||
					abs(ball.GetPosition().x - right_wall) <= ball.GetRadius()) {
					ball.ReverseX();
				}
				if (abs(ball.GetPosition().y - top_wall) <= ball.GetRadius() ||
					abs(ball.GetPosition().y - bottom_wall) <= ball.GetRadius()) {
					ball.ReverseY();
				}
			}

			bool areAllBallsStopped = true;
			for (auto& ball : balls) {
				ball.Update();
				if (areAllBallsStopped&&ball.IsAvailable() && ball.GetVelocity().Magnitude() > 0.1f) {
					areAllBallsStopped = false;
				}
			}
			if (areAllBallsStopped) {
				cue.OnClickable();
			}
			cue.Draw();
			DrawFormatString(50, 8, 0xffffff, L"�ł����񂾉񐔁F%d", shotCount);
			DrawFormatString(300, 8, 0xffffff, L"���Ƃ����ʂ̐��F%d", std::count_if(balls.begin(), balls.end(), [](Ball& b) {return !b.IsAvailable(); }));

			for (auto& h : _hiteffectList) {
				h.Draw();
			}
			auto it = std::remove_if(_hiteffectList.begin(), _hiteffectList.end(), [](HitEffect& he) {return !he.IsAvailable(); });
			_hiteffectList.erase(it, _hiteffectList.end());

			shaker.UpdateShake();

			if (isGameOver) {
				DrawGraph(0, 0, gameoverH, true);
			}

			DrawBox(30, 440, 620, 460, 0xffffff, false);
			DrawBox(31, 441, min(31+powerGauge, 619), 459, 0xffff00, true);
			DrawExtendGraph(31, 441, 619, 459, gaugeRH, false);
			DrawModiGraph(31, 441, min(31 + powerGauge, 619), 441,
				min(31 + powerGauge, 619), 459, 31, 459, gaugeH, false);

			ScreenFlip();
		}

	}
	
	DxLib::DxLib_End();
}