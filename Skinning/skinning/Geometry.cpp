#include "Geometry.h"



Vector2f ConvertToVector2f(const Vector2& v) {
	return Vector2f(v.x, v.y);
}

float Dot(const Vector2f& lval, const Vector2f& rval) {
	return lval.x*rval.x + lval.y*rval.y;
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

///�x�N�g���ɑ΂��čs���Z��K�p���A���ʂ̃x�N�g����Ԃ�
///@param mat �s��
///@param vec �x�N�g��
Vector2f
MultipleVec(const Matrix& mat, const Vector2f& vec) {
	Vector2f ret = {};
	ret.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2];
	ret.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2];
	return ret;
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

Matrix 
operator*(const Matrix& mat, float scale) {
	Matrix ret = {};

	for (int i = 0; i <= 2; ++i) {
		for (int j = 0; j <= 2; ++j) {
			ret.m[i][j] = mat.m[i][j] * scale;
		}
	}

	return ret;
}
Matrix 
operator+(const Matrix& lmat, const Matrix& rmat) {
	Matrix ret = {};

	for (int i = 0; i <= 2; ++i) {
		for (int j = 0; j <= 2; ++j) {
			ret.m[i][j] = lmat.m[i][j] + rmat.m[i][j];
		}
	}

	return ret;
}
Matrix 
LinearInterporate(const Matrix& matA, const Matrix& matB, float t) {
	return matA * (1.0f - t) + matB * t;
}

Matrix operator*(const Matrix& lmat, const Matrix& rmat) {
	return MultipleMat(lmat, rmat);
}
Vector2f operator*(const Matrix& lmat, const Vector2f& vec) {
	return MultipleVec(lmat, vec);
}

void
Matrix::operator*=(const Matrix& mat) {
	*this=MultipleMat(*this, mat);
}