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
