#include<DxLib.h>
#include<vector>
#include<list>
#include<algorithm>
#include<cmath>
#include"Ball.h"
#include"Cue.h"
#include"HitEffect.h"
#include"ScreenShaker.h"

int _ballCollisionSound = 0;

std::list<HitEffect> _hiteffectList;
int _hitEffectH = 0;

//※まず、ボール自身の挙動を記述するためBall.cppのUpdate関数からいじってください。


//※便利構造体としてVector2というのを用意しています。
//Normalize()で正規化し、Normalized()で正規化済みベクトルを返します。
//Dot(a,b)は内積値を返します。これらを利用してビリヤードゲームを完成させてください。
//
//最低限、ボール同士の衝突判定と、衝突による速度ベクトルの受け渡しができていることが
//評価要件です。
//
//とりあえずいじるのはbilliard.cppだけで十分なふうに作っていますが、できる人は色々といじってOK
//
//できる人はボールを増やしたり、よりゲーム性を上げたり、工夫してカッコよくしたりしてください
//

///円と、円のあたり判定
///@param a 円A
///@param b 円B
///@retval true 当たり
///@retval false はずれ
bool IsHit(const Circle& a,const Circle& b) {
	//当たってたらtrue、外れてたらfalseを返せばいいだけ


	float xdiff = b.pos.x - a.pos.x;//X方向差分
	float ydiff = b.pos.y - a.pos.y;//Y方向差分
	float totalRadius = a.radius + b.radius;//二つの円の半径を足したもの

	//③あたり判定のコードを書いてください
	return totalRadius*totalRadius>=xdiff*xdiff + ydiff*ydiff;//ここはOCでは消します

	bool ret = false;
	if (false) {//③ここに当たるための条件を書いてください
		//③当たる条件に合致したらretにtrueを代入してください
		//円の当たり判定というのは、
		//お互いの円の半径を足したものより中心点間の距離が近ければ当たっている
		//そうでなければ当たっていないというものです。

	}

	return ret;
}

///ボールの衝突チェック
///@param a ボールA
///@param b ボールB
///@remarks ボールAとボールBの衝突をチェックし、
///もし衝突していれば速度ベクトルの受け渡しを行います
void CheckBallCollision(Ball& a,Ball& b) {
	if (!IsHit(a.GetCircle(), b.GetCircle())) {
		return;//当たってなかったらさっさとリターン()
	}

	//当たっていたら速度ベクトルの受け渡しを行ってください
	//Aの法線方向成分をBに、Bの法線方向成分をAに渡してください

	//衝突したあとの衝突元と衝突先の速度ベクトルを計算していきます。
	//１．衝突地点での法線方向を計算
	//２．１のベクトルを正規化
	//３．Aの速度ベクトルの法線方向成分をBに(内積を使用)
	//		→Bにあげた分のベクトルは引いておくこと
	//４．Bの速度ベクトルの逆法線方向成分をAに(内積を使用)
	//		→Aにあげたぶんのベクトルは引いておくこと
	//※正規化はNormalize関数、内積はDot関数を使用できます
	
	//④法線方向のベクトルを計算
	Vector2 normal= b.GetCircle().pos - a.GetCircle().pos;
	//それぞれの中心点を引き算してください。それぞれの中心点はGetCircle().posで得られます。

	float diff = normal.Magnitude();
	//⑨補正用にめり込み量を計算…最後、時間が余ったらやる
	diff=a.GetRadius() + b.GetRadius() - diff;


	if (diff >= 1.0f) {
		//当たってるので音を鳴らすし、エフェクトを出す
		DxLib::ChangeVolumeSoundMem(min(255, (int)(diff * 80)), _ballCollisionSound);
		DxLib::PlaySoundMem(_ballCollisionSound, DX_PLAYTYPE_BACK);
		_hiteffectList.push_back(HitEffect(_hitEffectH,(a.GetCircle().pos + b.GetCircle().pos)*0.5f,(diff*80.0f/256.0f)));
	}


	//⑤-1衝突後の動きを計算します
	//計算の都合上、法線ベクトルの大きさを１にしておきます。
	//大きさを１にするには「ベクトルの正規化」というのを行います。
	//本当はベクトルの大きさを√(x*x+y*y)で計算して、その大きさでベクトルのそれぞれの
	//成分を割って作るのですが、一部の人には難しいかもしれないのでNormalizeという関数を
	//作っていますので、Normalize()と書いてください。
	normal.Normalize();

	//⑤-2めり込んでいる場合…aの座標を補正する
	//ゲームの世界は現実と違うため、計算上２つの物体が重なることがあります。
	//当然のことながらあり得ない事象のため、ここは内部の処理で重なりを補正します
	//補正と言う言葉が難しいかもしれませんが、要は重なってない状況にするために
	//それぞれのボールの座標を「めり込み反対方向」に移動させておきます。
	a.SetPosition(a.GetPosition() - normal*diff*0.5f);
	b.SetPosition(b.GetPosition() + normal*diff*0.5f);


	//⑤-3それぞれの速度ベクトルと法線ベクトルの内積をとります。
	Vector2 velA = a.GetVelocity();
	Vector2 velB = b.GetVelocity();
	
	float dotA = Dot(velA,normal);
	float dotB = Dot(velB, -normal);
	//なお、法線ベクトルはだいぶ↑で計算したnormalに入っている状態です。
	//内積はDotという関数で計算します。この計算で得られた値が
	//法線方向の速さの大きさに当たります



	//⑥↑で得られた法線方向大きさからそれぞれの速度ベクトルを計算
	//そしてそれをAddVelocity関数でそれぞれのボールに伝えてあげます。
	b.AddVelocity(normal*dotA);
	a.AddVelocity(-normal*dotA);

	//dotAにAからBに対して移動する速度ベクトルの大きさが入っているため
	//それに法線方向ベクトルのnormalを書けることで、Bに奪われる速度ベクトルが分かります。
	//なので、b.AddVelocity関数で、Bに速度を加算してあげます。
	//また、a.AddVelocity関数で、Bに奪われた速度にマイナスをかけることで残ったベクトルを
	//Aのベクトルとすることになります。ただしこの処理はBが止まっている時にのみ有効です。

	//⑦お互いに動いている場合は↓の処理も必要になります。
	b.AddVelocity(normal*dotB);
	a.AddVelocity(-normal*dotB);


}

