#pragma once

#include"Geometry.h"
class Actor
{
private:
	Position2 pos_;
	float radius_=5.0f;
	bool isChecked_ = false;
public:
	Actor(const Position2& pos);
	const Position2& GetPos()const { return pos_; }
	void Check() { isChecked_ = true; }
	bool IsChecked()const { return isChecked_; }
	void Update();
	void Draw();
};

