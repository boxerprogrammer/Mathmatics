#include "Monkey.h"
#include<DxLib.h>

const int GROUND_LINE=428;

Monkey::Monkey(const char* filename,int xnum,int ynum) : Character(filename,xnum,ynum),
	_vx(0),_vy(0),_isPushJump(false)
{
	OnInit(_vx,_vy);	
}


Monkey::~Monkey(void)
{
}

void 
Monkey::Update(){
	if(_pos.y>=GROUND_LINE){///�n�ʂɓ���������c
		OnGround(_pos.x,_pos.y,_vx,_vy);
	}
	char keyBuff[256];
	DxLib::GetHitKeyStateAll(keyBuff);
	if(keyBuff[KEY_INPUT_SPACE]){
		if(!_isPushJump){
			OnPushJumpKey(_vx,_vy);
		}
		_isPushJump=true;
	}else{
		_isPushJump=false;
	}
	if(keyBuff[KEY_INPUT_LEFT]){
		OnPushLeftKey(_vx,_vy);
	}
	if(keyBuff[KEY_INPUT_RIGHT]){
		OnPushRightKey(_vx,_vy);
	}

	OnAccel(_vx,_vy);
	OnMove(_pos.x,_pos.y,_vx,_vy);
	Draw();
}



void //�ړ��C�x���g
Monkey::OnMove(int& x, int& y,float vx,float vy){
	//�@�����Ɉړ��̂��߂̏�����`���Ă��������B
}

void //�L�[�������Ă��Ȃ����̉����C�x���g
Monkey::OnAccel(float& vx,float& vy){
	//�A�����ɉ����̂��߂̏����������Ă��������B0.5���炢
}

void //�������C�x���g
Monkey::OnInit(float& vx,float& vy){
	//�B�����ɏ������̏����������Ă��������B
}

void //���n�C�x���g
Monkey::OnGround(int& x, int& y,float& vx,float& vy){
	//�C�����ɒ��n���̏����������Ă�������
}

void //�W�����v�L�[�C�x���g
Monkey::OnPushJumpKey(float& vx, float& vy){
	//�D�����ɃW�����v�L�[�������ꂽ���̏����������Ă��������B-10.0���炢
}

void //���L�[�����ꂽ�C�x���g
Monkey::OnPushLeftKey(float& vx,float& vy){
	//�E�����ɍ��L�[�������ꂽ���̏����������Ă�������
}


void //�E�L�[�����ꂽ�C�x���g
Monkey::OnPushRightKey(float& vx, float &vy){
	//�F�����ɉE�L�[�������ꂽ���̏����������Ă�������
}
