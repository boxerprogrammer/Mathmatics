#pragma once
#include"IDivider.h"
/// <summary>
/// 四分木構造の可視化
/// </summary>
class QuadDivider : public IDivider
{
private:
	Position2 center_;
	IDivider* parent_;
public:
	QuadDivider(IDivider* parent,const Position2& center);
	virtual float GetLeft()const override;
	virtual float GetTop()const override;
	virtual void Update()override;
	virtual void Draw()override;

};

