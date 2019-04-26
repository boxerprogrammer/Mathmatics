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
	if(_pos.y>=GROUND_LINE){///地面に当たったら…
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



void //移動イベント
Monkey::OnMove(int& x, int& y,float vx,float vy){
	//①ここに移動のための処理を描いてください。
}

void //キーを押していない時の加速イベント
Monkey::OnAccel(float& vx,float& vy){
	//②ここに加速のための処理を書いてください。0.5くらい
}

void //初期化イベント
Monkey::OnInit(float& vx,float& vy){
	//③ここに初期化の処理を書いてください。
}

void //着地イベント
Monkey::OnGround(int& x, int& y,float& vx,float& vy){
	//④ここに着地時の処理を書いてください
}

void //ジャンプキーイベント
Monkey::OnPushJumpKey(float& vx, float& vy){
	//⑤ここにジャンプキーが押された時の処理を書いてください。-10.0くらい
}

void //左キー押されたイベント
Monkey::OnPushLeftKey(float& vx,float& vy){
	//⑥ここに左キーを押された時の処理を書いてください
}


void //右キー押されたイベント
Monkey::OnPushRightKey(float& vx, float &vy){
	//⑦ここに右キーを押された時の処理を書いてください
}
