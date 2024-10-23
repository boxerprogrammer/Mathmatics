#pragma once
#include"Geometry.h"
#include<vector>
#include<memory>
class QuadDivider;
class IDivider
{
protected:
	Size size_;
	std::vector<std::shared_ptr<IDivider>> children_;
public:
	std::shared_ptr<IDivider> AddQuadDivider(const Position2& center);
	const Size& GetSize()const;
	virtual float GetLeft()const = 0;
	virtual float GetTop()const = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

