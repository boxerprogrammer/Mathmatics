#include<DxLib.h>
#include<vector>
#include<array>
#include<algorithm>
#include<numeric>
#include"Geometry.h"

using namespace std;
float Clampf(float value, float minV = 0.0f, float maxV = 1.0f) {
	return max(min(value, maxV), minV);
}

Position2f Clamp(Position2f value, Position2f minV = Position2f(0.0f, 0.0f), Position2f maxV = Position2f(1.0f, 1.0f)) {
	return Position2f(Clampf(value.x, minV.x, maxV.x), Clampf(value.y, minV.y, maxV.y));
}

constexpr uint32_t points_num = 4;

void Draw(int rt, int screen_width, int screen_height, 
	vector<Position2f> &positions, Position2f& target) {
	SetDrawScreen(rt);
	DrawBox(0, 0, screen_width, screen_height, 0xffffff, true);

	//�e�\��
	DrawCircleAA(target.x + 2,
		target.y + 2,
		25, 32,
		0x000000,
		false,
		2);
	for (int i = 0; i < positions.size(); ++i) {

		DrawCircleAA(positions[i].x + 2,
			positions[i].y + 2,
			25, 32,
			0x000000,
			false,
			2);
		if (i < positions.size() - 1) {
			DrawLineAA(positions[i].x + 2,
				positions[i].y + 2,
				positions[i + 1].x + 2,
				positions[i + 1].y + 2,
				0x000000, 5);
		}
	}
	GraphFilter(rt, DX_GRAPH_FILTER_GAUSS, 4, 800);
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, rt, true);

	//�ʏ�\��
	for (int i = 0; i < positions.size(); ++i) {

		DrawCircleAA(positions[i].x,
			positions[i].y,
			25, 32,
			0x000000,
			false,
			2);
		if (i < positions.size() - 1) {
			DrawLineAA(positions[i].x,
				positions[i].y,
				positions[i + 1].x,
				positions[i + 1].y,
				0x000000, 5);
		}
		DrawCircleAA(target.x,
			target.y,
			25, 32,
			0xaa0000,
			false,
			2);
	}

}

void MoveControlPoint(vector<Position2f> &positions, 
	array<bool, points_num> &overlapped, 
	int &lastMouseInput, 
	bool &captured, 
	int &capturedNo, 
	std::vector<float> &edgeLens) {
	//�[�_�h���b�O�A���h�h���b�v�R���g���[��
	int mx, my;
	GetMousePoint(&mx, &my);
	Position2f mp(mx, my);
	for (int i = 0; i < positions.size(); ++i) {
		auto diff = mp - positions[i];
		auto r = diff.Length();
		if (r <= 20.0f&&i == points_num - 1) {//�[�_�ȊO�͑I�����Ȃ�
			overlapped[i] = true;
		}
		else {
			overlapped[i] = false;
		}
	}
	auto currentMouseInput = GetMouseInput();
	if (currentMouseInput) {
		if (lastMouseInput == 0) {
			for (int i = 0; i < positions.size(); ++i) {
				auto diff = mp - positions[i];
				auto r = diff.Length();
				if (r <= 20.0f&&i == points_num - 1) {//�[�_�̂ݑI���\
					captured = true;
					capturedNo = i;
				}
			}
		}
		else {
			if (captured&&capturedNo >= 0) {
				positions[capturedNo] = Clamp(mp, Position2f(0, 0), Position2f(640, 480));
				//�������A���̒����ȏ�ɗ���悤�ƁA���Ă͂����Ȃ�(����)
				auto linearVec = positions.back() - positions.front();
				auto limitLen = std::accumulate(edgeLens.begin(), edgeLens.end(), 0);

				if (linearVec.Length() >= limitLen) {
					linearVec = linearVec.Normalized()*limitLen;
					if (capturedNo = points_num - 1) {
						positions[capturedNo] = positions[0] + linearVec;
					}
				}

			}
		}
		lastMouseInput = currentMouseInput;
	}
	else {
		lastMouseInput = 0;
		capturedNo = -1;
		captured = false;
	}
}

