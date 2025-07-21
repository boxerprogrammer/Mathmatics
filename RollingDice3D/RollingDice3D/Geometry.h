#pragma once


//サイズを表す構造体
struct Size {
	float w;//幅
	float h;//高さ
};

//サイズを表す構造体(3D)
struct Size3 {
	float w;//幅
	float h;//高さ
	float d;//奥行き
};

//2D座標・ベクトルを表す構造体
struct Vector2 {
	Vector2():x(0),y(0){}
	Vector2(float inx,float iny):x(inx),y(iny){}
	float x, y;
	///ベクトルの大きさを返します
	const float Magnitude()const;
	
	///正規化(大きさを１に)します
	void Normalize();

	///正規化ベクトルを返します
	Vector2 Normalized();

	void operator+=(const Vector2& v);
	void operator-=(const Vector2& v);
	void operator*=(float scale);
	Vector2 operator*(float scale);
	Vector2 operator-() {
		return Vector2(-x, -y);
	}
};

Vector2 operator+(const Vector2& va, const Vector2 vb);
Vector2 operator-(const Vector2& va, const Vector2 vb);

///内積を返す
float Dot(const Vector2& va, const Vector2& vb);

///外積を返す
float Cross(const Vector2& va, const Vector2& vb);

///内積演算子
float operator*(const Vector2& va, const Vector2& vb);

///外積演算子
float operator%(const Vector2& va, const Vector2& vb);

//とりあえず「座標」って意味だとベクタより
//Positionのほうがよくね？って理由でこの名前
typedef Vector2 Position2;


//3D座標・ベクトルを表す構造体
struct Vector3 {
	Vector3() :x(0), y(0) ,z(0){}
	Vector3(float inx, float iny,float inz) :x(inx), y(iny) ,z(inz){}
	float x, y,z;
	///ベクトルの大きさを返します
	float Magnitude()const;
	float SQMagnitude()const;

	///正規化(大きさを１に)します
	void Normalize();

	///正規化ベクトルを返します
	Vector3 Normalized()const;

	void operator+=(const Vector3& v);
	void operator-=(const Vector3& v);
	void operator*=(float scale);
	Vector3 operator*(float scale)const;
	Vector3 operator-() const {
		return Vector3(-x, -y,-z);
	}
};
Vector3 operator+(const Vector3& va, const Vector3 vb);
Vector3 operator-(const Vector3& va, const Vector3 vb);

///内積を返す
float Dot(const Vector3& va, const Vector3& vb);

///外積を返す
Vector3 Cross(const Vector3& va, const Vector3& vb);

///内積演算子
float operator*(const Vector3& va, const Vector3& vb);

///外積演算子
Vector3 operator%(const Vector3& va, const Vector3& vb);
typedef Vector3 Position3;

struct Matrix4x4 {
	float m[4][4]={};
	Matrix4x4 operator*(const Matrix4x4& rhs) const;
	Vector3 operator*(const Vector3& v) const;
};

///単位行列を返す
Matrix4x4 IdentityMatrix();
///転置行列を返す
Matrix4x4 TransposeMatrix(const Matrix4x4& m);

///X軸中心回転行列を返す
Matrix4x4 RotationMatrixX(float angle);
///Y軸中心回転行列を返す
Matrix4x4 RotationMatrixY(float angle);
///Z軸中心回転行列を返す
Matrix4x4 RotationMatrixZ(float angle);

///平行移動行列を返す
Matrix4x4 TranslationMatrix(const Vector3& v);
///平行移動行列を返す
Matrix4x4 TranslationMatrix(float x, float y, float z);