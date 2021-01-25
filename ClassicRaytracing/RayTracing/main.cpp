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

//�q���g�ɂȂ�Ǝv���āA�F�X�Ɗ֐���p�ӂ��Ă���܂���
//�ʂɂ��̊֐����g��Ȃ���΂����Ȃ��킯�ł��A����ɉ���Ȃ���΂����Ȃ��킯�ł�
//����܂���B���C�g���[�V���O���ł��Ă���΍\���܂���B

///���C(����)�ƕ��ʂ̓����蔻��
///@param ray (���_����X�N���[���s�N�Z���ւ̃x�N�g��)
///@param plane ����
///@hint ���C�͐��K�����Ƃ����ق����g���₷�����낤
bool IsHitRayAndObject(const Position3& eye, const Vector3& ray, const Plane& plane) {
	return -(ray.Normalized()* plane.normal) >= 0;
}

///���C(����)�Ƌ��̂̓����蔻��
///@param ray (���_����X�N���[���s�N�Z���ւ̃x�N�g��)
///@param sphere ��
///@param normal[out] ���̒��S�����_�ւ̃x�N�g��
///@param hitPos[out] ��_�̍��W
///@hint ���C�͐��K�����Ƃ����ق����g���₷�����낤
bool IsHitRayAndObject(const Position3& eye,const Vector3& ray,const Sphere& sp,Vector3& normal, Vector3& hitPos) {
	//���C�����K���ς݂ł���O��Łc
	//���_���狅�̒��S�ւ̃x�N�g��(����)�����܂�
	Vector3 tocenter=sp.pos-eye;

	//���S���王���ւ̓��ς��Ƃ�܂��˃x�N�g����
	float dot=Dot(tocenter,ray);

	//�����x�N�g���ƃx�N�g�����������āA���S����̐����������_�����߂܂�
	Vector3 raydash=ray*dot;
	
	Vector3 vertical = tocenter - raydash;//�����x�N�g��
	//���̐����̒��������a�ȓ����ǂ����Ń��C�����ɂ������Ă��邩��
	//���肵�܂�
	float sqmag = vertical.SQMagnitude();
	if (sqmag < sp.radius*sp.radius) {
		float w = sqrt(sp.radius*sp.radius - sqmag);
		hitPos = eye+ray*(dot - w);//��_
		normal = ray*(dot - w) - tocenter;//���S�����_�ւ̃x�N�g��
		return true;
	}
	else {
		return false;
	}
}

///���˃x�N�g��������ĕԂ�
///@param inVec ���˃x�N�g��
///@param normalVec ���K���ς݂̖@���x�N�g��
///@return ���˂��ꂽ���˃x�N�g��
Vector3 ReflectedVector(const Vector3& inVec, const Vector3& normalVec) {
	return inVec - normalVec*2 * (inVec*normalVec);
}

///0�`1��RGB���������ŐF��ł�
///@param x X���W
///@param y Y���W
///@param r ��(0�`1)
///@param g ��(0�`1)
///@param b ��(0�`1)
void
DrawPixelWithFloat(int x,int y,float r, float g, float b) {
	DrawPixel(x, y, GetColor(min(r * 0xff,0xff), min(g * 0xff,0xff), min(b * 0xff,0xff)));
}

///0�`1��RGB���������ŐF��ł�
///@param x X���W
///@param y Y���W
///@param r ��(0�`1)
///@param g ��(0�`1)
///@param b ��(0�`1)
void
DrawPixelDataFloat(ColorData_t& data, int x, int y, float r, float g, float b) {
	DrawPixelData(data,x, y, min(r * 0xff, 0xff), min(g * 0xff, 0xff), min(b * 0xff, 0xff));
}

