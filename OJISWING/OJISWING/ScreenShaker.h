#pragma once
//��ʗh��N���X
class ScreenShaker
{
private:
	int _screenH;//���
	float _shakeScale;//�h�ꕝ
public:
	ScreenShaker();
	~ScreenShaker();
	//�h��g���K�[
	void Shake();

	//��ʗh�ꏀ��()
	void PrepareShake();

	//��ʗh��X�V
	void UpdateShake();
};

