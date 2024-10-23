#include "IDivider.h"
#include"QuadDivider.h"

std::shared_ptr<IDivider>
IDivider::AddQuadDivider(const Position2& center)
{
	children_.push_back(std::make_shared<QuadDivider>(this,center));
	return children_.back();
}

const Size& IDivider::GetSize() const
{
	return size_;
}
