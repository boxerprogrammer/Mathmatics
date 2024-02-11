#include "BadJumper.h"
#include<DxLib.h>

namespace {
	constexpr int ground_line = 380;
	constexpr float g = 0.5f;
}

void BadJumper::Jump()
{
	pos_.y = ground_line - 200;
}

void BadJumper::OnGround()
{
	pos_.y = ground_line;
	vel_.y = 0.0f;
}

BadJumper::BadJumper(int inx) :Actor(L" BAD")
{
	pos_.x = inx;
	pos_.y = ground_line;
}

void BadJumper::Update()
{
	UpdateKeyState();
	if (IsTriggered(KEY_INPUT_Z)) {
		Jump();
	}
	pos_ += vel_;
	if (pos_.y >= ground_line) {
		OnGround();
	}
	else {
		vel_.y = 5.0f;
	}
}
