#include "QuadDivider.h"
#include<DxLib.h>

QuadDivider::QuadDivider(IDivider* parent, const Position2& center):parent_(parent),center_(center)
{
	auto left = parent_->GetLeft();
	auto top = parent_->GetTop();
	auto& size = parent_->GetSize();
	size_ = { size.w / 2,size.h / 2 };
	auto right = parent_->GetLeft() + static_cast<float>(size.w);
	auto bottom = parent_->GetTop() + static_cast<float>(size.h);
	//center_.x = (left + right) / 2.0f;
	//center_.y = (top + bottom) / 2.0f;
	
}

float QuadDivider::GetLeft() const
{
	return center_.x-static_cast<float>(size_.w);
}

float QuadDivider::GetTop() const
{
	return center_.y - static_cast<float>(size_.h);
}

void QuadDivider::Update()
{
	for (auto& child : children_) {
		child->Update();
	}
}

void QuadDivider::Draw()
{
	auto& size = parent_->GetSize();
	auto left = center_.x - size.w / 2;//parent_->GetLeft();
	auto top = center_.y - size.h / 2;//parent_->GetTop();
	
	auto right = center_.x + size_.w;//parent_->GetLeft() + static_cast<float>(size.w);
	auto bottom = center_.y + size_.h;//parent_->GetTop() + static_cast<float>(size.h);
	//‰¡ü
	DrawLineAA(left, center_.y,
		right, center_.y, 0xffffff);
	//cü
	DrawLineAA(center_.x, top,
		center_.x, bottom, 0xffffff);

	for (auto& child : children_) {
		child->Draw();
	}

}
