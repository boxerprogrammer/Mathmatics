#include "NullDivider.h"

NullDivider::NullDivider()
{
	size_ = { 640, 480 };
}

float NullDivider::GetLeft() const
{
	return 0.0f;
}

float NullDivider::GetTop() const
{
	return 0.0f;
}

void NullDivider::Update()
{
	for (auto& child : children_) {
		child->Update();
	}
}

void NullDivider::Draw()
{
	for (auto& child : children_) {
		child->Draw();
	}
}

