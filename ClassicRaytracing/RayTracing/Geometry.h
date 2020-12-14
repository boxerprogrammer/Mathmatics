#pragma once


//�T�C�Y��\���\����
struct Size {
	float w;//��
	float h;//����
};

//2D���W�E�x�N�g����\���\����
struct Vector2 {
	Vector2():x(0),y(0){}
	Vector2(float inx,float iny):x(inx),y(iny){}
	float x, y;
	///�x�N�g���̑傫����Ԃ��܂�
	const float Magnitude()const;
	
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


//3D���W�E�x�N�g����\���\����
struct Vector3 {
	Vector3() :x(0), y(0) ,z(0){}
	Vector3(float inx, float iny,float inz) :x(inx), y(iny) ,z(inz){}
	float x, y,z;
	///�x�N�g���̑傫����Ԃ��܂�
	float Magnitude()const;
	float SQMagnitude()const;

	///���K��(�傫�����P��)���܂�
	void Normalize();

	///���K���x�N�g����Ԃ��܂�
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

///���ς�Ԃ�
float Dot(const Vector3& va, const Vector3& vb);

///�O�ς�Ԃ�
Vector3 Cross(const Vector3& va, const Vector3& vb);

///���ω��Z�q
float operator*(const Vector3& va, const Vector3& vb);

///�O�ω��Z�q
Vector3 operator%(const Vector3& va, const Vector3& vb);
typedef Vector3 Position3;

///�~��\���\����
struct Circle {
	float radius;//���a
	Position2 pos; //���S���W
	Circle() :radius(0), pos(0, 0) {}
	Circle(float r, Position2& p) :radius(r), pos(p) {}
};

///����(����)��\���\����
struct RayLine {
		Position3 start;//���_���W
		Vector3 vector;//������\���x�N�g��

		///�R���X�g���N�^
		///@param ���_(�n�_)
		///@param ������\���x�N�g��
		RayLine(const Position3& s, const Vector3& v):start(s),vector(v.Normalized()) {}

		///���_�ƏI�_���王���𐶐�
		///@param ���_(�n�_)
		///@param �I�_
		void InitFromStartEnd(const Position3& s, const Vector3& e) {
			start = s;
			vector = (e - s).Normalized();
		}
};

///�F
struct Color {
	float r, g, b;//RGB�F
	Color(float red, float green, float blue) :r(red), g(green), b(blue) {}
	bool operator==(const Color& c) {
		return r == c.r&&
		g == c.g&&
		b == c.b;
	}
	void operator=(const Color& c) {
		r = c.r;
		g = c.g;
		b = c.b;
	}
	void operator*=(float m) {
		r *= m;
		g *= m;
		b *= m;
	}
	void operator*=(const Color& c) {
		r *= c.r;
		g *= c.g;
		b *= c.b;
	}
	void operator+=(const Color& c) {
		r += c.r;
		g += c.g;
		b += c.b;
	}
	Color operator*(float m)const  {
		return Color(r * m,g*m,b*m);
	}
	Color operator*(const Color& c)const {
		return Color(r * c.r, g * c.g, b * c.b);
	}
	Color operator+(const Color& c) {
		return Color(r + c.r,	g + c.g,b + c.b);
	}
};


///�\�ʖ͗l��`
enum class Pattern {
	none,//�͗l�Ȃ�
	hstripe,//�Ȗ͗l(��)
	vstripe,//�Ȗ͗l(�c)
	checkered,//�s���͗l(�`�F�b�J�[�t���b�O)
	textured//�e�N�X�`���}�b�s���O
};

///�\�ʍގ�	
struct Material {
	Color diffuse;//�g�U���ː���(cos�ŕ\��������Ԃ��F)
	Color specular;//���ʔ��ː���(�X�y�L�����Ō��肳���\�ʍގ����Ԃ��F)
	Color ambient;//����(�Q�^)
	float specularity;//�X�y�L�����̋���(�搔)
	float reflectance;//���˗�
	Pattern pattern;//�͗l���
	Material() : diffuse(1,1,1),specular(0,0,0),ambient(0,0,0),specularity(0),reflectance(0){}
	Material(const Color& diff,
		const Color& spec,
		const Color& amb,
		float inSpecularity,
		float inReflectance,
		Pattern p=Pattern::none) : diffuse(diff), specular(spec), ambient(amb), specularity(inSpecularity), reflectance(inReflectance),pattern(p) {}
};

///���ʂƂ������ꏏ�����ɂ��邽�߂ɂ�����
struct GeometryObject {
	Material material;//�\�ʍގ�

	GeometryObject(const Material& m) :material(m) {}

	///�����蔻��
	///@param ray ��������n�܂钼��
	///@param hitpos[out] �Փ˓_
	///@param normal[out] �Փ˓_�̖@���x�N�g��
	///@retval true ��������
	///@retval false �O�ꂽ
	virtual bool CheckHit(const RayLine& ray, Position3& hitpos, Vector3& normal)const = 0;

	///�}�e���A����Ԃ��܂�
	///@return �}�e���A���I�u�W�F�N�g
	virtual const Material& GetMaterial()const=0;

	///�w����W����̍ŒZ����(���������璆�S�Ƃ̋���-���a�B���ʂ������琂���̒���)
	///@param p �����𑪂肽���_(���W)
	virtual float DistanceFromStart(const Position3& pos) = 0;
};

///��(�Ƃ���������)��\���\����
struct Plane : public GeometryObject {
	Vector3 normal;//�@���x�N�g��(a,b,c)�����K�����Ƃ�
	float offset;//���_����̃I�t�Z�b�g(d)
	Plane(Vector3& n, float o,const Material& m) :normal(n), offset(o),GeometryObject(m) {}

	///�����蔻��
	///@param ray ��������n�܂钼��
	///@param hitpos[out] �Փ˓_
	///@param norm[out] �Փ˓_�̖@���x�N�g��
	///@retval true ��������
	///@retval false �O�ꂽ
	bool CheckHit(const RayLine& ray, Position3& hitpos, Vector3& norm)const;

	///�}�e���A����Ԃ��܂�
	const Material& GetMaterial()const;

	///�w����W����̍ŒZ����(���������璆�S�Ƃ̋���-���a�B���ʂ������琂���̒���)
	float DistanceFromStart(const Position3& pos);
};

///����\���\����
struct Sphere : public GeometryObject {
	float radius;//���a
	Position3 pos; //���S���W
	Sphere() :radius(0), pos(0, 0,0) , GeometryObject(Material()) {}
	Sphere(float r, Position3& p,Material& m) :radius(r), pos(p) ,GeometryObject(m){}
	///�����蔻��
	bool CheckHit(const RayLine& ray, Position3& hitpos, Vector3& normal)const;

	///�}�e���A����Ԃ��܂�
	const Material& GetMaterial()const;

	///�w����W����̍ŒZ����(���������璆�S�Ƃ̋���-���a�B���ʂ������琂���̒���)
	float DistanceFromStart(const Position3& pos);
};

///��`��\���\����
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

