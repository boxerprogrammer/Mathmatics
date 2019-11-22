#include"Geometry.h"
#include<DxLib.h>
#include<cmath>


void
Rect::Draw() {
	DxLib::DrawBox(Left()*2, Top()*2, Right()*2, Bottom()*2, 0xffffffff, false);
}

void
Rect::Draw(Vector2& offset) {
	DxLib::DrawBox((Left()+offset.x)*2, (Top()+offset.y)*2, (Right()+offset.x)*2, (Bottom()+offset.y)*2, 0xffffffff, false);
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


///���ς�Ԃ�
float
Dot(const Vector2& va, const Vector2& vb) {
	return va.x*vb.x + va.y*vb.y;
}

///�O�ς�Ԃ�
float
Cross(const Vector2& va, const Vector2& vb) {
	return va.x*vb.y - vb.x*va.y;
}

///���ω��Z�q
float 
operator*(const Vector2& va, const Vector2& vb) {
	return Dot(va, vb);
}

///�O�ω��Z�q
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


//-------��������3D�̃^�[��------
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


///���ς�Ԃ�
float
Dot(const Vector3& va, const Vector3& vb) {
	return va.x*vb.x + va.y*vb.y+va.z*vb.z;
}

///�O�ς�Ԃ�
Vector3
Cross(const Vector3& va, const Vector3& vb) {
	return Vector3(va.z*vb.y-va.y*vb.z,va.z*vb.x-va.x*vb.z,va.x*vb.y - vb.x*va.y);
}

///���ω��Z�q
float
operator*(const Vector3& va, const Vector3& vb) {
	return Dot(va, vb);
}

///�O�ω��Z�q
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


///�����蔻��
bool 
Plane::CheckHit(const RayLine& ray, Position3& hitpos, Vector3& norm) const {
	auto rv = ray.vector;
	rv.Normalize();
	if (rv.Normalized()* normal < 0) {
		float t = (offset - ray.start *normal) / (rv *normal);
		hitpos=  ray.start + (rv*t);
		norm = normal.Normalized();
		return true;
	}
	return false;
}

///�}�e���A����Ԃ��܂�
const Material& 
Plane::GetMaterial()const {
	return material;
}

///�w����W����̍ŒZ����(���������璆�S�Ƃ̋���-���a�B���ʂ������琂���̒���)
float 
Plane::DistanceFromStart(const Position3& pos) {
	return (normal*pos) - offset;
}
///�����蔻��
bool 
Sphere::CheckHit(const RayLine& ray, Position3& hitpos, Vector3& normal)const{
	//���C�����K���ς݂ł���O��Łc
	//
	//���_���狅�̒��S�ւ̃x�N�g��(����)�����܂�
	Vector3 tocenter=pos-ray.start;
	//
	//���S���王���ւ̓��ς��Ƃ�܂��˃x�N�g����
	float dot=Dot(tocenter,ray.vector);

	if (dot < 0.0f)return false;

	//
	//�����x�N�g���ƃx�N�g�����������āA���S����̐����������_�����߂܂�
	Vector3 raydash=ray.vector*dot;
	
	Vector3 vertical = tocenter - raydash;//�����x�N�g��
	//���̐����̒��������a�ȓ����ǂ����Ń��C�����ɂ������Ă��邩��
	//���肵�܂�
	float sqmag = vertical.SQMagnitude();
	if (sqmag < radius*radius) {
		float w = sqrt(radius*radius - sqmag);
		hitpos = ray.start+ray.vector*(dot - w);//��_
		normal = ray.vector*(dot - w) - tocenter;//���S�����_�ւ̃x�N�g��
		normal.Normalize();
		return true;
	}
	else {
		return false;
	}
}

///�}�e���A����Ԃ��܂�
const Material& 
Sphere::GetMaterial()const {
	return material;
}

///�w����W����̍ŒZ����(���������璆�S�Ƃ̋���-���a�B���ʂ������琂���̒���)
float 
Sphere::DistanceFromStart(const Position3& p) {
	return (pos-p).Magnitude()-radius;
}