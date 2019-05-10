#include "HitEffect.h"
#include<DxLib.h>


HitEffect::HitEffect(int handle,Position2 pos,float scale):_handle(handle),_pos(pos),_frame(0),_scale(scale)
{
}


HitEffect::~HitEffect()
{
}


void HitEffect::Draw() {
	int w, h;
	GetGraphSize(_handle, &w, &h);
	w = w / 15;
	DrawRectRotaGraph2(_pos.x,_pos.y, (_frame / 2) * 256, 0,256,256,w/2,h/2,_scale,0.0f,_handle,true,false);// _pos.x - w / 2, _pos.y - h / 2, (_frame / 2) * 256, 0, 256, 256, _handle, true, false);
	++_frame;
}


bool
HitEffect::IsAvailable() {
	return _frame <= 30;
}
