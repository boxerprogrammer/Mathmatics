#include<dxlib.h>
#include<cmath>
#include<vector>
#include<functional>
#include<iostream>
#include<omp.h>
#include<cassert>
#include"Geometry.h"

using namespace std;

constexpr int screen_width = 640;
constexpr int screen_height = 480;

struct ColorU8 {
	UINT8 r, g, b, a;
	ColorU8() :r(0), g(0), b(0), a(0) {}
	ColorU8(UINT8 inr, UINT8 ing, UINT8 inb, UINT8 ina) :r(inr), g(ing), b(inb), a(ina) {};
	void SetColor(UINT8 inr, UINT8 ing, UINT8 inb, UINT8 ina) {
		r = inr;
		g = ing;
		b = inb;
		a = ina;
	}
};
using ColorData_t = vector<ColorU8>;
void DrawPixelData(ColorData_t& data, int x, int y, UINT8 r, UINT8 g, UINT8 b) {
	assert(data.size() == screen_width * screen_height);
	assert(0 <= x && x < screen_width);
	assert(0 <= y && y < screen_height);
	data[(size_t)x + (size_t)screen_width * (size_t)y].SetColor(r, g, b, 255);
}

//ヒントになると思って、色々と関数を用意しておりますが
//別にこの関数を使わなければいけないわけでも、これに沿わなければいけないわけでも
//ありません。レイトレーシングができていれば構いません。

///レイ(光線)と平面の当たり判定
///@param ray (視点からスクリーンピクセルへのベクトル)
///@param plane 平面
///@hint レイは正規化しといたほうが使いやすいだろう
bool IsHitRayAndObject(const Position3& eye, const Vector3& ray, const Plane& plane) {
	return -(ray.Normalized()* plane.normal) >= 0;
}

///レイ(光線)と球体の当たり判定
///@param ray (視点からスクリーンピクセルへのベクトル)
///@param sphere 球
///@param normal[out] 球の中心から交点へのベクトル
///@param hitPos[out] 交点の座標
///@hint レイは正規化しといたほうが使いやすいだろう
bool IsHitRayAndObject(const Position3& eye,const Vector3& ray,const Sphere& sp,Vector3& normal, Vector3& hitPos) {
	//レイが正規化済みである前提で…
	//視点から球体中心へのベクトル(視線)を作ります
	Vector3 tocenter=sp.pos-eye;

	//中心から視線への内積をとります⇒ベクトル長
	float dot=Dot(tocenter,ray);

	//視線ベクトルとベクトル長をかけて、中心からの垂線下した点を求めます
	Vector3 raydash=ray*dot;
	
	Vector3 vertical = tocenter - raydash;//垂線ベクトル
	//その垂線の長さが半径以内かどうかでレイが球にあたっているかを
	//判定します
	float sqmag = vertical.SQMagnitude();
	if (sqmag < sp.radius*sp.radius) {
		float w = sqrt(sp.radius*sp.radius - sqmag);
		hitPos = eye+ray*(dot - w);//交点
		normal = ray*(dot - w) - tocenter;//中心から交点へのベクトル
		return true;
	}
	else {
		return false;
	}
}

///反射ベクトルを作って返す
///@param inVec 入射ベクトル
///@param normalVec 正規化済みの法線ベクトル
///@return 反射された入射ベクトル
Vector3 ReflectedVector(const Vector3& inVec, const Vector3& normalVec) {
	return inVec - normalVec*2 * (inVec*normalVec);
}

///0～1のRGB浮動小数で色を打つ
///@param x X座標
///@param y Y座標
///@param r 赤(0～1)
///@param g 緑(0～1)
///@param b 青(0～1)
void
DrawPixelWithFloat(int x,int y,float r, float g, float b) {
	DrawPixel(x, y, GetColor(min(r * 0xff,0xff), min(g * 0xff,0xff), min(b * 0xff,0xff)));
}

