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
	
	//�@���x�x�N�g����K�p����
	//�Ȃ��A�{�[���̍��W��circle.pos�ŕ\����A�{�[���̑��x��velocity�ŕ\����Ă���
	circle.pos += velocity;
	
	//�{�[���`��
	DrawCircle(circle.pos.x, circle.pos.y, 10, color, true);//�{�[���{��
	DrawCircle(circle.pos.x, circle.pos.y, 11, 0x000000, false, 1);//�{�[���֊s
	DrawCircle(circle.pos.x-4, circle.pos.y-4, 2, 0xffffff, true);//��

	//�A���x������������
	//��������Ȃ��Ƃ��܂ł��{�[�������������Ď����łĂȂ��̂�
	//���������x�𗎂Ƃ��Ă�������
	velocity *= 0.995f;//�������x������

	//�ق��Ƃ��Ƃ��肪�Ȃ��̂ł�����x�Łu��~�v������
	//Magunitude()�Ƃ����̂͂��̃x�N�g���́u�傫���v��Ԃ�
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

///���x�𔽓](Y����)
void
Ball::ReverseY() {
	velocity.y = -velocity.y;
}