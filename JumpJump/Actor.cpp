#include "Actor.h"
#include<DxLib.h>
#include<array>
#include<algorithm>
#include<numeric>
namespace {
	std::array<int, 3> frames_ = { 10,5,10 };
	constexpr int w = 40;
	constexpr int h = 32;
}

Actor::Actor()
{
	handle_ = DxLib::LoadGraph(L"img/character.png");
	totalFrame_= std::accumulate(frames_.begin(), frames_.end(),0);
}

void Actor::Draw()
{
	int idx = 0;
	int frame = frame_;
	for (; idx < frames_.size(); ++idx) {
		if (frame <= frames_[idx]) {
			break;
		}
		frame -= frames_[idx];
	}
	DrawRotaGraph2(pos_.x, pos_.y, w / 2, h, 1.0f, 0.0f, handle_, true);
	frame_ = (frame_ + 1) % totalFrame_;
}
