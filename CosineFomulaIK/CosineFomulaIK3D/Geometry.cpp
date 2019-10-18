#include "Geometry.h"



Vector2f ConvertToVector2f(const Vector2& v) {
	return Vector2f(v.x, v.y);
}

float Dot(const Vector2f& lval, const Vector2f& rval) {
	return lval.x*rval.x + lval.y*rval.y;
}


Vector3f ConvertToVector3f(const Vector3& v) {
	return Vector3f(v.x, v.y,v.z);
}

float Dot(const Vector3f& lval, const Vector3f& rval) {
	return lval.x*rval.x + lval.y*rval.y + lval.z*rval.z;
}

Vector3f Cross(const Vector3f& lval, const Vector3f& rval) {
	return Vector3f(lval.y*rval.z - lval.z*rval.y,
		lval.z*rval.x - lval.x*rval.z,
		lval.x*rval.y - lval.y*rval.x);
}