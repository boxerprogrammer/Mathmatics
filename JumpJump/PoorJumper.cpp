#include "PoorJumper.h"
#include<DxLib.h>

namespace {
	constexpr int ground_line = 380;
}

void PoorJumper::Jump()
{
	vel_.y = -5.0f;
}

void PoorJumper::OnGround()
{
	pos_.y = ground_line;
	vel_.y = 0.0f;
}

PoorJumper::PoorJumper(int inx) :Actor(L"POOR")
{
	pos_.x = inx;
	pos_.y = ground_line;
}

void PoorJumper::Update()
{
	UpdateKeyState();
	if (IsTriggered(KEY_INPUT_Z)) {
		Jump();
	}
	pos_ += vel_;
	if (pos_.y >= ground_line) {
		OnGround();
	}
	else if (vel_.y<0.0f &&  pos_.y < ground_line - 200) {
		vel_.y = 5.0f;
	}
}
