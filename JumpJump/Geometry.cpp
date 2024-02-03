#include"Geometry.h"
#include<DxLib.h>
#include<cmath>


void
Rect::Draw(unsigned int color,bool fill) {
	DxLib::DrawBox(Left() , Top() , Right() , Bottom(), color, fill);
}

void
Rect::Draw(Vector2& offset) {
	DxLib::DrawBox((Left() + offset.x) , (Top() + offset.y), (Right() + offset.x), (Bottom() + offset.y), 0xffffffff, false);
}
std::array<Position2, 4> 
Rect::GetPositions()const {
	std::array<Position2, 4> verts;
	verts[0] = Vector2(Left(), Top());
	verts[1] = Vector2(Right(), Top());
	verts[2] = Vector2(Left(), Bottom());
	verts[3] = Vector2(Right(), Bottom());
	return verts;
}
void
Vector2::operator*=(float scale) {
	x *= scale;
	y *= scale;
}

Vector2
Vector2::operator*(float scale) const{
	return { x * scale, y * scale };
}

Vector2 operator+(const Vector2& va, const Vector2 vb) {
	Vector2 ret;
	ret.x = va.x + vb.x;
	ret.y = va.y + vb.y;
	return ret;
}

Vector2 operator-(const Vector2& va, const Vector2 vb) {
	return { va.x - vb.x, va.y - vb.y };
}

float
Vector2::Magnitude()const {
	return std::hypot(x, y);
}


void
Vector2::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}


Vector2
Vector2::Normalized()const {
	float mag = Magnitude();
	return Vector2(x / mag, y / mag);
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


///�Q�̍s��̏�Z��Ԃ�
///@param lmat ���Ӓl(�s��)
///@param rmat �E�Ӓl(�s��)
///@attention ��Z�̏����ɒ��ӂ��Ă�������
Matrix
MultipleMat(const Matrix& lmat, const Matrix& rmat) {
	Matrix ret = {};

	for (int k = 0; k <= 2; ++k) {
		for (int j = 0; j <= 2; ++j) {
			for (int i = 0; i <= 2; ++i) {
				ret.m[k][j] += lmat.m[k][i] * rmat.m[i][j];
			}
		}
	}

	return ret;

}

Matrix operator*(const Matrix& lmat, const Matrix& rmat) {
	return MultipleMat(lmat, rmat);
}

///�x�N�g���ɑ΂��čs���Z��K�p���A���ʂ̃x�N�g����Ԃ�
///@param mat �s��
///@param vec �x�N�g��
Vector2
MultipleVec(const Matrix& mat, const Vector2& vec) {
	Vector2 ret = {};
	ret.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2];
	ret.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2];
	return ret;
}
Vector2 operator*(const Matrix& mat, const Vector2& vec) {
	return MultipleVec(mat, vec);
}

///�P�ʍs���Ԃ�
Matrix IdentityMat() {
	Matrix ret = {};
	ret.m[0][0] = ret.m[1][1] = ret.m[2][2] = 1;
	return ret;
}

///���s�ړ��s���Ԃ�
///@param x X�������s�ړ���
///@param y Y�������s�ړ���
Matrix TranslateMat(float x, float y) {
	Matrix ret = {};
	ret = IdentityMat();
	ret.m[0][2] = x;
	ret.m[1][2] = y;
	return ret;
}

///��]�s���Ԃ�
///@param angle ��]�p�x
Matrix RotateMat(float angle) {
	Matrix ret = {};
	ret = IdentityMat();
	ret.m[0][0] = cos(angle);
	ret.m[0][1] = -sin(angle);
	ret.m[1][0] = sin(angle);
	ret.m[1][1] = cos(angle);
	return ret;
}