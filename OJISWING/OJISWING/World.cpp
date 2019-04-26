#include "World.h"
#include "Ojisan.h"
#include<DxLib.h>
#include<cmath>

float g=1.0f;

World::World(Ojisan& Ojisan) : _angle(0),_Ojisan(Ojisan)
{
}


World::~World(void)
{
}


void 
World::Update(){
	char keyBuff[256];
	DxLib::GetHitKeyStateAll(keyBuff);

	
	if(keyBuff[KEY_INPUT_LEFT]){
		_angle-=1.0f;		
		//‚±‚±‚ÉŒX‚«‚ÉŠñ‚é‰Á‘¬‚Ìˆ—‚ð‘‚¢‚Ä‰º‚³‚¢
	}
	if(keyBuff[KEY_INPUT_RIGHT]){
		_angle+=1.0f;
		//‚±‚±‚ÉŒX‚«‚ÉŠñ‚é‰Á‘¬‚Ìˆ—‚ð‘‚¢‚Ä‰º‚³‚¢
	}
}

void 
World::Draw(){
	float radian = getAngle();
	if(_angle>=0.0f){
		DrawLine(0,428,640,428-(640*tan(radian)),0xffffffff,2);
	}else{
		DrawLine(0,
			428+(640*tan(radian)),
			640,
			428,0xffffffff,2);
	}
}

float 
World::getAngle(){
	return  _angle*3.14159265358979/180.0f;
}

float
World::getHeight(float x){
	if(_angle>=0.0f){
		return  x*tan(getAngle());
	}else{
		return  -(640-x)*tan(getAngle());
	}
}