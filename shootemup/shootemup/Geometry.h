#pragma once


//�T�C�Y��\���\����
struct Size {
	float w;//��
	float h;//����
};

//���W��\���\����
struct Vector2 {
	Vector2():x(0),y(0){}
	Vector2(float inx,float iny):x(inx),y(iny){}
	float x, y;
	///�x�N�g���̑傫����Ԃ��܂�
	float Magnitude()const;
	
	///���K��(�傫�����P��)���܂�
	void Normalize();

	///���K���x�N�g����Ԃ��܂�
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

///���ς�Ԃ�
float Dot(const Vector2& va, const Vector2& vb);

///�O�ς�Ԃ�
float Cross(const Vector2& va, const Vector2& vb);

///���ω��Z�q
float operator*(const Vector2& va, const Vector2& vb);

///�O�ω��Z�q
float operator%(const Vector2& va, const Vector2& vb);

//�Ƃ肠�����u���W�v���ĈӖ����ƃx�N�^���
//Position�̂ق����悭�ˁH���ė��R�ł��̖��O
typedef Vector2 Position2;

struct Circle {
	float radius;//���a
	Position2 pos; //���S���W
	Circle() :radius(0), pos(0, 0) {}
	Circle(float r, Position2& p) :radius(r), pos(p) {}
};

struct Rect {
	Position2 pos; //���S���W
	int w, h;//��,����
	Rect() : pos(0, 0), w(0), h(0) {}
	Rect(float x, float y, int inw, int inh) :
		pos(x, y), w(inw), h(inh) {}
	Rect(Position2& inpos, int inw, int inh) :
		pos(inpos), w(inw), h(inh)
	{}
	void SetCenter(float x, float y) {
		pos.x = x;
		pos.y = y;
	}
	void SetCenter(const Position2& inpos) {
		pos.x = inpos.x;
		pos.y = inpos.y;
	}
	Vector2 Center() {
		return pos;
	}
	float Left() { return pos.x - w / 2; }
	float Top() { return pos.y - h / 2; }
	float Right() { return pos.x + w / 2; }
	float Bottom() { return pos.y + h / 2; }
	void Draw();//�����̋�`��`�悷��
	void Draw(Vector2& offset);//�����̋�`��`�悷��(�I�t�Z�b�g�t��)
};

