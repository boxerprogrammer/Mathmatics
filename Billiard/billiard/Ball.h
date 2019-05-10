#pragma once

#include"Geometry.h"

#include<string>

class Ball
{
private:
	bool isAvailable;
	Position2 initPos;
	unsigned int color;//色
	Circle circle;
	Vector2 velocity;//速度ベクトル
	const std::string tag;
public:
	Ball(unsigned int col,float x,float y,const char* intag="");
	~Ball();
	
	bool IsAvailable()const;
	void Kill();

	///状況を初期状態に戻す
	void Reset();

	///円情報を取得
	const Circle& GetCircle()const;

	///円の現在の中心点を取得
	const Position2& GetPosition()const;

	///円の現在の半径を取得
	float GetRadius()const;

	///円の現在の速度を取得
	const Vector2& GetVelocity()const;

	///円の現在座標をセット
	void SetPosition(float x, float y);

	///円の現在座標をセット
	void SetPosition(const Position2& p);

	///円の速度をセット
	void SetVelocity(const Vector2& v);

	///円の速度を加算
	void AddVelocity(const Vector2& v);

	///速度を反転(X方向)
	void ReverseX();

	///速度を反転(Y方向)
	void ReverseY();

	///更新描画処理
	void Update();

	std::string GetTag()const { return tag; }
};