///�\�ʍގ��Ɩ@���ƌ�����P���ɂ��̃I�u�W�F�N�g�ɂ����邻�̓_�̐F��Ԃ�
///@param object �I�u�W�F�N�g(�}�e���A���������Ă����)
///@param ray ���˃x�N�g��
///@param hitpos �Փ˓_
///@param normal �Փ˓_�ɂ����邻�̕��̖̂@���x�N�g��(���K���ς�)
///@param toLight ���C�g�����x�N�g��
Color GetBasicColor(const GeometryObject* object, const Vector3& ray,const Position3& hitpos, const Vector3& normal, const Vector3& toLight) {
	//�_�ɐF��t����
	
	Vector3 norm = normal.Normalized();

	//�����̔��˃x�N�g��(���K����)������
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

///�ċA�g���[�X����
///@param rayline ����(�x�N�g���{�n�_)
///@param objects ��������ׂ��I�u�W�F�N�g����
///@param exclusiveObject ���O���ׂ��I�u�W�F�N�g
///@param toLight ���������ւ̃x�N�g��
///@param limit �ċA���E
///@param miss ���ɂ�������Ȃ�����
Color RecursiveTrace(RayLine& rayline, std::vector<GeometryObject*>& objects, GeometryObject* exclusiveObject,Vector3& toLight,unsigned int limit,bool& miss) {
	pair<float, function<Color(void)> > zbuffunc = make_pair(FLT_MAX, []()->Color {return Color(0,0,0); });
	miss = false;
	//���ׂĂ̕��̂Ƃ̂����蔻�������
	for (GeometryObject* obj : objects) {
		if (exclusiveObject == obj) continue;
		Position3 hitpos;
		Vector3 normal;
		//�������Ă���
		if (obj->CheckHit(rayline, hitpos, normal)) {
			auto distance=(hitpos - rayline.start).Magnitude();
			//���͔̂��˂���H���Ȃ��H�����I�ɂ����H
			auto& material = obj->GetMaterial();
			if (limit > 0&&material.reflectance > 0.1f) {
				auto func= [=,&objects,&toLight]() {
					//���˂���Ȃ甽�˃x�N�g���ƏՓ˓_�����ɍċA����(�ċA���E���l������)
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
			} else {//���˂��Ȃ�
				auto func = [=]() {
					//���̂����̂܂ܕ`�悷��
					auto color = GetBasicColor(obj, rayline.vector, hitpos, normal, toLight);
					//���˂��Ȃ��Ȃ���������𒲂ׂĕ��̂̂ǂꂩ�Ɠ������Ă�����Â�����
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

///���C�g���[�V���O
///@param toLight ���C�g����
///@param eye ���_���W
///@param objects �������q�b�g����\���̂���I�u�W�F�N�g(����)
void RayTracing(ColorData_t& data,Vector3 toLight, const Position3& eye,std::vector<GeometryObject*>& objects) {
	
	toLight.Normalize();
	float ambient = 0.1f;
#pragma omp parallel for
	for (int y = 0; y < screen_height; ++y) {//�X�N���[���c����
#pragma omp parallel for
		for (int x = 0; x < screen_width; ++x) {//�X�N���[��������
			
			//�@���_�ƃX�N���[�����W���王���x�N�g�������
			Vector3 ray = Vector3(x - screen_width / 2, screen_height / 2 - y, 0) - eye;
			//�A���K�����Ƃ�
			ray.Normalize();

			//�ċA�g���[�X�֐����Ăяo��
			bool miss;
			auto c = RecursiveTrace(RayLine(eye,ray),objects,nullptr,toLight,5,miss);
			DrawPixelDataFloat(data,x, y, c.r, c.g, c.b);
			
		}
	}
}
///���C�g���[�V���O
///@param eye ���_���W
///@param sphere ���I�u�W�F�N�g(���̂��������ɂ���)
//void RayTracing(Vector3 toLight,const Position3& eye,const Sphere& sphere,const Plane& plane) {
//	toLight.Normalize();
//	float ambient = 0.1f;
//
//#pragma omp parallel for
//	for (int y = 0; y < screen_height; ++y) {//�X�N���[���c����
//		int threadNum = omp_get_thread_num();
//		std::cout << "thread_num=" << threadNum << std::endl;
//		for (int x = 0; x < screen_width; ++x) {//�X�N���[��������
//			//�@���_�ƃX�N���[�����W���王���x�N�g�������
//			Vector3 ray=Vector3(x-screen_width/2,screen_height/2-y,0)-eye;
//			//�A���K�����Ƃ�
//			ray.Normalize();
//			//�BIsHitRay�֐���True�������甒���h��Ԃ�
//			Vector3 normal = Vector3();//�@���󂯎��p
//			Position3 hitPos = Vector3();//��_�󂯎��p
//			//���h��Ԃ���DrawPixel�Ƃ����֐����g���B
//			//if(IsHitRayAndObject(eye,ray, sphere,normal,hitPos)){
//			if (sphere.CheckHit(RayLine(eye,ray),hitPos,normal)) {
//				//�_�ɐF��t����
//				//���K���ςݖ@��
//				normal.Normalize();
//
//				//���˃x�N�g��(���K����)������
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
//				//���˃x�N�g���H�����ɓ����邩�H
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
//					//�������������Ă݂�
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
	SetMainWindowText(_T("�ÓT�I���C�g���[�V���O"));
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

	//�����������_
	//�����������ƂȂ��Ă��܂��B
	//�E����W�n�Ȃ̂Ŏ�O���v���X�ł�
	//�X�N���[����0�̈ʒu�ɂ���z��ł�
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