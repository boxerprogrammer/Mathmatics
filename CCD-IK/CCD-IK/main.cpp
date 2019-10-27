#include<DxLib.h>
#include<vector>
#include<array>
#include<algorithm>
#include<numeric>
#include<functional>
#include"Geometry.h"

using namespace std;
float Clampf(float value, float minV = 0.0f, float maxV = 1.0f) {
	return max(min(value, maxV), minV);
}

Position2f Clamp(Position2f value, Position2f minV = Position2f(0.0f, 0.0f), Position2f maxV = Position2f(1.0f, 1.0f)) {
	return Position2f(Clampf(value.x, minV.x, maxV.x), Clampf(value.y, minV.y, maxV.y));
}

constexpr uint32_t points_num = 4;
struct Limit {
	float minimum=0.0f;//�ŏ��l
	float maximum=0.0f;//�ő�l
	Limit(float mini=0.0f, float maxi=0.0f) :minimum(mini), maximum(maxi) {}
};
struct IKPoint {
	Position2f pos;
	Limit limit;
	IKPoint(const Position2f& inp, Limit& inlim) :pos(inp), limit(inlim) {}
	IKPoint() :pos(0.0f,0.0f), limit(Limit()) {}

};

void Draw(int rt, int screen_width, int screen_height, 
	vector<IKPoint> &positions, Position2f& target) {
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

		DrawCircleAA(positions[i].pos.x + 2,
			positions[i].pos.y + 2,
			25, 32,
			0x000000,
			false,
			2);
		if (i < positions.size() - 1) {
			DrawLineAA(positions[i].pos.x + 2,
				positions[i].pos.y + 2,
				positions[i + 1].pos.x + 2,
				positions[i + 1].pos.y + 2,
				0x000000, 5);
		}
	}
	GraphFilter(rt, DX_GRAPH_FILTER_GAUSS, 4, 800);
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, rt, true);

	//�ʏ�\��
	for (int i = 0; i < positions.size(); ++i) {

		DrawCircleAA(positions[i].pos.x,
			positions[i].pos.y,
			25, 32,
			0x000000,
			false,
			2);
		if (i < positions.size() - 1) {
			DrawLineAA(positions[i].pos.x,
				positions[i].pos.y,
				positions[i + 1].pos.x,
				positions[i + 1].pos.y,
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

void MoveControlPoint(vector<IKPoint> &positions,
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
		auto diff = mp - positions[i].pos;
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
				auto diff = mp - positions[i].pos;
				auto r = diff.Length();
				if (r <= 20.0f&&i == points_num - 1) {//�[�_�̂ݑI���\
					captured = true;
					capturedNo = i;
				}
			}
		}
		else {
			if (captured&&capturedNo >= 0) {
				positions[capturedNo].pos = Clamp(mp, Position2f(0, 0), Position2f(640, 480));
				//�������A���̒����ȏ�ɗ���悤�ƁA���Ă͂����Ȃ�(����)
				auto linearVec = positions.back().pos - positions.front().pos;
				auto limitLen = std::accumulate(edgeLens.begin(), edgeLens.end(), 0);

				if (linearVec.Length() >= limitLen) {
					linearVec = linearVec.Normalized()*limitLen;
					if (capturedNo = points_num - 1) {
						positions[capturedNo].pos = positions[0].pos + linearVec;
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
constexpr float epsilon = 0.0001f;

/**
 * ���֐߃m�[�h
 */
class Node
{
public:
	Vector2f pos;
	float rad=0.0f; // ��]�p�x
	Node(float x = 0, float y = 0, float rad = 0)
	{
		x = x;
		y = y;
		rad = rad;
	}
};

/**
 * ���֐ߗp�x�N�g��
 */
struct JointVector
{
	Vector2f vec;
	float n=0.0f; // ���ς̌���
	/**
	 * ���ς����߂Č��ʂ�ێ�����
	 */
	void Dot(Vector2f& v)
	{
		n = vec.x * v.x + vec.y * v.y;
	}
};




///�����t��CCD-IK
void LimitedCCD_IK(const Position2f& target, vector<IKPoint>& initPositions,vector<IKPoint>& positions, std::vector<float>& edgeLens, unsigned int cyclic = 4) {
	//����target��positions�̐擪���S���̒�����蒷���Ȃ�p�x��180�m��Ȃ̂�CCD-IK���s�킸���[�����]������
	if (accumulate(edgeLens.begin(), edgeLens.end(), 0) < (target - positions.front().pos).Length()) {
		auto vec = target - positions.front().pos;
		vec.Normalize();
		for (int i = 0; i < edgeLens.size(); ++i) {
			positions[i + 1].pos = positions[i].pos + vec*edgeLens[i];
		}
		return;
	}

	//�J��Ԃ��񐔂����J��Ԃ�
	for (int c = 0; c < cyclic; ++c) {
		//���[����U�߂Ă���
		auto rit = positions.rbegin();
		++rit;//���[�Ƃ̊p�x�Ōv�Z���邽�߁A���[�͏Ȃ�
		float accumulateAngle = 0.0f;
		unsigned int i = 0;
		auto remainAngle = 0.0f;
		for (;rit!= positions.rend(); ++rit) {
			//��]�p�x�v�Z
			Vector2f vecToEnd = positions.back().pos - rit->pos;//���[�ƌ��݂̃m�[�h�̃x�N�g��
			Vector2f vecToTarget = target - rit->pos;//���݂̃m�[�h����^�[�Q�b�g�ւ̃x�N�g��
			//�������݂̏󋵂ƃ^�[�Q�b�g�̃x�N�g�����v�Z���Ă���B���ϓ����Ƃ�Ίp�x�����߂��邪�c�H
			

			//���ݒ������̓_�ƁA�ЂƂ������ɋ߂��_�Ńx�N�g�������(�����p�x�̂���)
			//��O�̃x�N�g�����擾�B�Ȃ��ꍇ��0�x�N�g��
			Vector2f vecPrevious = Vector2f();
			if (rit + 1 != positions.rend()) {
				vecPrevious = rit->pos - (rit + 1)->pos;
				vecPrevious.Normalize();
			}

			//�^�[�Q�b�g�����̃R���g���[���|�C���g�ƈ�v���Ă���
			//����ꍇ�͂��̃R���g���[���|�C���g�͂�����Ȃ�
			auto tarLen = vecToTarget.Length();
			auto endLen = vecToEnd.Length();
			if (tarLen == 0.0f)continue;
			if (endLen == 0.0f)continue;
			
			vecToEnd.Normalize();
			vecToTarget.Normalize();
			
			//2D������p�x�v�Z�͒P��(3D�̏ꍇ�����͂����Ȃ��ł�)
			float dot = Dot(vecToEnd, vecToTarget);
			float cross = Cross(vecToEnd, vecToTarget);
			float angle = atan2f(cross, dot); 
			
			if (dot <= -1.0f)continue;
			//���S�ɓ���(�������͔��Ό���)�������ꍇ�͂ق�̂�����ƋȂ���
			//���������~�b�g�Ɉ���������Ȃ��悤
			if (fabs(dot-1.0f) <epsilon){
				if (tarLen<endLen  ) {
					if (fabsf(rit->limit.minimum) < fabsf(rit->limit.maximum)) {
						angle = acos(tarLen / endLen)/30.0f;
					}
					else {
						angle = -acos(tarLen / endLen)/30.0f;
					}
				}
			}
			
			
			if (vecPrevious.Length() > 0) {
				//���̂ƑO�̃x�N�g���Ԃ̊p�x�𑪂��Ă���
				//���݂̂�Ǝ��̂�̊Ԃ̃x�N�g�����Z�o
				auto currentVec=rit.base()->pos - rit->pos;
				auto anglePrev = atan2(Cross(vecPrevious, currentVec), Dot(vecPrevious, currentVec));

				auto angleForLimit = angle + anglePrev;

				//auto angleForLimit = atan2(Cross(vecPrevious, vecToTarget), Dot(vecPrevious, vecToTarget));
				
				auto tmpAngle = Clampf(angleForLimit, rit->limit.minimum, rit->limit.maximum);

				//������������Ă��炻�̕��߂��B
				angle += tmpAngle - angleForLimit;
			}

			if (angle == 0.0f)continue;
			//��]�s��𓾂�
			Matrix mat = MultipleMat(TranslateMat(rit->pos.x,rit->pos.y),MultipleMat(RotateMat(angle),TranslateMat(-rit->pos.x,-rit->pos.y)));
			//���݂̃m�[�h���疖�[�܂ŉ�]����
			auto it = rit.base();
			--it;
			for (; it != positions.end(); ++it) {
				it->pos = MultipleVec(mat, it->pos);
			}
		}
		if ((positions.back().pos - target).Length() < epsilon) {
			break;
		}
	}
}

void CCD_IK(const Position2f& target, vector<IKPoint>& initPositions, vector<IKPoint>& positions, std::vector<float>& edgeLens, unsigned int cyclic = 4) {
	//�J��Ԃ��񐔂����J��Ԃ�
	for (int c = 0; c < cyclic; ++c) {
		//���[����U�߂Ă���
		auto rit = positions.rbegin();
		++rit;//���[�Ƃ̊p�x�Ōv�Z���邽�߁A���[�͏Ȃ�
		for (; rit != positions.rend(); ++rit) {
			//��]�p�x�v�Z
			Vector2f vecToEnd = positions.back().pos - rit->pos;//���[�ƌ��݂̃m�[�h�̃x�N�g��
			Vector2f vecToTarget = target - rit->pos;//���݂̃m�[�h����^�[�Q�b�g�ւ̃x�N�g��
			auto tarLen = vecToTarget.Length();
			auto endLen = vecToEnd.Length();
			if (tarLen == 0.0f)continue;
			if (endLen == 0.0f)continue;

			vecToEnd.Normalize();
			vecToTarget.Normalize();
			//2D������p�x�v�Z�͒P��(3D�̏ꍇ�����͂����Ȃ��ł�)
			float angle = atan2f(Cross(vecToEnd, vecToTarget), Dot(vecToEnd, vecToTarget));

			if (angle == 0.0f)continue;
			//��]�s��𓾂�
			Matrix mat = MultipleMat(TranslateMat(rit->pos.x, rit->pos.y), MultipleMat(RotateMat(angle), TranslateMat(-rit->pos.x, -rit->pos.y)));
			//���݂̃m�[�h���疖�[�܂ŉ�]����
			auto it = rit.base();
			for (; it != positions.end(); ++it) {
				it->pos = MultipleVec(mat, it->pos);
			}
		}
		//���[�̍��W���^�[�Q�b�g�Ɓu�قځv��v������v�Z��ł��؂�
		if ((positions.back().pos - target).Length() < epsilon) {
			break;
		}
	}

}

void InitPositions(std::vector<IKPoint> &positions, std::vector<float> &edgeLens, Position2f &target) {
	for (int i = 0; i < positions.size(); ++i) {
		positions[i].pos = Position2f(50 + i * (500 / (points_num - 1)), 240);
	}
	for (int i = 0; i < edgeLens.size(); ++i) {
		edgeLens[i] = (positions[i + 1].pos - positions[i].pos).Length();
	}
	target = positions.back().pos;
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
	
	vector<IKPoint> positions(points_num);
	vector<IKPoint> initialPositions(points_num);

	std::vector<float> edgeLens(positions.size() - 1);
	Position2f target=Position2f();
	InitPositions(positions, edgeLens, target);
	for (auto& point : positions) {
		point.limit = Limit(-0 / 12.0f, DX_PI_F/6.0f);
	}
	positions[0].limit = Limit(-DX_PI_F, DX_PI_F);//���������Ȃ�

	copy(begin(positions), end(positions), begin(initialPositions));

	float angle = 0.0f;
	
	vector<function<void(const Position2f&, vector<IKPoint>&, vector<IKPoint>&, vector<float>&, unsigned  int) >> ccdfuncs;

	ccdfuncs.emplace_back([](const Position2f& t, vector<IKPoint>& ini, vector<IKPoint>& ikposs, vector<float>& edgeLens, unsigned  int trycount) {
		CCD_IK(t, ini, ikposs, edgeLens, trycount);
		DrawFormatString(10, 10, 0x000000,"CCD-IK(�����p�x�Ȃ�) : ���s��=%d",trycount);
		});
	ccdfuncs.emplace_back( [](const Position2f& t, vector<IKPoint>& ini, vector<IKPoint>& ikposs, vector<float>& edgeLens, unsigned  int trycount) {
		LimitedCCD_IK(t, ini, ikposs, edgeLens, trycount);
		DrawFormatString(10, 10, 0x000000,"CCD-IK(�����p�x����) : ���s��=%d", trycount);
		int py = 10;
		for (auto& ikpos : ikposs) {
			py += 25;
			DrawFormatString(10, py, 0x000000, "�}�C�i�X�����p=%d", static_cast<int>(ikpos.limit.minimum *180.0f/DX_PI_F));
			py += 25;
			DrawFormatString(10, py, 0x000000, "�v���X�����p=%d", static_cast<int>(ikpos.limit.maximum *180.0f / DX_PI_F));
		}
		});

	int func_id = 0;
	int trycount = 4;
	int limitMaxAngle = 30;
	int limitMinAngle = 0;
	while (ProcessMessage() == 0) {
		GetHitKeyStateAll(keystate);
		//R���������烊�X�^�[�g
		if (keystate[KEY_INPUT_R] && !lastKeyState[KEY_INPUT_R]) {
			InitPositions(positions, edgeLens, target);
			trycount = 4;
		}

		//�����p�ύX
		if (keystate[KEY_INPUT_Z] && !lastKeyState[KEY_INPUT_Z]) {
			InitPositions(positions, edgeLens, target);
			limitMaxAngle = (limitMaxAngle + 30) % 180;
			for (auto& point : positions) {
				point.limit = Limit(DX_PI_F*static_cast<float>(-limitMinAngle) / 180.0f, DX_PI_F*static_cast<float>(limitMaxAngle) / 180.0f);
			}
			positions[0].limit = Limit(-DX_PI_F, DX_PI_F);//�������͐����Ȃ�
		}
		if (keystate[KEY_INPUT_X] && !lastKeyState[KEY_INPUT_X]) {
			InitPositions(positions, edgeLens, target);
			limitMinAngle = (limitMinAngle + 30) % 180;
			for (auto& point : positions) {
				point.limit = Limit(DX_PI_F*static_cast<float>(-limitMinAngle) / 180.0f, DX_PI_F*static_cast<float>(limitMaxAngle) / 180.0f);
			}
			positions[0].limit = Limit(-DX_PI_F, DX_PI_F);//�������͐����Ȃ�
		}

		if (keystate[KEY_INPUT_UP] && !lastKeyState[KEY_INPUT_UP]) {
			InitPositions(positions, edgeLens, target);
			func_id = (func_id + 1) % ccdfuncs.size();
		}else if (keystate[KEY_INPUT_DOWN] && !lastKeyState[KEY_INPUT_DOWN]) {
			InitPositions(positions, edgeLens, target);
			func_id = (func_id+ccdfuncs.size()-1)%ccdfuncs.size();

		}

		//�R���g���[���|�C���g�̈ړ�
		//MoveControlPoint(positions, overlapped, lastMouseInput, captured, capturedNo, edgeLens);
		MoveTarget(target, lastMouseInput, captured, targetOverlapped);

		//CCD_IK(target, initialPositions, positions,edgeLens,4);
		//LimitedCCD_IK(target, initialPositions, positions, edgeLens, 4);
		

		//�`��
		Draw(rt, screen_width, screen_height, positions, target);

		ccdfuncs[func_id](target, initialPositions, positions, edgeLens, trycount);

		ScreenFlip();
		copy(begin(keystate), end(keystate), begin(lastKeyState));
	}
	DxLib_End();
}
