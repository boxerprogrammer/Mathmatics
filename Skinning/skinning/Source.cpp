#include<DxLib.h>
#include<vector>
#include<array>
#include<algorithm>
#include"Geometry.h"

constexpr float br1 = 5.0f;//表示用の「半径」
constexpr float br2 = 10.0f;//表示用の「半径」
constexpr float dt = 15.0f;// ボーンひし形の距離

using namespace std;
constexpr UINT32 white = 0xffffffff;
constexpr UINT32 red = 0xffff8888;
class Bone {
private:
	Position2f bpos_;//基準点
	Vector2f vec_;//ベクトル(向きのみ)
	float length_ = 1.0f;
	UINT32 col_=white;
	Matrix mat_ = IdentityMat();
public:
	const Position2f& GetPosition()const {
		return bpos_;
	}
	void SetPosition(const Position2f& pos){
		bpos_=pos;
	}
	Bone(const Position2f& pos, const const Vector2f& vec) :bpos_(pos) {
		length_ =vec.Length();
		vec_ = vec/length_;
	}
	void SetMatrix(const Matrix& mat) {
		mat_ = mat;
	}
	UINT32 GetColor()const {
		return col_;
	}
	void SetColor(UINT32 col) {
		col_ = col;
	}
	Position2f Next()const {
		auto vec = MultipleVec(mat_, vec_);
		return bpos_ + vec * length_;
	}
	void Draw() {
		
		auto vec = MultipleVec(mat_, vec_);

		DrawCircleAA(bpos_.x, bpos_.y, br1,16, col_, 1, 2.0f);//円1
		DrawCircleAA(bpos_.x, bpos_.y, br2,16, col_, 0, 2.0f);//円2

		auto R=vec.R();
		Position2f p[4] = {
			bpos_,
			bpos_ + (vec - R) * dt,
			bpos_ + vec *  max(length_,2.0*dt),
			bpos_ + (vec + R) * dt };
		DrawQuadrangleAA(
			p[0].x, 
			p[0].y, 
			p[1].x, 
			p[1].y,
			p[2].x,
			p[2].y, 
			p[3].x,
			p[3].y, 
			col_, false,1.0f);
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


void RecursiveCalculate(vector<Matrix>& mats,size_t idx) {
	if (idx+1 >= mats.size())return;
	mats[idx + 1] = MultipleMat(mats[idx], mats[idx + 1]);
	RecursiveCalculate(mats, idx+1);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	DxLib::SetMainWindowText("スキニング");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);


	vector<Position2f> posses = {
		{100.0f, 250.0f},
		{350.0f,250.0f},
		{600.0f,250.0f} };

	auto bones = BuildBones(posses);
	vector<float> angles(posses.size());
	vector<Matrix> matrices(posses.size());
	fill(angles.begin(), angles.end(), 0.0f);
	fill(matrices.begin(), matrices.end(),IdentityMat());

	int mx, my;
	GetMousePoint(&mx, &my);
	Position2f lastMpos(mx, my);
	int lastMouseInput = 0;
	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		fill(matrices.begin(), matrices.end(), IdentityMat());


		for (size_t i = 0; i < bones.size(); ++i) {
			matrices[i] = RotateMat(angles[i]);
		}
		RecursiveCalculate(matrices, 0);
		for (size_t i = 0; i < bones.size(); ++i) {
			bones[i].SetMatrix(matrices[i]);
		}
		for (size_t i = 0; i < bones.size()-1; ++i) {
			bones[i+1].SetPosition(bones[i].Next());
		}
		auto minput=GetMouseInput();
		GetMousePoint(&mx, &my);
		Position2f mpos(mx, my);
		for (size_t i = 0; i < bones.size();++i) {
			auto& bone = bones[i];
			auto& angle = angles[i];
			if (minput & MOUSE_INPUT_LEFT) {
				if (lastMouseInput & MOUSE_INPUT_LEFT){
					if (bone.GetColor() == red) {
						
						auto diff = mpos.y - lastMpos.y;
						angle += diff*0.025f;
						
						
					}
				}
				else {
					auto diff = (bone.GetPosition() - mpos).Length();
					if (diff <= 5.0f) {
						bone.SetColor(red);
					}
					else {
						bone.SetColor(white);
					}
				}
				
			}
			
			bone.Draw();
		}
		
		lastMpos = mpos;
		lastMouseInput = minput;
		ScreenFlip();
	}

	DxLib_End();
	return 0;
}