void MoveTarget(Position2f& target, int &lastMouseInput, bool &captured, bool& targetOverlapped){
	//�[�_�h���b�O�A���h�h���b�v�R���g���[��
	int mx, my;
	GetMousePoint(&mx, &my);
	Position2f mp(mx, my);
	auto diff = mp - target;
	auto r = diff.Length();
	if (r <= 20.0f) {//�[�_�ȊO�͑I�����Ȃ�
		targetOverlapped = true;
	}
	else {
		targetOverlapped = false;
	}
	
	auto currentMouseInput = GetMouseInput();
	if (currentMouseInput) {
		if (lastMouseInput == 0) {
			auto diff = mp - target;
			auto r = diff.Length();
			if (r <= 20.0f) {//�[�_�̂ݑI���\
				captured = true;
			}
		}
		else {
			if (captured) {
				target = Clamp(mp, Position2f(0, 0), Position2f(640, 480));
				//�������A���̒����ȏ�ɗ���悤�ƁA���Ă͂����Ȃ�(����)
				

			}
		}
		lastMouseInput = currentMouseInput;
	}
	else {
		lastMouseInput = 0;
		captured = false;
	}
}

///positions�̖��[��target�Ɉ�v���A���擪�𓮂����Ȃ��悤��positions������������
void CCD_IK(Position2f target, vector<Position2f>& positions, std::vector<float>& edgeLens,unsigned int cyclic = 4) {
	//����target��positions�̐擪���S���̒�����蒷���Ȃ�p�x��180�m��Ȃ̂�CCD-IK���s�킸���[�����]������
	if (accumulate(edgeLens.begin(), edgeLens.end(), 0) < (target - positions.front()).Length()) {
		auto vec = target - positions.front();
		vec.Normalize();
		for (int i = 0; i < edgeLens.size(); ++i) {
			positions[i + 1] = positions[i] + vec*edgeLens[i];
		}
		return;
	}

	//�J��Ԃ��񐔂����J��Ԃ�
	for (int c = 0; c < cyclic; ++c) {
		//���[����U�߂Ă���
		auto rit = positions.rbegin();
		++rit;//���[�Ƃ̊p�x�Ōv�Z���邽�߁A���[�͏Ȃ�
		for (;rit!= positions.rend(); ++rit) {
			//��]�p�x�v�Z
			Vector2f vecToEnd = positions.back() - *rit;//���[�ƌ��݂̃m�[�h�̃x�N�g��
			Vector2f vecToTarget = target - *rit;//���݂̃m�[�h����^�[�Q�b�g(�G�t�F�N�^)�ւ̃x�N�g��
			vecToEnd.Normalize();
			vecToTarget.Normalize();
			//2D������p�x�v�Z�͒P��(3D�̏ꍇ�����͂����Ȃ�)
			auto cross = Cross(vecToEnd, vecToTarget);
			auto dot = Dot(vecToEnd, vecToTarget);
			auto angle = atan2f(Cross(vecToEnd, vecToTarget), Dot(vecToEnd, vecToTarget));
			//��]�s��𓾂�
			Matrix mat = MultipleMat(TranslateMat(rit->x,rit->y),MultipleMat(RotateMat(angle),TranslateMat(-rit->x,-rit->y)));
			//���݂̃m�[�h���疖�[�܂ŉ�]����
			auto it = rit.base();
			for (; it != positions.end(); ++it) {
				*it = MultipleVec(mat, *it);
			}


		}
	}
}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetWindowText("CCD-IK 2D");
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	int screen_width, screen_height;
	GetWindowSize(&screen_width, &screen_height);
	SetDrawScreen(DX_SCREEN_BACK);
	auto rt = MakeScreen(screen_width, screen_height);


	bool targetOverlapped = false;
	int currentX = 512;
	int capturedNo = -1;
	int lastMouseInput = 0;
	bool captured = false;
	char keystate[256];
	char lastKeyState[256];
	
	array<bool, points_num> overlapped = {};
	vector<Position2f> positions(points_num);
	for (int i = 0; i < positions.size(); ++i) {
		positions[i] = Position2f(50 + i * (500/(points_num-1)), 240);
	}
	std::vector<float> edgeLens(positions.size() - 1);
	for (int i = 0; i < edgeLens.size(); ++i) {
		edgeLens[i] = (positions[i + 1] - positions[i]).Length();
	}

	float angle = 0.0f;
	Position2f target = positions.back();
	while (ProcessMessage() == 0) {

		//�R���g���[���|�C���g�̈ړ�
		//MoveControlPoint(positions, overlapped, lastMouseInput, captured, capturedNo, edgeLens);
		MoveTarget(target, lastMouseInput, captured, targetOverlapped);

		CCD_IK(target, positions,edgeLens);


		//�`��
		Draw(rt, screen_width, screen_height, positions, target);

		ScreenFlip();

	}
	DxLib_End();
}
