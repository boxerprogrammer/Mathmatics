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


///当たり判定
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

///マテリアルを返します
const Material& 
Plane::GetMaterial()const {
	return material;
}

///指定座標からの最短距離(球だったら中心との距離-半径。平面だったら垂線の長さ)
float 
Plane::DistanceFromStart(const Position3& pos) {
	return (normal*pos) - offset;
}
///当たり判定
bool 
Sphere::CheckHit(const RayLine& ray, Position3& hitpos, Vector3& normal)const{
	//レイが正規化済みである前提で…
	//
	//視点から球体中心へのベクトル(視線)を作ります
	Vector3 tocenter=pos-ray.start;
	//
	//中心から視線への内積をとります⇒ベクトル長
	float dot=Dot(tocenter,ray.vector);

	if (dot < 0.0f)return false;

	//
	//視線ベクトルとベクトル長をかけて、中心からの垂線下した点を求めます
	Vector3 raydash=ray.vector*dot;
	
	Vector3 vertical = tocenter - raydash;//垂線ベクトル
	//その垂線の長さが半径以内かどうかでレイが球にあたっているかを
	//判定します
	float sqmag = vertical.SQMagnitude();
	if (sqmag < radius*radius) {
		float w = sqrt(radius*radius - sqmag);
		hitpos = ray.start+ray.vector*(dot - w);//交点
		normal = ray.vector*(dot - w) - tocenter;//中心から交点へのベクトル
		normal.Normalize();
		return true;
	}
	else {
		return false;
	}
}

///マテリアルを返します
const Material& 
Sphere::GetMaterial()const {
	return material;
}

///指定座標からの最短距離(球だったら中心との距離-半径。平面だったら垂線の長さ)
float 
Sphere::DistanceFromStart(const Position3& p) {
	return (pos-p).Magnitude()-radius;
}