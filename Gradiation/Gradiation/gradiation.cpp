#include<DxLib.h>
#include<cmath>
#include"Geometry.h"

//�ꕔ�́u������߂Ă�K�N�Z�C����v�ɍ���
//�l���邱�Ƃ��������ȁB�o�J�ł��A�z�ł���������l���邱�Ƃ��������ȁB
//�l���邱�Ƃ��������悤�Ȑl�͂��A��������]�݂��͂���Ȃ��Ȃ��H
//�]���r�ɂł��H�킹�Ă������炢�����ł����H

//�萔
constexpr float g_ = 0.3f;//�d�͉����x(1�t���[���ɉ��s�N�Z����������)
constexpr float shadow_offset = 3.0f;//�e�I�t�Z�b�g
constexpr float line_base = 460.0f;//���̃x�[�X���C��
constexpr unsigned short circle_edge_num = 32;//�~�̕ӂ̐�


///�{�[���������ƐG�ꂽ�������͐�����艺�ɗ�����true��Ԃ�
///@param seg ����
///@param circle �~
///@remarks �����̒��������߂�Ⴂ���Ǝv���̂ŊO�ώg���Ă��\���܂���
///�֐�cross�͎������Ă���̂Łc�g�����炢�����H
bool IsHit(const Segment& seg, const Circle& circle) {
	auto v = seg.posb - seg.posa;
	v.Normalize();
	auto w = circle.pos - seg.posa;
	return fabsf(Cross(v,w))<=circle.radius;
}

///���݂̐������Ǝw���x���W����������x���W�ł�y���W��Ԃ�
///@param seg �������
///@param x X���W
///@return Y���W
float GetYFromSegmentAndX(const Segment& seg, const float x) {
	//�u��ԁv�������́u�X����y=ax+b�v�ɂ��čl����
	auto a = (seg.posb.y - seg.posa.y)/(seg.posb.x - seg.posa.x);
	return a*x+seg.posa.y;
}

///�������瓾��ꂽY���W����{�[��(�~)�̕␳���W���v�Z���Ԃ�
///@param c �{�[��(�~)�I�u�W�F�N�g
///@param seg �����I�u�W�F�N�g
///@return ����ׂ��~�̒��S���W
Position2 GetAdjustedCirclePosition(const Circle& c, const Segment& seg) {
	//�u��ԁv�������́u�X����y=ax+b�v�ɂ��čl����
	auto y = GetYFromSegmentAndX(seg, c.pos.x) - c.radius;
	return Position2(c.pos.x, y);
}

///�����̌X��������{�[���̉����x���v�Z����
///@param seg �����I�u�W�F�N�g
///@return ���݂̐����̌X���ɂ�����{�[���ɑ΂�������x
///@remarks posA�����AposB���E�Ƃ��܂��B
float CalculateAccelerationForBall(const Segment& seg) {
	//�@posA��posB�ւ̃x�N�g�������
	//�A-90���Ŗ@���x�N�g���ɂ���
	//�B���K������
	//�C�B�ŏo�����p�x�Əd�͉����x_g�������x���v�Z����
	return 0.0f;
}

///�����̌X��������{�[���̉����x���x�N�g�����v�Z����
///@param seg �����I�u�W�F�N�g
///@return ���݂̐����̌X���ɂ�����{�[���ɑ΂�������x�x�N�g��
///@remarks posA�����AposB���E�Ƃ��܂��B
Vector2 CalculateAccelerationVectorForBall(const Segment& seg) {
	//�@posA��posB�ւ̃x�N�g�������
	auto N = seg.posb - seg.posa;
	//�A-90���Ŗ@���x�N�g���ɂ���
	N = { -N.y,N.x };
	//�B���K������
	N.Normalize();
	//�C�B�ŏo�����p�x�Əd�͉����x_g�������x�x�N�g�����v�Z����
	Vector2 G = { 0.0f, g_ };

	return G+N*Dot(-N,G);
}

///���x�Ɖ����x����V�������x�𓾂�
float GetNextVelocity(const float velocity,const float acceleration) {
	//�ȒP������̂Ńm�[�q���g
	return velocity+acceleration;
}

