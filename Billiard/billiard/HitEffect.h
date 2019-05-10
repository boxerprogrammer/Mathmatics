#pragma once

#include"Geometry.h"

class HitEffect
{
private:
	int _handle;
	int _frame;
	Position2 _pos;
	float _scale;
public:
	HitEffect(int handle,Position2 pos,float scale=1.0f);
	~HitEffect();
	void Draw();
	bool IsAvailable();
};

