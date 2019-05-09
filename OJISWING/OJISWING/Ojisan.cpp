#include "Ojisan.h"
#include<DxLib.h>
#include"ScreenShaker.h"
const int GROUND_LINE=428;

Ojisan::Ojisan(const char* filename,int xnum,int ynum,ScreenShaker& shaker) : Character(filename,xnum,ynum),_shaker(shaker),
	_vx(0),_vy(0),_ax(0),_ay(0),_isPushJump(false),_isJumped(false)
{
	_phase=&Ojisan::FallingPhase;
	//�Ђ��̎x�_���`����
	_endPoint.x=320;
	_endPoint.y=0;
	_scroll.x=0;
	_scroll.y=0;
	_v=0;
	OnInit(_vx,_vy);
	_explosionSE = LoadSoundMem("sound/explosion.mp3");
	_explosionH = LoadGraph("img/explosion.png");
	_g=0.8f;//�d�͂̒�`
	_length=320;//�R�̒����̌v�Z
	_handle=LoadGraph("img/ojisan.png");
}

void 
Ojisan::OnActivateTension(){
	_phase=&Ojisan::SwingPhase;
}

void 
Ojisan::FallingPhase(){
	char keyBuff[256];
	DxLib::GetHitKeyStateAll(keyBuff);
	if(keyBuff[KEY_INPUT_SPACE]){
		if(!_isPushJump){
			OnActivateTension();
		}
		_isPushJump=true;
	}else{
		_isPushJump=false;
	}
	OnMove(_pos.x,_pos.y,_vx,_vy);
	_vy+=_g;
	DrawLine(_pos.x,_pos.y,_endPoint.x,0,0xffffffff,2);
	DrawRotaGraph(_pos.x,_pos.y,1.0,0,_handle,true,false);
	SetStartPosition(_pos.x,_pos.y);
}

//�U��q�t�F�[�Y
void 
Ojisan::SwingPhase(){
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

	Vec2 v=(_pos-_endPoint);
	
	v=v.normalized();
	
	_theta =atan2f(Cross(v,Vec2(-1,0)),Dot(v,Vec2(-1,0)));
	float cost = Dot(v,Vec2(-1,0));
	float sint = Cross(v,Vec2(-1,0));

	float a=_g*(cost);
	_v+=a;

	if(_isJumped){
		OnMove(_pos.x,_pos.y,_vx,_vy);
		_vy+=_g;
		
	}else{
		OnMove(_pos.x,_pos.y,_v*sint,_v*cost);
		OnAdjust();
	}
	

	
	if(!_isJumped){
		DrawLine(_pos.x - _scroll.x,_pos.y,320 - _scroll.x,0,0xffffffff,2);
	}
	DrawRotaGraph(_pos.x - _scroll.x,_pos.y,1.0,-(_theta -3.14159265358979/2),_handle,true,false);
}
void 
Ojisan::FlyingPhase(){
	Vec2 v=(_pos-_endPoint);
	v=v.normalized();
	_theta =atan2f(Cross(v,Vec2(-1,0)),Dot(v,Vec2(-1,0)));
	if(_pos.y>=GROUND_LINE){///�n�ʂɓ���������c
		OnGround(_pos.x,_pos.y,_vx,_vy);
		_shaker.Shake();
		_phase = &Ojisan::GroundPhase;
		PlaySoundMem(_explosionSE, DX_PLAYTYPE_BACK);
	}	
	char keyBuff[256];
	DxLib::GetHitKeyStateAll(keyBuff);
	if(keyBuff[KEY_INPUT_SPACE]){
		if(!_isPushJump){
			ResetOjisan(_endPoint.x,_endPoint.y);
			_scroll.x=0;
			_scroll.y=0;
		}
		_isPushJump=true;
	}else{
		_isPushJump=false;
	}
	OnMove(_pos.x,_pos.y,_vx,_vy);
	_vy+=_g;
	DrawRotaGraph(_pos.x-_scroll.x,_pos.y-_scroll.y,1.0,-(_theta -3.14159265358979/2),_handle,true,false);
}


