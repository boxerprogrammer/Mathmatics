#pragma once
#include<array>

#include<DxLib.h>
#include"Geometry.h"

//ダイスの最大最小の座標
struct BoundingBox {
	Position3 min;
	Position3 max;
};
class Dice
{
private:
	int imgH_;
	std::array<VERTEX3D, 24> vertices_;
	std::array<uint16_t, 36> indices_;
public:
	Dice();
	void Update(const Matrix4x4& mat);
	void Draw() const;
	BoundingBox GetBoundingBox() const;
};