//メイン関数
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
//int main() {
	
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText(_T("ビリヤードを作ってみよう"));
	DxLib_Init();
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	const int boardWidth = 550;
	const int boardHeight = 350;

	std::vector<Ball> balls;
	balls.push_back(Ball(0xdddddd,70,200,"self"));//自分ボール

	balls.push_back(Ball(0xeeee33,300,200));//1番ボール(黄色)

	//⑧ボールを増やしてみる
	balls.push_back(Ball(0x4444ff, 319, 188));//2番ボール(青色)
	balls.push_back(Ball(0xff4444, 319, 212));//3番ボール(赤色)
	balls.push_back(Ball(0xff44ff, 338, 176));//4番ボール(紫色)
	balls.push_back(Ball(0xffaa44, 338, 200));//5番ボール(オレンジ色)
	balls.push_back(Ball(0x00ff00, 338, 224));//6番ボール(緑色)
	balls.push_back(Ball(0xaa0000, 357, 164));//7番ボール(あずき色)
	balls.push_back(Ball(0x444444, 357, 188));//8番ボール(黒色)
	balls.push_back(Ball(0xffaaaa, 357, 212));//9番ボール(ピンク色)
	balls.push_back(Ball(0x88eeee, 357, 236));//10番ボール(水色)

	auto cueH=LoadGraph(L"Q.png");
	auto gaugeH = LoadGraph(L"gauge.png");
	auto gaugeRH = LoadGraph(L"gauge_r.png");
	//auto kiminotanH = LoadGraph(L"kiminota-n.png");
	
	_ballCollisionSound = LoadSoundMem(L"billiard-ball1.mp3",6);
	auto _ballInHoleSound = LoadSoundMem(L"billiard-pocket1.mp3");

	auto gameoverH = LoadGraph(L"gameover.png");
	_hitEffectH = LoadGraph(L"hiteffect.png");

	Cue cue(balls[0],cueH);

	bool isGameOver = false;

	bool leftClickTrigger = false;

	int shotCount = 0;

	{
		int powerGauge = 0;
		bool chargingGo = false;
		ScreenShaker shaker;

		while (ProcessMessage() == 0) {
			shaker.PrepareShake();

			ClearDrawScreen();
			//DrawGraph(0, 422, kiminotanH, true);
			if (DxLib::CheckHitKey(KEY_INPUT_R)) {
				for (auto& ball : balls) {
					StopMusic();
					ball.Reset();
					isGameOver = false;
					shotCount = 0;
				}
			}
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) {
				if (!leftClickTrigger) {
					powerGauge = 0;
					chargingGo = true;
				}
				else {
					if (chargingGo) {
						powerGauge = min(powerGauge+4,590);
					}
				}
				leftClickTrigger = true;

			}
			else {
				if (leftClickTrigger&&chargingGo) {
					int mx, my;
					GetMousePoint(&mx, &my);
					Vector2 target(mx, my);
					cue.Shot(target, static_cast<float>(powerGauge)*1.5f);
					++shotCount;
				}
				chargingGo = false;
				leftClickTrigger = false;
			}
			if (DxLib::CheckHitKey(KEY_INPUT_ESCAPE)) {
				if (MessageBox(nullptr, L"終了しますか？", L"処す？処す？", MB_OKCANCEL | MB_ICONQUESTION) == IDOK) {
					break;
				}
			}
			//ビリヤード台描画
			DrawBox(50 - 22, 50 - 22, boardWidth + 50 + 22, boardHeight + 50 + 22, 0x00aa00, true);
			DrawBox(50, 50, boardWidth + 50, boardHeight + 50, 0x000000, false);
			DrawCircle(50, 50, 10, 0x000000);
			DrawCircle(boardWidth + 50, 50, 10, 0x000000);
			DrawCircle(50, boardHeight + 50, 10, 0x000000);
			DrawCircle(boardWidth + 50, boardHeight + 50, 10, 0x000000);
			DrawBox(boardWidth / 2 + 50 - 12, 35, boardWidth / 2 + 50 + 12, 55, 0x000000, true);
			DrawBox(boardWidth / 2 + 50 - 12, boardHeight + 45, boardWidth / 2 + 50 + 12, boardHeight + 65, 0x000000, true);

			const float left_wall = 50;
			const float right_wall = boardWidth + 50;
			const float top_wall = 50;
			const float bottom_wall = boardHeight + 50;

			//穴あたり判定
			for (auto& ball : balls) {
				if (!ball.IsAvailable())continue;
				if (IsHit(ball.GetCircle(), Circle(10, Position2(50, 50))) ||//左上
					IsHit(ball.GetCircle(), Circle(10, Position2(boardWidth + 50, 50))) ||//右上
					IsHit(ball.GetCircle(), Circle(10, Position2(50, boardHeight + 50))) ||//左下
					IsHit(ball.GetCircle(), Circle(10, Position2(boardWidth + 50, boardHeight + 50))) ||//右下
					IsHit(ball.GetCircle(), Circle(10, Position2(boardWidth / 2 + 50, 45))) ||
					IsHit(ball.GetCircle(), Circle(10, Position2(boardWidth / 2 + 50, boardHeight + 55)))) {
					if (ball.GetTag() == "self") {
						DxLib::PlayMusic(L"gameover.wav", DX_PLAYTYPE_BACK);
						isGameOver = true;
						shaker.Shake();
					}
					ball.Kill();
					DxLib::PlaySoundMem(_ballInHoleSound, DX_PLAYTYPE_BACK);
				}
			}

			//玉同士あたり判定(総当たり)
			for (int i = 0; i < balls.size() - 1; ++i) {
				if (!balls[i].IsAvailable())continue;
				for (int j = i + 1; j < balls.size(); ++j) {
					if (!balls[j].IsAvailable())continue;
					CheckBallCollision(balls[i], balls[j]);
				}
			}

			//壁当たり
			for (auto& ball : balls) {
				if (!ball.IsAvailable())continue;
				if (abs(ball.GetPosition().x - left_wall) <= ball.GetRadius() ||
					abs(ball.GetPosition().x - right_wall) <= ball.GetRadius()) {
					ball.ReverseX();
				}
				if (abs(ball.GetPosition().y - top_wall) <= ball.GetRadius() ||
					abs(ball.GetPosition().y - bottom_wall) <= ball.GetRadius()) {
					ball.ReverseY();
				}
			}

			bool areAllBallsStopped = true;
			for (auto& ball : balls) {
				ball.Update();
				if (areAllBallsStopped&&ball.IsAvailable() && ball.GetVelocity().Magnitude() > 0.1f) {
					areAllBallsStopped = false;
				}
			}
			if (areAllBallsStopped) {
				cue.OnClickable();
			}
			cue.Draw();
			DrawFormatString(50, 8, 0xffffff, L"打ち込んだ回数：%d", shotCount);
			DrawFormatString(300, 8, 0xffffff, L"落とした玉の数：%d", std::count_if(balls.begin(), balls.end(), [](Ball& b) {return !b.IsAvailable(); }));

			for (auto& h : _hiteffectList) {
				h.Draw();
			}
			auto it = std::remove_if(_hiteffectList.begin(), _hiteffectList.end(), [](HitEffect& he) {return !he.IsAvailable(); });
			_hiteffectList.erase(it, _hiteffectList.end());

			shaker.UpdateShake();

			if (isGameOver) {
				DrawGraph(0, 0, gameoverH, true);
			}

			DrawBox(30, 440, 620, 460, 0xffffff, false);
			DrawBox(31, 441, min(31+powerGauge, 619), 459, 0xffff00, true);
			DrawExtendGraph(31, 441, 619, 459, gaugeRH, false);
			DrawModiGraph(31, 441, min(31 + powerGauge, 619), 441,
				min(31 + powerGauge, 619), 459, 31, 459, gaugeH, false);

			ScreenFlip();
		}

	}
	
	DxLib::DxLib_End();
}