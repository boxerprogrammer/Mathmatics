#include "Geometry.h"



Vector2f ConvertToVector2f(const Vector2& v) {
	return Vector2f(v.x, v.y);
}

float Dot(const Vector2f& lval, const Vector2f& rval) {
	return lval.x*rval.x + lval.y*rval.y;
}


float Cross(const Vector2f& lval, const Vector2f& rval) {
	return lval.x*rval.y - lval.y*rval.x;
}

///２つの行列の乗算を返す
///@param lmat 左辺値(行列)
///@param rmat 右辺値(行列)
///@attention 乗算の順序に注意してください
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

///ベクトルに対して行列乗算を適用し、結果のベクトルを返す
///@param mat 行列
///@param vec ベクトル
Vector2f
MultipleVec(const Matrix& mat, const Vector2f& vec) {
	Vector2f ret = {};
	ret.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2];
	ret.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2];
	return ret;
}


///単位行列を返す
Matrix IdentityMat() {
	Matrix ret = {};
	ret.m[0][0] = ret.m[1][1] = ret.m[2][2] = 1;
	return ret;
}

///平行移動行列を返す
///@param x X方向平行移動量
///@param y Y方向平行移動量
Matrix TranslateMat(float x, float y) {
	Matrix ret = {};
	ret = IdentityMat();
	ret.m[0][2] = x;
	ret.m[1][2] = y;
	return ret;
}

///回転行列を返す
///@param angle 回転角度
Matrix RotateMat(float angle) {
	Matrix ret = {};
	ret = IdentityMat();
	ret.m[0][0] = cos(angle);
	ret.m[0][1] = -sin(angle);
	ret.m[1][0] = sin(angle);
	ret.m[1][1] = cos(angle);
	return ret;
}