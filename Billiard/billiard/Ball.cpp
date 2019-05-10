#include "Ball.h"
#include<DxLib.h>


Ball::Ball(unsigned int col,float x,float y,const char* intag):
color(col),
circle(10,Vector2(x,y)),
initPos(x,y),
isAvailable(true),
tag(intag)
{
}
bool 
Ball::IsAvailable()const {
	return isAvailable;
}
void 
Ball::Kill() {
	isAvailable = false;
}

Ball::~Ball()
{
}

const Circle&
Ball::GetCircle()const {
	return circle;
}

const Vector2&
Ball::GetVelocity() const {
	return velocity;
}

void 
Ball::SetVelocity(const Vector2& v) {
	velocity = v;
}

void
Ball::AddVelocity(const Vector2& v) {
	velocity += v;
}
float
Ball::GetRadius()const {
	return circle.radius;
}

const Position2&
Ball::GetPosition()const {
	return circle.pos;
}

void
Ball::SetPosition(float x,float y) {
	circle.pos.x = x;
	circle.pos.y = y;
}

void
Ball::SetPosition(const Position2& p) {
	circle.pos = p;
}


void
Ball::Update() {
	if (!isAvailable)return;
	
	//①速度ベクトルを適用する
	//なお、ボールの座標はcircle.posで表され、ボールの速度はvelocityで表されている
	circle.pos += velocity;
	
	//ボール描画
	DrawCircle(circle.pos.x, circle.pos.y, 10, color, true);//ボール本体
	DrawCircle(circle.pos.x, circle.pos.y, 11, 0x000000, false, 1);//ボール輪郭
	DrawCircle(circle.pos.x-4, circle.pos.y-4, 2, 0xffffff, true);//艶

	//②速度を減衰させる
	//これをしないといつまでもボールが動き続けて次が打てないので
	//少しずつ速度を落としてください
	velocity *= 0.995f;//少しずつ遅くする

	//ほっとくときりがないのである程度で「停止」させる
	//Magunitude()というのはそのベクトルの「大きさ」を返す
	if (velocity.Magnitude() < 0.1f) {
		velocity = Vector2();
	}
}

void 
Ball::Reset() {
	isAvailable = true;
	circle.pos = initPos;
	velocity = Vector2();
}


void 
Ball::ReverseX() {
	velocity.x = -velocity.x;
}

///速度を反転(Y方向)
void
Ball::ReverseY() {
	velocity.y = -velocity.y;
}