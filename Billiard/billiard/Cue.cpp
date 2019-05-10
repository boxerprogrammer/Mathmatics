#include "Cue.h"
#include<DxLib.h>


Cue::Cue(Ball& ball,int inimgH):_playerball(ball),
_isClickable(true),
_countdowntimer(0),
_imgH(inimgH)
{
	int w, h;
	DxLib::GetGraphSize(inimgH, &w, &h);
	_imgSize.w = w;
	_imgSize.h = h;
	_staypos = ball.GetPosition();
	_soundH = DxLib::LoadSoundMem(L"billiard-shot1.mp3");
}


Cue::~Cue()
{
}


//描画する
void 
Cue::Draw() {
	Position2 bpos = _playerball.GetPosition();
	
	Vector2 offset=Vector2(0,0);
	Vector2 fvec = _force.Normalized();
	float angle = atan2(fvec.y, fvec.x);
	if (_countdowntimer > 10) {
		offset.x = 60*(-(float)(60-_countdowntimer) / 50.0f)*fvec.x;
		offset.y = 60 * (-(float)(60 - _countdowntimer) / 50.0f)*fvec.y;
	}
	else if(_countdowntimer>1){
		offset.x = 60*(-(float)_countdowntimer / 10.0f)*fvec.x;
		offset.y = 60 * (-(float)(_countdowntimer) / 10.0f)*fvec.y;
	}
	else if(_countdowntimer==1){
		AddForce();
		_staypos = bpos;
		_isShotting = false;
	}
	if (_isClickable) {
		int mx, my;
		GetMousePoint(&mx, &my);
		float angle = atan2(my-bpos.y, mx-bpos.x);
		DxLib::DrawRotaGraph2(bpos.x, bpos.y, _imgSize.w, _imgSize.h / 2, 1.0f, angle, _imgH, true);
	}
	else {
		if (_countdowntimer >= 0) {
			DxLib::DrawRotaGraph2(bpos.x + offset.x, bpos.y + offset.y, _imgSize.w, _imgSize.h / 2, 1.0f, angle, _imgH, true);
		}
		else {
			DxLib::DrawRotaGraph2(_staypos.x, _staypos.y, _imgSize.w, _imgSize.h / 2, 1.0f, angle, _imgH, true);
		}
	}
	--_countdowntimer;// = max(_countdowntimer - 1, 0);
}

//プレイヤーボールに力(という名の速度ベクトル)を加える
void 
Cue::AddForce() {
	_playerball.AddVelocity(_force*0.02f);
	DxLib::PlaySoundMem(_soundH, DX_PLAYTYPE_BACK);
}

//キューで球を撃つ
void 
Cue::Shot(Position2& tagetpos,float power) {
	_force=tagetpos - _playerball.GetPosition();
	_force.Normalize();
	_force *= power;
	_isClickable = false;
	_isShotting = true;
	_countdowntimer = 60;
}

void
Cue::OnClickable() {
	if (_isShotting)return;
	_isClickable = true;
}