///0～1のRGB浮動小数で色を打つ
///@param x X座標
///@param y Y座標
///@param r 赤(0～1)
///@param g 緑(0～1)
///@param b 青(0～1)
void
DrawPixelDataFloat(ColorData_t& data, int x, int y, float r, float g, float b) {
	DrawPixelData(data,x, y, min(r * 0xff, 0xff), min(g * 0xff, 0xff), min(b * 0xff, 0xff));
}

///表面材質と法線と光から単純にそのオブジェクトにおけるその点の色を返す
///@param object オブジェクト(マテリアルも入っているよ)
///@param ray 入射ベクトル
///@param hitpos 衝突点
///@param normal 衝突点におけるその物体の法線ベクトル(正規化済み)
///@param toLight ライト方向ベクトル
Color GetBasicColor(const GeometryObject* object, const Vector3& ray,const Position3& hitpos, const Vector3& normal, const Vector3& toLight) {
	//点に色を付ける
	
	Vector3 norm = normal.Normalized();

	//光線の反射ベクトル(正規化済)をつくる
	Vector3 light(-toLight);
	Vector3 reflectV = ReflectedVector(light,norm);
	reflectV.Normalize();

	auto mat = object->GetMaterial();

	float dotNL = norm*toLight;
	float specularB = 0;
	Vector3 normalizedInvRay = (-ray).Normalized();
	if (dotNL > 0 && mat.specular.r>0.0f) {
		specularB = max(pow(reflectV*normalizedInvRay, mat.specularity), 0.0f)*mat.specular.r;
	}

	float specular[3] = { specularB,specularB,specularB };
	
	float albedo[3] = { mat.diffuse.r ,mat.diffuse.g,mat.diffuse.b };

	if (mat.pattern == Pattern::checkered) {
		int thresholdX = (int)hitpos.x / 20;
		int thresholdZ = (int)hitpos.z / 20;
		int threshold = thresholdX + thresholdZ;
		threshold = threshold < 0 ? threshold + 2 : threshold;
		for (auto& a : albedo) {
			if (abs(threshold % 2) == (hitpos.x*hitpos.z > 0)) {
				a *= 0.7;
			}
		}
	}

	float ambient = object->GetMaterial().ambient.r;
	float diffuseB = min(max(norm*toLight, 0.0f) + ambient, 1.0f);
	float diffuse[3] = { diffuseB*albedo[0],diffuseB*albedo[1],diffuseB*albedo[2] };
	float color[3] = { min(diffuse[0] + specular[0] + ambient,1),
		min(diffuse[1] + specular[1] + ambient,1),
		min(diffuse[2] + specular[2] + ambient,1) };
	
	return Color(color[0], color[1], color[2]);
}