void
Ojisan::GroundPhase() {
	char keyBuff[256];
	DxLib::GetHitKeyStateAll(keyBuff);
	if (keyBuff[KEY_INPUT_SPACE]) {
		if (!_isPushJump) {
			ResetOjisan(_endPoint.x, _endPoint.y);
			_scroll.x = 0;
			_scroll.y = 0;
		}
		_isPushJump = true;
	}
	else {
		_isPushJump = false;
	}
	DrawRotaGraph(_pos.x - _scroll.x, _pos.y - _scroll.y, 1.0, -(_theta - 3.14159265358979 / 2), _handle, true, false);
	DxLib::DrawRectRotaGraph2(_pos.x - _scroll.x, _pos.y - _scroll.y, (_frame / 2) * 256, 0, 256, 256, 128, 220, 2.0f, 0.0, _explosionH, true);
	++_frame;
}

void
Ojisan::SetEndPoint(float x,float y){
	_endPoint.x=x;
	_endPoint.y=y;
	Vec2 v = _pos-_endPoint;
	_length=v.Length();
}

void 
Ojisan::SetStartPosition(float x, float y){
	Character::SetPosition(x,y);
	Vec2 v = _pos-_endPoint;
	_length=v.Length();
	_v = 0;
}

void 
Ojisan::ResetOjisan(float x, float y){
	_isJumped=false;
	SetStartPosition(x,y);
	_phase=&Ojisan::FallingPhase;
	_frame = 0;
}


Ojisan::~Ojisan(void)
{
}

void
Ojisan::SetAngle(float yrate){
	_vy=_vx*yrate;
}

void
Ojisan::SetAccel(float ax,float ay){
	_ax=ax;
	_ay=ay;
}

void 
Ojisan::Update(){
	(this->*_phase)();
}

void Ojisan::OnAdjust(){
	Vec2 v=(_pos-_endPoint);
	if(v.Length() > _length){
		_pos = _endPoint + v.normalized()*_length;
	}
}

void //�ړ��C�x���g
Ojisan::OnMove(float& x, float& y,float vx,float vy){
	//�@�����Ɉړ��̂��߂̏�����`���Ă��������B
	x+=vx;
	y+=vy;
}

void //�L�[�������Ă��Ȃ����̉����C�x���g
Ojisan::OnAccel(float& vx,float& vy,const float& ax, const float& ay){
	//�A�����ɉ����̂��߂̏����������Ă��������B0.5���炢
	vx+=ax;
	vy+=ay;
}


void //�������C�x���g
Ojisan::OnInit(float& vx,float& vy){
	//�B�����ɏ������̏����������Ă��������B
}

void //���n�C�x���g
Ojisan::OnGround(float& x, float& y,float& vx,float& vy){
	//�C�����ɒ��n���̏����������Ă�������
	y=GROUND_LINE;
	vy=0;
	vx=0;
}

void //�W�����v�L�[�C�x���g
Ojisan::OnPushJumpKey(float& vx, float& vy){

	//�D�����ɃW�����v�L�[�������ꂽ���̏����������Ă��������B-10.0���炢
	Vec2 v=(_pos-_endPoint);
	v=v.normalized();
	_isJumped=true;
	float theta=atan2f(Cross(v,Vec2(-1,0)),Dot(v,Vec2(-1,0)));
	float cost = Dot(v,Vec2(-1,0));
	float sint = Cross(v,Vec2(-1,0));
	_v *= 1.6f;
	vx=_v*sint;
	vy=_v*cost;

	_phase=&Ojisan::FlyingPhase;



}
bool 
Ojisan::IsFlying()const {
	return _isJumped;
}

void //���L�[�����ꂽ�C�x���g
Ojisan::OnPushLeftKey(float& vx,float& vy){
	//�E�����ɍ��L�[�������ꂽ���̏����������Ă�������
	_v-=0.5f;
}


void //�E�L�[�����ꂽ�C�x���g
Ojisan::OnPushRightKey(float& vx, float &vy){
	//�F�����ɉE�L�[�������ꂽ���̏����������Ă�������
	_v+=0.5f;
}
