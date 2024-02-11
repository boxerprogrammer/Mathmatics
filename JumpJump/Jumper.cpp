#include "Jumper.h"
#include<DxLib.h>

namespace {
	constexpr int ground_line = 380;
	constexpr float g = 0.5f;
}

void Jumper::Jump()
{
	vel_.y = -15.0f;
}

void Jumper::OnGround()
{
	pos_.y = ground_line;
	vel_.y = 0.0f;
}

Jumper::Jumper(int inx) :Actor(L"GOOD")
{
	pos_.x = inx;
	pos_.y = ground_line;
}

void Jumper::Update()
{
	UpdateKeyState();
	if (IsTriggered(KEY_INPUT_Z)) {
		Jump();
	}
	vel_.y += g;
	pos_ += vel_;
	if (pos_.y >= ground_line) {
		OnGround();
	}
}