///再帰トレースする
///@param rayline 直線(ベクトル＋始点)
///@param objects 判定を取るべきオブジェクトたち
///@param exclusiveObject 除外すべきオブジェクト
///@param toLight 光源方向へのベクトル
///@param limit 再帰限界
///@param miss 何にも当たらなかった
Color RecursiveTrace(RayLine& rayline, std::vector<GeometryObject*>& objects, GeometryObject* exclusiveObject,Vector3& toLight,unsigned int limit,bool& miss) {
	pair<float, function<Color(void)> > zbuffunc = make_pair(FLT_MAX, []()->Color {return Color(0,0,0); });
	miss = false;
	//すべての物体とのあたり判定を見る
	for (GeometryObject* obj : objects) {
		if (exclusiveObject == obj) continue;
		Position3 hitpos;
		Vector3 normal;
		//当たっている
		if (obj->CheckHit(rayline, hitpos, normal)) {
			auto distance=(hitpos - rayline.start).Magnitude();
			//物体は反射する？しない？部分的にそう？
			auto& material = obj->GetMaterial();
			if (limit > 0&&material.reflectance > 0.1f) {
				auto func= [=,&objects,&toLight]() {
					//反射するなら反射ベクトルと衝突点を元に再帰する(再帰限界を考慮して)
					auto ray = ReflectedVector(rayline.vector, normal).Normalized();
					auto color = GetBasicColor(obj, rayline.vector, hitpos, normal, toLight);
					bool miss;
					auto refcol=RecursiveTrace(RayLine(hitpos, ray), objects, obj, toLight, limit - 1, miss);
					auto blend = miss ? 0.0f: material.reflectance;
					color = color*(1.0f-blend)+ refcol*blend;
					return color;
				};
				if (distance < zbuffunc.first) {
					zbuffunc = make_pair(distance, func);
				}
			} else {//反射しない
				auto func = [=]() {
					//物体をそのまま描画する
					auto color = GetBasicColor(obj, rayline.vector, hitpos, normal, toLight);
					//反射しないなら光源方向を調べて物体のどれかと当たっていたら暗くする
					for (auto object : objects) {
						if (obj == object)continue;
						Position3 hitpos2;
						Vector3 normal2;
						if (object->CheckHit(RayLine(hitpos, toLight), hitpos2, normal2)) {
							color *= 0;
							break;
						}
					}
					return color;
				};
				if (distance < zbuffunc.first) {
					zbuffunc = make_pair(distance, func);
				}
			}
		}
	}
	if ( zbuffunc.first==FLT_MAX) {
		if (limit == 5) {
			return Color(0.5, 1, 1);
		}
		else {
			miss = true;
			return Color(1, 1, 1);
		}
	}
	else {
		return zbuffunc.second();
	}
}

