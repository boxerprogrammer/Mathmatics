#pragma once
#include"Geometry.h"
#include<array>
/// <summary>
/// ゲーム中に配置できる基底クラス
/// </summary>
class Actor
{
protected:
	int handle_=0;
	int frame_=0;
	int totalFrame_=0;
	Position2 pos_ = {};
	std::array<char, 256> keyState_;
	std::array<char, 256> lastKeyState_;
	void UpdateKeyState();
	bool IsTriggered(int idx)const;
public:
	Actor();
	virtual void Update()=0;
	void Draw();

};

