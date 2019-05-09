#include<DxLib.h>
#include<cassert>
#include "ScreenShaker.h"



ScreenShaker::ScreenShaker():_shakeScale(0)
{
	assert(DxLib::DxLib_IsInit());
	int w, h,bit;
	GetScreenState(&w, &h,&bit);
	_screenH = MakeScreen(w, h);
	
}


ScreenShaker::~ScreenShaker()
{
}

//—h‚êƒgƒŠƒK[
void 
ScreenShaker::Shake() {
	_shakeScale = 10.0f;
}

//‰æ–Ê—h‚ê€”õ()
void 
ScreenShaker::PrepareShake() {
	SetDrawScreen(_screenH);
}

//‰æ–Ê—h‚êXV
void
ScreenShaker::UpdateShake() {
	SetDrawScreen(DX_SCREEN_BACK);
	//ClearDrawScreen();
	auto sx = DxLib::GetRand(6) - 3;
	auto sy = DxLib::GetRand(6) - 3;
	DrawGraph(sx*_shakeScale, sy*_shakeScale, _screenH, true);
	_shakeScale *= 0.95f;
}
