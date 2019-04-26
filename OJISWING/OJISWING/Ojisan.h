#pragma once
#include"Character.h"
class Ojisan : public Character
{
	public:
		Ojisan(const char* filename,int xnum,int ynum);
		~Ojisan(void);
		///毎フレーム呼び出す処理
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
		//各フェーズ
		void (Ojisan::*_phase)();
		void FallingPhase();//落下フェーズ
		void SwingPhase();//振り子フェーズ
		void FlyingPhase();//射出後フェーズ



		void OnAdjust();

		void SetY(float y){SetPosition(_pos.x,y);}

		///y/xの比率を入力する
		///@param yrate Y/X比率
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
		///初期化イベント
		void OnInit(float& vx, float& vy);

		///テンション有効時イベント
		void OnActivateTension();


		///着地イベント
		void OnGround(float& x,float& y,float& vx,float& vy);

		///移動イベント
		void OnMove(float& x,float& y,float vx,float vy);

		///加速イベント
		void OnAccel(float& vx,float& vy,const float& ax,const float& ay);

		///ジャンプキー押されたイベント
		void OnPushJumpKey(float& vx,float& vy);

		///左キー押されたイベント
		void OnPushLeftKey(float& vx,float& vy);

		///右キー押されたイベント
		void OnPushRightKey(float& vx,float& vy);
};

