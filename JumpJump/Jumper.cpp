#include "Jumper.h"

namespace {
	constexpr int ground_line = 380;
}

Jumper::Jumper() :Actor()
{
	pos_.x = 500;
	pos_.y = ground_line;
}

void Jumper::Update()
{
}
