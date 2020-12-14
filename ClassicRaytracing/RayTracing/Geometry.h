#pragma once


//サイズを表す構造体
struct Size {
	float w;//幅
	float h;//高さ
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

///円を表す構造体
struct Circle {
	float radius;//半径
	Position2 pos; //中心座標
	Circle() :radius(0), pos(0, 0) {}
	Circle(float r, Position2& p) :radius(r), pos(p) {}
};

///直線(視線)を表す構造体
struct RayLine {
		Position3 start;//視点座標
		Vector3 vector;//方向を表すベクトル

		///コンストラクタ
		///@param 視点(始点)
		///@param 方向を表すベクトル
		RayLine(const Position3& s, const Vector3& v):start(s),vector(v.Normalized()) {}

		///視点と終点から視線を生成
		///@param 視点(始点)
		///@param 終点
		void InitFromStartEnd(const Position3& s, const Vector3& e) {
			start = s;
			vector = (e - s).Normalized();
		}
};

///色
struct Color {
	float r, g, b;//RGB色
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


///表面模様定義
enum class Pattern {
	none,//模様なし
	hstripe,//縞模様(横)
	vstripe,//縞模様(縦)
	checkered,//市松模様(チェッカーフラッグ)
	textured//テクスチャマッピング
};

///表面材質	
struct Material {
	Color diffuse;//拡散反射成分(cosで表されるやつが返す色)
	Color specular;//鏡面反射成分(スペキュラで決定される表面材質が返す色)
	Color ambient;//環境光(ゲタ)
	float specularity;//スペキュラの強さ(乗数)
	float reflectance;//反射率
	Pattern pattern;//模様種別
	Material() : diffuse(1,1,1),specular(0,0,0),ambient(0,0,0),specularity(0),reflectance(0){}
	Material(const Color& diff,
		const Color& spec,
		const Color& amb,
		float inSpecularity,
		float inReflectance,
		Pattern p=Pattern::none) : diffuse(diff), specular(spec), ambient(amb), specularity(inSpecularity), reflectance(inReflectance),pattern(p) {}
};

///平面とか球を一緒くたにするためにあるやつ
struct GeometryObject {
	Material material;//表面材質

	GeometryObject(const Material& m) :material(m) {}

	///当たり判定
	///@param ray 視線から始まる直線
	///@param hitpos[out] 衝突点
	///@param normal[out] 衝突点の法線ベクトル
	///@retval true 当たった
	///@retval false 外れた
	virtual bool CheckHit(const RayLine& ray, Position3& hitpos, Vector3& normal)const = 0;

	///マテリアルを返します
	///@return マテリアルオブジェクト
	virtual const Material& GetMaterial()const=0;

	///指定座標からの最短距離(球だったら中心との距離-半径。平面だったら垂線の長さ)
	///@param p 距離を測りたい点(座標)
	virtual float DistanceFromStart(const Position3& pos) = 0;
};

///床(というか平面)を表す構造体
struct Plane : public GeometryObject {
	Vector3 normal;//法線ベクトル(a,b,c)←正規化しとけ
	float offset;//原点からのオフセット(d)
	Plane(Vector3& n, float o,const Material& m) :normal(n), offset(o),GeometryObject(m) {}

	///当たり判定
	///@param ray 視線から始まる直線
	///@param hitpos[out] 衝突点
	///@param norm[out] 衝突点の法線ベクトル
	///@retval true 当たった
	///@retval false 外れた
	bool CheckHit(const RayLine& ray, Position3& hitpos, Vector3& norm)const;

	///マテリアルを返します
	const Material& GetMaterial()const;

	///指定座標からの最短距離(球だったら中心との距離-半径。平面だったら垂線の長さ)
	float DistanceFromStart(const Position3& pos);
};

///球を表す構造体
struct Sphere : public GeometryObject {
	float radius;//半径
	Position3 pos; //中心座標
	Sphere() :radius(0), pos(0, 0,0) , GeometryObject(Material()) {}
	Sphere(float r, Position3& p,Material& m) :radius(r), pos(p) ,GeometryObject(m){}
	///当たり判定
	bool CheckHit(const RayLine& ray, Position3& hitpos, Vector3& normal)const;

	///マテリアルを返します
	const Material& GetMaterial()const;

	///指定座標からの最短距離(球だったら中心との距離-半径。平面だったら垂線の長さ)
	float DistanceFromStart(const Position3& pos);
};

///矩形を表す構造体
struct Rect {
	Position2 pos; //中心座標
	int w, h;//幅,高さ
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
	void Draw();//自分の矩形を描画する
	void Draw(Vector2& offset);//自分の矩形を描画する(オフセット付き)
};

