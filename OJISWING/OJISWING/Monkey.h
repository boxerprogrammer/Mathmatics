#pragma once
#include"Character.h"
class Monkey : public Character
{
	public:
		Monkey(const char* filename,int xnum,int ynum);
		~Monkey(void);
		///毎フレーム呼び出す処理
		void Update();
		
	private:
		float _vx,_vy;
		int _handle;
		bool _isPushJump;
		///初期化イベント
		void OnInit(float& vx, float& vy);

		///着地イベント
		void OnGround(int& x,int& y,float& vx,float& vy);

		///移動イベント
		void OnMove(int& x,int& y,float vx,float vy);

		///加速イベント
		void OnAccel(float& vx,float& vy);

		///ジャンプキー押されたイベント
		void OnPushJumpKey(float& vx,float& vy);

		///左キー押されたイベント
		void OnPushLeftKey(float& vx,float& vy);

		///右キー押されたイベント
		void OnPushRightKey(float& vx,float& vy);
};

