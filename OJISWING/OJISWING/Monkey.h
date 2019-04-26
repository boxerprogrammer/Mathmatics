#pragma once
#include"Character.h"
class Monkey : public Character
{
	public:
		Monkey(const char* filename,int xnum,int ynum);
		~Monkey(void);
		///���t���[���Ăяo������
		void Update();
		
	private:
		float _vx,_vy;
		int _handle;
		bool _isPushJump;
		///�������C�x���g
		void OnInit(float& vx, float& vy);

		///���n�C�x���g
		void OnGround(int& x,int& y,float& vx,float& vy);

		///�ړ��C�x���g
		void OnMove(int& x,int& y,float vx,float vy);

		///�����C�x���g
		void OnAccel(float& vx,float& vy);

		///�W�����v�L�[�����ꂽ�C�x���g
		void OnPushJumpKey(float& vx,float& vy);

		///���L�[�����ꂽ�C�x���g
		void OnPushLeftKey(float& vx,float& vy);

		///�E�L�[�����ꂽ�C�x���g
		void OnPushRightKey(float& vx,float& vy);
};

