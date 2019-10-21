#pragma once
#include<cmath>
template<typename T>
struct Vector2D {
	Vector2D() : x(0), y(0) {}
	Vector2D(T inx, T iny) : x(inx), y(iny) {}
	T x;
	T y;
	void operator+=(const Vector2D<T>& in) {
		x += in.x;
		y += in.y;
	}
	void operator*=(float scale) {
		x *= scale;
		y *= scale;
	}
	void operator-=(const Vector2D<T>& in) {
		x -= in.x;
		y -= in.y;
	}

	Vector2D<int> ToIntVec()const {
		Vector2D<int> v(x, y);
		return v;
	}
	Vector2D<float> ToFloatVec()const {
		Vector2D<float> v(x, y);
		return v;
	}
	float Length()const {
		return hypot(x, y);
	}

	float SQLength()const {
		return x*x+y*y;
	}

	Vector2D<float> Normalized()const {
		auto len = Length();
		return Vector2D<float>((float)x / len, (float)y / len);
	}

	//90°回転する
	Vector2D<float> Orthogonaled()const {
		return Vector2D<float>((float)y , (float)-x );
	}

	//拡大
	Vector2D<float> Scaled(float scale) {
		return Vector2D<float>(x*scale, y*scale);
	}

	Vector2D<T> operator-() {
		return Vector2D<T>(-x, -y);
	}

};



//Vector2D<T>のための+オペレータオーバーロード
template<typename T>
Vector2D<T> operator+(const Vector2D<T>& lv, const Vector2D<T>& rv) {
	return Vector2D<T>(lv.x + rv.x, lv.y + rv.y);
}

template<typename T>
Vector2D<T> operator-(const Vector2D<T>& lv, const Vector2D<T>& rv) {
	return Vector2D<T>(lv.x - rv.x, lv.y - rv.y);
}

template<typename T>
Vector2D<T> operator*(const Vector2D<T>& lv, const float scale) {
	return Vector2D<T>(lv.x*scale, lv.y*scale);
}

//整数型ベクトル
typedef Vector2D<int> Vector2;
typedef Vector2 Position2;

//実数ベクトル
typedef Vector2D<float> Vector2f;
typedef Vector2f Position2f;

Vector2f ConvertToVector2f(const Vector2& v);

float Dot(const Vector2f& lval, const Vector2f& rval);




//3D--------------------------------------------------------------
template<typename T>
struct Vector3D {
	Vector3D() : x(0), y(0),z(0) {}
	Vector3D(T inx, T iny,T inz) : x(inx), y(iny),z(inz) {}
	T x;
	T y;
	T z;
	void operator+=(const Vector3D<T>& in) {
		x += in.x;
		y += in.y;
		z += in.z;
	}
	void operator*=(float scale) {
		x *= scale;
		y *= scale;
		z *= scale;
	}
	void operator-=(const Vector3D<T>& in) {
		x -= in.x;
		y -= in.y;
		z -= in.z;
	}

	Vector3D<int> ToIntVec()const {
		Vector3D<int> v(x, y,z);
		return v;
	}
	Vector3D<float> ToFloatVec()const {
		Vector3D<float> v(x, y,z);
		return v;
	}
	float Length()const {
		return std::hypot(std::hypotf(x, y), z);
	}

	float SQLength()const {
		return x * x + y * y+ z*z;
	}

	Vector3D<float> Normalized()const {
		auto len = Length();
		return Vector3D<float>((float)x / len, (float)y / len,(float)z/len);
	}

	//Z軸中心に90°回転する
	Vector3D<float> Orthogonaled()const {
		return Vector3D<float>((float)y, (float)-x,z);
	}

	//拡大
	Vector3D<float> Scaled(float scale) {
		return Vector3D<float>(x*scale, y*scale,z*scale);
	}

	Vector3D<T> operator-() {
		return Vector3D<T>(-x, -y,-z);
	}

};



//Vector3D<T>のための+オペレータオーバーロード
template<typename T>
Vector3D<T> operator+(const Vector3D<T>& lv, const Vector3D<T>& rv) {
	return Vector3D<T>(lv.x + rv.x, lv.y + rv.y,lv.z+rv.z);
}

template<typename T>
Vector3D<T> operator-(const Vector3D<T>& lv, const Vector3D<T>& rv) {
	return Vector3D<T>(lv.x - rv.x, lv.y - rv.y,lv.z-rv.z);
}

template<typename T>
Vector3D<T> operator*(const Vector3D<T>& lv, const float scale) {
	return Vector3D<T>(lv.x*scale, lv.y*scale,lv.z*scale);
}

//整数型ベクトル
typedef Vector3D<int> Vector3;
typedef Vector3 Position3;

//実数ベクトル
typedef Vector3D<float> Vector3f;
typedef Vector3f Position3f;

Vector3f ConvertToVector3f(const Vector3& v);

float Dot(const Vector3f& lval, const Vector3f& rval);
Vector3f Cross(const Vector3f& lval, const Vector3f& rval);

