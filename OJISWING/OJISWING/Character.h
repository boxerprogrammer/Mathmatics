#pragma once

#include<vector>



struct Position{
	Position(float ix,float iy){x=ix;y=iy;}
	Position(){
		x=0;
		y=0;
	}
	float x;
	float y;
	float Length(){
		return hypotf(x,y);
	}
	Position normalized(){
		return Position(x/Length(),y/Length());
	}
	Position operator+(const Position& in){
		return Position(x+in.x,y+in.y);
	}

	Position operator-(const Position& in){
		return Position(x-in.x,y-in.y);
	}

	Position operator*(float s){
		return Position(x*s,y*s);
	}

};

typedef Position Vec2;

inline float Dot(const Vec2& a, const Vec2& b){
	return a.x*b.x+a.y*b.y;
}

inline float Cross(const Vec2& a, const Vec2& b){
	return a.x*b.y - b.x*a.y;
}

struct Size{
	int w;
	int h;
};

///アニメーション情報
struct AnimInfo{
	int animNo;
	int wait;
	int nextIndex;
};

class Character
{
public:
	Character(void);
	Character(const char* filename,int divnumx=1,int divnumy=1);
	~Character(void);
	///アニメーション情報を追加する
	///@param animNo アニメーション番号(絵の番号)
	///@param frames このアニメーションの次のインデクス
	///@note nextIndexに-1を入れると、どのフレームにも跳ばなくなります。
	void AddAnimInfo(int animNo,int wait,int nextIndex=-1);

	///画像の位置を変更する
	///@param x ｘ座標
	///@param y ｙ座標
	void SetPosition(float x,float y);

	///画像を指定座標分移動させる
	void Move(float x, float y);

	
protected:
	Position _pos;
	///画像の描画
	void Draw();
	float _angle;
private:
	int _animCount;
	std::vector<int> _handles;
	int _divnum;
	int _xnum;
	Size _graphSize;
	Size _unitsize;
	std::vector<AnimInfo> _animData;
	
	int _currentIndex;
};

