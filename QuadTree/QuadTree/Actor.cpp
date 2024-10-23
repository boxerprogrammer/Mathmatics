#include "Actor.h"
#include<DxLib.h>

Actor::Actor(const Position2& pos):pos_(pos)
{
}

void Actor::Update()
{
}

void Actor::Draw()
{
	DrawCircleAA(pos_.x, pos_.y, radius_, 32, 0xaaaaff,false,2.0f);
}
