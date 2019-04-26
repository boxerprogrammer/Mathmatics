#pragma once
#include"Character.h"
class Ojisan : public Character
{
	public:
		Ojisan(const char* filename,int xnum,int ynum);
		~Ojisan(void);
		///���t���[���Ăяo������
		void Update();

		
		float GetX(){return _pos.x;}
		Vec2& GetPosition(){return _pos;};
		void SetPosition(float x, float y){_pos.x=x;_pos.y=y;};

		void SetGravity(float g){_g=g;};

		void SetLength(float len){_length;};
		
		void SetStartPosition(float x, float y);

		void SetEndPoint(float x, float y);

		void SetScroll(float x, float y){_scroll.x=x;_scroll.y=y;}

		Vec2& GetEndPoint(){return _endPoint;}


		void ResetOjisan(float x, float y);

	private:
		//�e�t�F�[�Y
		void (Ojisan::*_phase)();
		void FallingPhase();//�����t�F�[�Y
		void SwingPhase();//�U��q�t�F�[�Y
		void FlyingPhase();//�ˏo��t�F�[�Y



		void OnAdjust();

		void SetY(float y){SetPosition(_pos.x,y);}

		///y/x�̔䗦����͂���
		///@param yrate Y/X�䗦
		void SetAngle(float yrate );

		void SetAccel(float ax,float ay);
		Position _endPoint;
		Position _scroll;
		float _g;
		float _v;
		float _ax,_ay;
		float _vx,_vy;
		float _length;
		int _handle;
		bool _isPushJump;
		bool _isJumped;
		///�������C�x���g
		void OnInit(float& vx, float& vy);

		///�e���V�����L�����C�x���g
		void OnActivateTension();


		///���n�C�x���g
		void OnGround(float& x,float& y,float& vx,float& vy);

		///�ړ��C�x���g
		void OnMove(float& x,float& y,float vx,float vy);

		///�����C�x���g
		void OnAccel(float& vx,float& vy,const float& ax,const float& ay);

		///�W�����v�L�[�����ꂽ�C�x���g
		void OnPushJumpKey(float& vx,float& vy);

		///���L�[�����ꂽ�C�x���g
		void OnPushLeftKey(float& vx,float& vy);

		///�E�L�[�����ꂽ�C�x���g
		void OnPushRightKey(float& vx,float& vy);
};

