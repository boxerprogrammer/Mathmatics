#pragma once

#include"Geometry.h"

#include<string>

class Ball
{
private:
	bool isAvailable;
	Position2 initPos;
	unsigned int color;//�F
	Circle circle;
	Vector2 velocity;//���x�x�N�g��
	const std::string tag;
public:
	Ball(unsigned int col,float x,float y,const char* intag="");
	~Ball();
	
	bool IsAvailable()const;
	void Kill();

	///�󋵂�������Ԃɖ߂�
	void Reset();

	///�~�����擾
	const Circle& GetCircle()const;

	///�~�̌��݂̒��S�_���擾
	const Position2& GetPosition()const;

	///�~�̌��݂̔��a���擾
	float GetRadius()const;

	///�~�̌��݂̑��x���擾
	const Vector2& GetVelocity()const;

	///�~�̌��ݍ��W���Z�b�g
	void SetPosition(float x, float y);

	///�~�̌��ݍ��W���Z�b�g
	void SetPosition(const Position2& p);

	///�~�̑��x���Z�b�g
	void SetVelocity(const Vector2& v);

	///�~�̑��x�����Z
	void AddVelocity(const Vector2& v);

	///���x�𔽓](X����)
	void ReverseX();

	///���x�𔽓](Y����)
	void ReverseY();

	///�X�V�`�揈��
	void Update();

	std::string GetTag()const { return tag; }
};

