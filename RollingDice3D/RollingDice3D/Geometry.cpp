#include"Geometry.h"
#include<DxLib.h>
#include<cmath>

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

const float
Vector2::Magnitude()const {
	return hypot(x, y);
}


void 
Vector2::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}


Vector2
Vector2::Normalized() {
	float mag = Magnitude();
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


//-------ここから3Dのターン------
void
Vector3::operator*=(float scale) {
	x *= scale;
	y *= scale;
	z *= scale;
}

Vector3
Vector3::operator*(float scale)const {
	return Vector3(x*scale, y*scale,z*scale);
}

Vector3 operator+(const Vector3& va, const Vector3 vb) {
	return Vector3(va.x + vb.x, va.y + vb.y,va.z+vb.z);
}

Vector3 operator-(const Vector3& va, const Vector3 vb) {
	return Vector3(va.x - vb.x, va.y - vb.y,va.z-vb.z);
}

float
Vector3::Magnitude()const {
	return sqrt(SQMagnitude());
}

float
Vector3::SQMagnitude()const {
	return x*x + y*y + z*z;
}

void
Vector3::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}


Vector3
Vector3::Normalized()const {
	float mag = Magnitude();
	return Vector3(x / mag, y / mag,z/mag);
}


///内積を返す
float
Dot(const Vector3& va, const Vector3& vb) {
	return va.x*vb.x + va.y*vb.y+va.z*vb.z;
}

///外積を返す
Vector3
Cross(const Vector3& va, const Vector3& vb) {
	return Vector3(va.z*vb.y-va.y*vb.z,va.z*vb.x-va.x*vb.z,va.x*vb.y - vb.x*va.y);
}

///内積演算子
float
operator*(const Vector3& va, const Vector3& vb) {
	return Dot(va, vb);
}

///外積演算子
Vector3
operator%(const Vector3& va, const Vector3& vb) {
	return Cross(va, vb);
}

void
Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}
void
Vector3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const
{
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m[i][0] * rhs.m[0][j] +
							 m[i][1] * rhs.m[1][j] +
							 m[i][2] * rhs.m[2][j] +
							 m[i][3] * rhs.m[3][j];
		}
	}
	return result;
}

Vector3 Matrix4x4::operator*(const Vector3& v) const
{
	Vector3 result;
	result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
	result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
	result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
	return result;
}

Matrix4x4 IdentityMatrix()
{
	return {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

Matrix4x4 TransposeMatrix(const Matrix4x4& m)
{
	return {
		m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
		m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
		m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
		m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]
	};
}

Matrix4x4 RotationMatrixX(float angle)
{
	return {
		1, 0, 0, 0,
		0, cos(angle), -sin(angle), 0,
		0, sin(angle), cos(angle), 0,
		0, 0, 0, 1
	};
}

Matrix4x4 RotationMatrixY(float angle)
{
	return {
		cos(angle), 0, sin(angle), 0,
		0, 1, 0, 0,
		-sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1
	};
}

Matrix4x4 RotationMatrixZ(float angle)
{
	return {
		cos(angle), -sin(angle), 0, 0,
		sin(angle), cos(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

Matrix4x4 TranslationMatrix(const Vector3& v)
{
	return {
		1, 0, 0, v.x,
		0, 1, 0, v.y,
		0, 0, 1, v.z,
		0, 0, 0, 1
	};
}

Matrix4x4 TranslationMatrix(float x, float y, float z)
{
	return {
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	};
}
