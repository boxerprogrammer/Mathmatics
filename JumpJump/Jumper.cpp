#include "Jumper.h"
#include<DxLib.h>

namespace {
	constexpr int ground_line = 380;
}

void Jumper::Jump()
{
}

Jumper::Jumper() :Actor()
{
	pos_.x = 500;
	pos_.y = ground_line;
}

void Jumper::Update()
{
	UpdateKeyState();
	if (IsTriggered(KEY_INPUT_Z)) {
		Jump();
	}

}
