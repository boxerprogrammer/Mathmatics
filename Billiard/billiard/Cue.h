#pragma once
#include"Ball.h"

///キュー(ビリヤードの球を突くアレ)クラス
class Cue
{
public:
	Cue(Ball& playerball,int imgH);
	~Cue();

	//描画する
	void Draw();


	//キューで球を撃つ
	void Shot(Position2& tagetpos,float power=1.0f);

	void OnClickable();
private:
	Ball& _playerball;
	int _imgH = 0;
	bool _isClickable = true;
	bool _isShotting = false;
	int _countdowntimer = 0;
	Vector2 _force;
	Position2 _staypos;
	Size _imgSize;
	int _soundH;

	//プレイヤーボールに力(という名の速度ベクトル)を加える
	void AddForce();
};

