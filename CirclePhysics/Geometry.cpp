#include"Geometry.h"
#include<DxLib.h>
#include<cmath>


void
Rect::Draw(uint32_t color) {
	DxLib::DrawBox(Left()*2, Top()*2, Right()*2, Bottom()*2, color, false);
}

void
Rect::Draw(Vector2& offset, uint32_t color) {
	DxLib::DrawBox((Left()+offset.x)*2, (Top()+offset.y)*2, (Right()+offset.x)*2, (Bottom()+offset.y)*2, color, false);
}

void
Vector2::operator*=(float scale) {
	x *= scale;
	y *= scale;
}

Vector2
Vector2::operator*(float scale) {
	return Vector2(x*scale, y*scale);
}

Vector2 operator+(const Vector2& va, const Vector2 vb) {
	return Vector2(va.x + vb.x, va.y + vb.y);
}

Vector2 operator-(const Vector2& va, const Vector2 vb){
	return Vector2(va.x - vb.x, va.y - vb.y);
}

float
Vector2::Magnitude()const {
	return hypot(x, y);
}


void 
Vector2::Normalize() {
	float mag = Magnitude();
	if (mag == 0) {
		return;
	}
	x /= mag;
	y /= mag;
}


Vector2
Vector2::Normalized() {
	float mag = Magnitude();
	if (mag == 0) {
		return Vector2(0, 0);
	}
	return Vector2(x / mag,	y /mag);
}


///内積を返す
float
Dot(const Vector2& va, const Vector2& vb) {
	return va.x*vb.x + va.y*vb.y;
}

///外積を返す
float
Cross(const Vector2& va, const Vector2& vb) {
	return va.x*vb.y - vb.x*va.y;
}

///内積演算子
float 
operator*(const Vector2& va, const Vector2& vb) {
	return Dot(va, vb);
}

///外積演算子
float 
operator%(const Vector2& va, const Vector2& vb) {
	return Cross(va, vb);
}

bool IsHit(const Circle& c1, const Circle& c2)
{
	float distance = (c1.pos - c2.pos).Magnitude();
	return distance <= (c1.radius + c2.radius);
}

Position2 GetHitPoint(const Circle& c1, const Circle& c2)
{
	auto normal = (c2.pos - c1.pos).Normalized();
	return c1.pos + normal * c1.radius;

}

Vector2 GetHitNormal(const Circle& c1, const Circle& c2)
{
	auto normal = (c2.pos - c1.pos).Normalized();
	return normal;
}

Vector2 GetOverlapVector(const Circle& c1, const Circle& c2)
{
	auto normal = (c2.pos - c1.pos);
	float distance = normal.Magnitude();
	float overlap = (c1.radius + c2.radius) - distance;
	return normal.Normalized() * overlap;
}

bool IsHitFloor(const Circle& c, float floorY)
{
	return c.pos.y + c.radius >= floorY;
}

Vector2 GetHitFloorNormal(const Circle& c, float floorY)
{
	return Vector2(0, -1) * ((c.pos.y + c.radius) - floorY);
}

bool IsHitWallL(const Circle& c, float wallX)
{
	return c.pos.x - c.radius <= wallX;
}

bool IsHitWallR(const Circle& c, float wallX)
{
	return c.pos.x + c.radius >= wallX;
}

Vector2 GetHitWallLNormal(const Circle& c, float wallX)
{
	return Vector2(1, 0) * (wallX - (c.pos.x - c.radius));
}

Vector2 GetHitWallRNormal(const Circle& c, float wallX)
{
	return Vector2(-1, 0) * ((c.pos.x + c.radius) - wallX);
}


void 
Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
}
void 
Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
}

void Circle::Draw(uint32_t color)
{
	DrawCircleAA(pos.x, pos.y, radius, 16, color, true);
}
