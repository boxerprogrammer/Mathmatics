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
	void operator/=(float div) {
		x /= div;
		y /= div;
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
	void Normalize() {
		auto len = Length();
		x /= len;
		y /= len;
	}

	//90����]����
	Vector2D<float> Orthogonaled()const {
		return Vector2D<float>((float)y , (float)-x );
	}

	//�g��
	Vector2D<float> Scaled(float scale) {
		return Vector2D<float>(x*scale, y*scale);
	}

	Vector2D<T> operator-() {
		return Vector2D<T>(-x, -y);
	}

};



//Vector2D<T>�̂��߂�+�I�y���[�^�I�[�o�[���[�h
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

template<typename T>
Vector2D<T> operator/(const Vector2D<T>& lv, const float div) {
	return Vector2D<T>(lv.x /div, lv.y /div);
}


//�����^�x�N�g��
typedef Vector2D<int> Vector2;
typedef Vector2 Position2;

//�����x�N�g��
typedef Vector2D<float> Vector2f;
typedef Vector2f Position2f;

Vector2f ConvertToVector2f(const Vector2& v);

float Dot(const Vector2f& lval, const Vector2f& rval);

struct Matrix {
	float m[3][3];
	void operator*=(const Matrix& mat);
};

Matrix operator*(const Matrix& mat, float scale);
Matrix operator+(const Matrix& matA, const Matrix& matB);

///�P�ʍs���Ԃ�
Matrix IdentityMat();

///���s�ړ��s���Ԃ�
///@param x X�������s�ړ���
///@param y Y�������s�ړ���
Matrix TranslateMat(float x, float y);

///��]�s���Ԃ�
///@param angle ��]�p�x
Matrix RotateMat(float angle);

///�Q�̍s��̏�Z��Ԃ�
///@param lmat ���Ӓl(�s��)
///@param rmat �E�Ӓl(�s��)
///@attention ��Z�̏����ɒ��ӂ��Ă�������
Matrix MultipleMat(const Matrix& lmat, const Matrix& rmat);

Matrix operator*(const Matrix& lmat, const Matrix& rmat);
Vector2f operator*(const Matrix& lmat, const Vector2f& vec);


///�x�N�g���ɑ΂��čs���Z��K�p���A���ʂ̃x�N�g����Ԃ�
///@param mat �s��
///@param vec �x�N�g��
Vector2f MultipleVec(const Matrix& mat, const Vector2f& vec);

///�s�����`�⊮����
///@param matA �s��A
///@param matB �s��B
///@param t ����
///t�͈̔͂�0.0�`1.0�ŁAmatA*(1-t)+matB*t�ɕ�Ԃ����
Matrix LinearInterporate(const Matrix& matA, const Matrix& matB, float t);