///���x�Ɗp�x����X������Y�����̑��x�ɕ�����
///Vector2�Ƃ��ĕԂ�
Vector2 GetVelocityVector(const float velocity, const float angle) {
	//�ǂ�����cos�łǂ�����sin���H
	return Vector2(0, 0);
}



///�ڂ������e��\��
void DrawBlurShadow(int rendertarget,const Segment& seg,const Circle& c) {
	SetDrawScreen(rendertarget);
	ClearDrawScreen();
	
	int screen_width, screen_height;
	GetWindowSize(&screen_width, &screen_height);

	DrawBox(0, 0, screen_width, screen_height, 0xffffff, true);
	DrawCircleAA(c.pos.x + shadow_offset, c.pos.y + shadow_offset, c.radius, circle_edge_num, 0x444488, false, 3);
	DrawLineAA(seg.posa.x, seg.posa.y + shadow_offset, seg.posb.x, seg.posb.y + shadow_offset, 0x444488, 3);
	GraphFilter(rendertarget, DX_GRAPH_FILTER_GAUSS, 4, 800);
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, rendertarget, false);

}

void DrawBall(const Circle& c) {
	DrawCircleAA(c.pos.x + shadow_offset, c.pos.y + shadow_offset, c.radius, circle_edge_num, 0xaaaaaa, false, 2);
	DrawCircleAA(c.pos.x, c.pos.y, c.radius, circle_edge_num, 0x000000, false, 2);
}

void DrawFloor(const Segment& seg) {

	DrawLineAA(seg.posa.x, seg.posa.y+ shadow_offset, seg.posb.x, seg.posb.y+shadow_offset, 0xaaaaaa, 2);
	DrawLineAA(seg.posa.x, seg.posa.y , seg.posb.x, seg.posb.y , 0x000000, 2);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	int screen_width, screen_height;
	GetWindowSize(&screen_width, &screen_height);


	Segment seg(0,line_base, screen_width,line_base);
	Circle c(30, Position2(100, 100));
	
	
	char keystate[256];

	auto rt=MakeScreen(screen_width, screen_height);

	Vector2 v_ = Vector2(0,g_);
	while (ProcessMessage() == 0) {
		GetHitKeyStateAll(keystate);

		//������(�㉺�ړ��̂�)
		if (keystate[KEY_INPUT_UP]) {
			if (seg.posb.y > line_base) {
				seg.posb.y += 1.0f;
			}
			else {
				seg.posa.y -= 1.0f;
			}
		}
		if (keystate[KEY_INPUT_DOWN]) {
			if (seg.posa.y < line_base) {
				seg.posa.y += 1.0f;
			}
			else {
				seg.posb.y -= 1.0f;
			}
		}
		if (keystate[KEY_INPUT_R]) {//�������W�ɖ߂�
			c.pos = { 100,100 };
		}


		//�`��
		DrawBlurShadow(rt, seg, c);
		DrawBall(c);
		DrawFloor(seg);

		//��ʒ[�ȏ�ɂ͍s���Ȃ��悤��
		if (seg.posa.x > c.pos.x || seg.posb.x < c.pos.x) {
			v_ = Vector2(0, 0);
		}

		//�{�[���̓������x�����^�����L�q����
		c.pos += v_;

		//���ɓ������ĂȂ�������d�͂ɂ�铙���������^��
		//���������̂܂܂ł͐^���ɗ����邾���Ȃ̂�
		//���ɓ���������A�⓹�ɉ����ĉ���悤�ɂ��Ă�������
		if (IsHit(seg,c)) {
			//���������ꍇ�͏��ɉ����Đi�߂Ă��������B
			//��̂ق��Ɋ֐�������̂ŁA������g���Ă��������B
			c.pos = GetAdjustedCirclePosition(c, seg);
			auto a = CalculateAccelerationVectorForBall(seg);
			v_ += a;
		}
		else {
			v_.y += g_;
		}
		
		ScreenFlip();
	}

	DxLib_End();
}