///レイトレーシング
///@param toLight ライト方向
///@param eye 視点座標
///@param objects 視線がヒットする可能性のあるオブジェクト(複数)
void RayTracing(ColorData_t& data,Vector3 toLight, const Position3& eye,std::vector<GeometryObject*>& objects) {
	
	toLight.Normalize();
	float ambient = 0.1f;
#pragma omp parallel for
	for (int y = 0; y < screen_height; ++y) {//スクリーン縦方向
#pragma omp parallel for
		for (int x = 0; x < screen_width; ++x) {//スクリーン横方向
			
			//①視点とスクリーン座標から視線ベクトルを作る
			Vector3 ray = Vector3(x - screen_width / 2, screen_height / 2 - y, 0) - eye;
			//②正規化しとく
			ray.Normalize();

			//再帰トレース関数を呼び出す
			bool miss;
			auto c = RecursiveTrace(RayLine(eye,ray),objects,nullptr,toLight,5,miss);
			DrawPixelDataFloat(data,x, y, c.r, c.g, c.b);
			
		}
	}
}
///レイトレーシング
///@param eye 視点座標
///@param sphere 球オブジェクト(そのうち複数にする)
//void RayTracing(Vector3 toLight,const Position3& eye,const Sphere& sphere,const Plane& plane) {
//	toLight.Normalize();
//	float ambient = 0.1f;
//
//#pragma omp parallel for
//	for (int y = 0; y < screen_height; ++y) {//スクリーン縦方向
//		int threadNum = omp_get_thread_num();
//		std::cout << "thread_num=" << threadNum << std::endl;
//		for (int x = 0; x < screen_width; ++x) {//スクリーン横方向
//			//①視点とスクリーン座標から視線ベクトルを作る
//			Vector3 ray=Vector3(x-screen_width/2,screen_height/2-y,0)-eye;
//			//②正規化しとく
//			ray.Normalize();
//			//③IsHitRay関数がTrueだったら白く塗りつぶす
//			Vector3 normal = Vector3();//法線受け取り用
//			Position3 hitPos = Vector3();//交点受け取り用
//			//※塗りつぶしはDrawPixelという関数を使う。
//			//if(IsHitRayAndObject(eye,ray, sphere,normal,hitPos)){
//			if (sphere.CheckHit(RayLine(eye,ray),hitPos,normal)) {
//				//点に色を付ける
//				//正規化済み法線
//				normal.Normalize();
//
//				//反射ベクトル(正規化済)をつくる
//				Vector3 light(-toLight);
//				Vector3 reflectV =light - normal*2 * (light*normal);
//				reflectV.Normalize();
//
//				float dotNL = normal*toLight;
//				float specularB = 0;
//				if (dotNL >= 0) {
//					specularB=max(pow(reflectV*(-ray), 20.0f), 0.0f);
//				}
//
//				specularB += pow(1 - ((-ray)*normal),20);
//
//				float specular[3] = { specularB,specularB,specularB };
//
//				float albedo[3] = {1.0f,0.7f,0.7f};
//
//				float diffuseB= min(max(normal*toLight, 0.0f) + ambient, 1.0f);
//				float diffuse[3] = { diffuseB*albedo[0],diffuseB*albedo[1],diffuseB*albedo[2] };
//				float color[3] = {min(diffuse[0]+specular[0]+ambient,1),
//					min(diffuse[1]+specular[1] + ambient,1),
//					min(diffuse[2]+specular[2]+ambient,1) };
//				
//				//反射ベクトル？が床に当たるか？
//				ray=ReflectedVector(ray, normal);
//				auto& plnorm = plane.normal;
//				if (-ray*plnorm > 0) {
//					if (plane.CheckHit(RayLine(hitPos,ray),hitPos,normal)) {
//						float albedo[3] = { 0.7f,0.7f,1.0f };
//						float t = (plane.offset - hitPos*plnorm) / (ray*plnorm);
//						Position3 pos = hitPos + ray*t;
//						int thresholdX = (int)pos.x / 20;
//						int thresholdZ = (int)pos.z / 20;
//						int threshold = thresholdX + thresholdZ;
//						threshold = threshold < 0 ? threshold + 2 : threshold;
//						float dark = 0.7f;
//						if (abs(threshold % 2) == (pos.x*pos.z>0)) {
//							DrawPixelWithFloat(x, y, color[0]*albedo[0], color[1] * albedo[1], color[2] * albedo[2]);
//						}
//						else {
//							DrawPixelWithFloat(x, y, color[0] * albedo[0] * dark, color[1] * albedo[1] * dark, color[2] * albedo[2] * dark);
//						}	
//						continue;
//					}
//				}
//				DrawPixelWithFloat(x, y, color[0], color[1], color[2]);
//			}
//			else {
//				Position3 intersect;
//				Vector3 normal;
//				if(plane.CheckHit(RayLine(eye,ray),intersect,normal)){
//					float albedo[3] = { 0.7f,0.7f,1.0f };
//					//float t = (plane.offset - eye*plane.normal) / (ray*plane.normal);
//					//Position3 pos= eye + (ray*t);
//					int thresholdX = (int)intersect.x / 20;
//					int thresholdZ = (int)intersect.z / 20;
//					int threshold = thresholdX + thresholdZ;
//					threshold = threshold < 0 ? threshold + 2 : threshold;
//					float dark = 0.7f;
//
//					//光線方向を見てみる
//					if (sphere.CheckHit(RayLine(intersect, toLight.Normalized()), hitPos,normal)) {
//						for (auto& al : albedo) {
//							al *= 0.5f;
//						}
//					}
//
//					if (abs(threshold % 2) == (intersect.x*intersect.z>0)) {
//						DrawPixelWithFloat(x, y, albedo[0] , albedo[1] , albedo[2]);
//					}
//					else {
//						DrawPixelWithFloat(x, y, albedo[0]*dark, albedo[1] * dark, albedo[2] * dark);
//					}
//				}
//			}
//		}
//	}
//}


