#include "Character.h"
#include<DxLib.h>

Character::Character(void)
{
}

Character::Character(const char* fileName,int divnumx,int divnumy):_divnum(divnumx*divnumy),
_animCount(0),
_currentIndex(0),
_angle(0.f)
{
	_pos.x=0;
	_pos.y=0;
	int tmpH=LoadGraph(fileName);
	DxLib::GetGraphSize(tmpH,&_graphSize.w,&_graphSize.h);
	_unitsize.w = _graphSize.w/divnumx;
	_unitsize.h = _graphSize.w/divnumy;
	DeleteGraph(tmpH);
	_handles.resize(_divnum);
	LoadDivGraph(fileName,_divnum,divnumx,divnumy,_unitsize.w,_unitsize.h,&_handles[0]);
}


Character::~Character(void)
{
}

void
Character::AddAnimInfo(int animNo,int wait,int nextIndex){
	AnimInfo info;
	info.animNo=animNo;
	info.wait=wait;
	info.nextIndex=nextIndex;
	_animData.push_back(info);
}

void
Character::Draw(){
	if(_animData.empty()){
		return;
	}
	DrawRotaGraph2(_pos.x,_pos.y,_unitsize.w/2,_unitsize.h/2,1.0,_angle,_handles[_animData[_currentIndex].animNo],true);
	if( ++_animCount>=_animData[_currentIndex].wait ){
		int nextIndex = _animData[_currentIndex].nextIndex;
		_animCount=0;
		_currentIndex = nextIndex==-1 ? _currentIndex : nextIndex;
	}
}


void 
Character::SetPosition(float x,float y){
	_pos.x=x;
	_pos.y=y;
}