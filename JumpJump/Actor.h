#pragma once
#include"Geometry.h"
/// <summary>
/// �Q�[�����ɔz�u�ł�����N���X
/// </summary>
class Actor
{
protected:
	int handle_;
	int frame_;
	int totalFrame_;
	Position2 pos_;
public:
	Actor();
	virtual void Update()=0;
	void Draw();

};

