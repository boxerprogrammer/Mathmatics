#pragma once
#include"Ball.h"

///�L���[(�r�����[�h�̋���˂��A��)�N���X
class Cue
{
public:
	Cue(Ball& playerball,int imgH);
	~Cue();

	//�`�悷��
	void Draw();


	//�L���[�ŋ�������
	void Shot(Position2& tagetpos,float power=1.0f);

	void OnClickable();
private:
	Ball& _playerball;
	int _imgH = 0;
	bool _isClickable = true;
	bool _isShotting = false;
	int _countdowntimer = 0;
	Vector2 _force;
	Position2 _staypos;
	Size _imgSize;
	int _soundH;

	//�v���C���[�{�[���ɗ�(�Ƃ������̑��x�x�N�g��)��������
	void AddForce();
};

