#pragma once
#include"Geometry.h"
/// <summary>
/// ゲーム中に配置できる基底クラス
/// </summary>
class Actor
{
protected:
	int handle_;
	int frame_;
	int totalFrame_;
	Position2 pos_;
public:
	Actor();
	virtual void Update()=0;
	void Draw();

};

