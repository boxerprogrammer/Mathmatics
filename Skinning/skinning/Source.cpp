#include<DxLib.h>
#include<vector>
#include<array>
#include"Geometry.h"

constexpr float br1 = 5.0f;//�\���p�́u���a�v
constexpr float br2 = 10.0f;//�\���p�́u���a�v
constexpr float dt = 15.0f;// �{�[���Ђ��`�̋���

using namespace std;

class Bone {
private:
	Position2f bpos_;//��_
	Vector2f vec_;//�x�N�g��(�����̂�)
	float length_ = 1.0f;
	UINT32 col_=0xffffff;
public:
	Bone(const Position2f& pos, const const Vector2f& vec) :bpos_(pos) {
		length_ =vec.Length();
		vec_ = vec/length_;
	}
	void Draw() {
		DrawCircleAA(bpos_.x, bpos_.y, br1,16, col_, 1, 2.0f);//�~1
		DrawCircleAA(bpos_.x, bpos_.y, br2,16, col_, 0, 2.0f);//�~2
		auto R=vec_.R();
		Position2f p[4] = {
			bpos_,
			bpos_ + (vec_ - R) * dt,
			bpos_ + vec_ *  max(length_,2.0*dt),
			bpos_ + (vec_ + R) * dt };
		DrawQuadrangleAA(
			p[0].x, 
			p[0].y, 
			p[1].x, 
			p[1].y,
			p[2].x,
			p[2].y, 
			p[3].x,
			p[3].y, 
			0xffffff, false,1.0f);
	}
};

vector<Bone> 
BuildBones(const vector<Position2f>& posses) {
	vector<Bone> ret;
	ret.reserve(posses.size() - 1);
	for (size_t i = 0; i < posses.size() - 1; ++i) {
		ret.emplace_back(posses[i],posses[i+1]-posses[i]);
	}
	return ret;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	DxLib::SetMainWindowText("�X�L�j���O");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);


	vector<Position2f> posses = {
		{100.0f, 250.0f},
		{350.0f,250.0f},
		{600.0f,250.0f} };

	auto bones = BuildBones(posses);


	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		for (auto& bone : bones) {
			bone.Draw();
		}
		

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}