int main() {
	vector<ColorU8> data(screen_width * screen_height);
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, 32);
	SetMainWindowText(_T("古典的レイトレーシング"));
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	BASEIMAGE bimg = {};
	bimg.GraphData = data.data();
	bimg.ColorData.ColorBitDepth = 32;
	bimg.ColorData.BlueLoc = 8;
	bimg.ColorData.GreenLoc = 16;
	bimg.ColorData.AlphaLoc = 24;
	bimg.ColorData.RedLoc = 0;
	bimg.ColorData.PixelByte = 4;
	bimg.ColorData.AlphaWidth = 8;
	bimg.ColorData.RedWidth = 8;
	bimg.ColorData.BlueWidth = 8;
	bimg.ColorData.GreenWidth = 8;
	bimg.ColorData.RedMask = 0x000000ff;
	bimg.ColorData.BlueMask = 0x0000ff00;
	bimg.ColorData.GreenMask = 0x00ff0000;
	bimg.ColorData.AlphaMask = 0xff000000;
	bimg.ColorData.Format = DX_BASEIMAGE_FORMAT_NORMAL;
	bimg.Width = screen_width;
	bimg.Height = screen_height;
	bimg.Pitch = screen_width * sizeof(data[0]);

	auto gH = CreateGraphFromBaseImage(&bimg);

	//第一引数が視点
	//第二引数が球となっています。
	//右手座標系なので手前がプラスです
	//スクリーンは0の位置にある想定です
	char keystate[256];
	Position3 eye = Vector3(0, 0, 500);

	std::vector<GeometryObject*> objects;
	objects.push_back(new Sphere(100, Position3(0, -100, -100), 
		Material(Color(1.0f, 0.7f, 0.7f), Color(1.0f, 1.0f, 1.0f), Color(0.0f, 0.0f, 0.0f), 20.0f, 0.9f)));
	objects.push_back(new Sphere(100, Position3(200, 0, -200), 
		Material(Color(0.5f, 0.7f, 1.0f), Color(1.0f, 1.0f, 1.0f), Color(0.0f, 0.0f, 0.0f), 10.0f, 0.5f)));
	objects.push_back(new Sphere(100, Position3(-100, 50, -300), 
		Material(Color(1.0f, 1.0f, 0.7f), Color(1.0f, 1.0f, 1.0f), Color(0.0f, 0.0f, 0.0f), 10.0f, 0.7f)));
	objects.push_back(new Plane(Vector3(0, 1, 0), -200, 
		Material(Color(0.7f, 1.0f, 0.7f), Color(0.0, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), 10.0f, 0.0f, Pattern::checkered)));

	Sphere* sp = (Sphere*)(objects[0]);
	Vector3 toLight(-1, 1, 1);
	while(ProcessMessage()==0){
		ClearDrawScreen();
		DxLib::GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_4]) {
			toLight.x -= 0.05f;
		}
		if (keystate[KEY_INPUT_6]) {
			toLight.x += 0.05f;
		}
		if (keystate[KEY_INPUT_2]) {
			toLight.y -= 0.05f;
		}
		if (keystate[KEY_INPUT_8]) {
			toLight.y += 0.05f;
		}

		if(keystate[KEY_INPUT_RIGHT]){
			eye.x+=5;
		}
		if (keystate[KEY_INPUT_LEFT]) {
			eye.x-=5;
		}
		if (keystate[KEY_INPUT_UP]) {
			eye.y+=5;
		}
		if (keystate[KEY_INPUT_Z]) {
			eye.z+=5;
		}
		if (keystate[KEY_INPUT_X]) {
			eye.z -= 5;
		}
		
		if (keystate[KEY_INPUT_W]) {
			sp->pos.y += 5;
		}
		if (keystate[KEY_INPUT_A]) {
			sp->pos.x -= 5;
		}
		if (keystate[KEY_INPUT_S]) {
			sp->pos.y -= 5;
		}
		if (keystate[KEY_INPUT_D]) {
			sp->pos.x += 5;
		}

		DrawBox(0, 0, screen_width, screen_height, GetColor(0.5 * 255, 0.8 * 255, 0.8 * 255), true);
		
		RayTracing(data,toLight,eye,objects);
		ReCreateGraphFromBaseImage(&bimg, gH);
		DrawGraph(0, 0, gH, false);
		auto fps = GetFPS();
		DrawFormatString(10, 10, 0xffffff, L"%f", fps);
		ScreenFlip();
	}
	DxLib_End();
}