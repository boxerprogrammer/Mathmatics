#include "Geometry.h"



Vector2f ConvertToVector2f(const Vector2& v) {
	return Vector2f(v.x, v.y);
}

float Dot(const Vector2f& lval, const Vector2f& rval) {
	return lval.x*rval.x + lval.y*rval.y;
}
