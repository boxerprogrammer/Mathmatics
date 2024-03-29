#include "Actor.h"
#include<DxLib.h>
#include<array>
#include<algorithm>
#include<numeric>
namespace {
	std::array<int, 3> frames_ = { 10,5,10 };
	constexpr int w = 40;
	constexpr int h = 32;
	constexpr int offset_str_x = -18;
}

void Actor::UpdateKeyState()
{
	lastKeyState_ = keyState_;
	GetHitKeyStateAll(keyState_.data());
}

bool Actor::IsTriggered(int idx) const
{
	return keyState_[idx]&&!lastKeyState_[idx];
}

Actor::Actor(const wchar_t* label):label_(label)
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
	
	DrawRectRotaGraph2(pos_.x, pos_.y, idx*w,0,w,h, w / 2, h, 1.0f, 0.0f, handle_, true);
	DrawFormatString(offset_str_x+pos_.x+2, pos_.y+2, 0x000000, L"%s", label_.c_str());
	DrawFormatString(offset_str_x+pos_.x, pos_.y,0xffffff, L"%s", label_.c_str());
	frame_ = (frame_ + 1) % totalFrame_;
}
