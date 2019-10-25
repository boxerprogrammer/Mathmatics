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

/**
 * �֐߂̈ړ�(Inverse Kinematics)
 * @param nodes    �֐�([0]�����`[$-1]��[)
 * @param vRad     �֐߂̉�]�p�x
 * @param lRad     ��]�p�x�̌��E�l
 * @param distance �֐ߊԂ̋���
 * @param x        �ڕW���W(X)
 * @param y        �ڕW���W(Y)
 */
void CCDIKSample(vector<Node>& nodes, float vRad, float lRad,
	float distance, const Position2f& target)
{
	float cos = cosf(vRad);
	float sin = sinf(vRad);
	// �O���̏���(��]�p�x�����߂�)
	Node& nTip = nodes.back(); // ��[�m�[�h
	// ��[�ˍ���
	// ���o�[�X�C�e���[�g�B��[�m�[�h�͏������Ȃ�
	for (int i = nodes.size() - 1; i >= 0; i--)
	{
		Node& n = nodes[i];
		// �֐߂��玩�@�ւ̃x�N�g���̌v�Z
		Vector2f dVec=target - n.pos;
		// �֐߂����[�ւ̃x�N�g���Ɠ��ς̌v�Z
		JointVector jNode;
		jNode.vec= nTip.pos - n.pos;
		jNode.Dot(dVec);

		// �E���̃x�N�g���̌v�Z
		JointVector jRight;
		if (n.rad + vRad <= lRad)
		{
			jRight.vec.x = cos * jNode.vec.x - sin * jNode.vec.y;
			jRight.vec.y = sin * jNode.vec.x + cos * jNode.vec.y;
			jRight.Dot(dVec);
		}
		else
		{
			// ���E�𒴂����̂ŉ񂹂Ȃ�
			jRight.n = jNode.n;
		}

		// �����̃x�N�g���̌v�Z
		JointVector jLeft;
		if (n.rad - vRad >= -lRad)
		{
			jLeft.vec.x = cos * jNode.vec.x + sin * jNode.vec.y;
			jLeft.vec.y = -sin * jNode.vec.x + cos * jNode.vec.y;
			jLeft.Dot(dVec);
		}
		else
		{
			// ���E�𒴂����̂ŉ񂹂Ȃ�
			jLeft.n = jNode.n;
		}

		// ��]�����̑I��
		// ���ς��r���āA��]���R�ʂ�̂Ȃ�����I��
		// ��[����]�����āA�V������[�̈ʒu�����߂�
		if (jRight.n > jNode.n&& jRight.n > jLeft.n)
		{
			// �E���
			n.rad += vRad;
			nTip.pos = n.pos + jRight.vec;
				
		}
		if (jLeft.n > jNode.n && jLeft.n > jRight.n)
		{
			// �����
			n.rad -= vRad;
			nTip.pos = n.pos + jLeft.vec;
		}
	}

	// �㔼�̏���(���W�����߂�)
	Position2f p =Position2f( distance,0);

	// �����ː�[
	// �����͈ړ����Ȃ�
	for (int i = 1; i < nodes.size(); i++)
	{
		Node& n1 = nodes[i - 1];
		Node& n2 = nodes[i];
		float cos = cosf(n1.rad);
		float sin = sinf(n1.rad);
		Vector2f d(cos * p.x - sin * p.y,
				sin * p.x + cos * p.y);
		n2.pos = n1.pos + d;
		p = d;
	}
}






///�����t��CCD-IK
void LimitedCCD_IK(Position2f target, vector<IKPoint>& initPositions,vector<IKPoint>& positions, std::vector<float>& edgeLens, unsigned int cyclic = 4) {
	//����target��positions�̐擪���S���̒�����蒷���Ȃ�p�x��180�m��Ȃ̂�CCD-IK���s�킸���[�����]������
	if (accumulate(edgeLens.begin(), edgeLens.end(), 0) < (target - positions.front().pos).Length()) {
		auto vec = target - positions.front().pos;
		vec.Normalize();
		for (int i = 0; i < edgeLens.size(); ++i) {
			positions[i + 1].pos = positions[i].pos + vec*edgeLens[i];
		}
		return;
	}

	//copy(begin(initPositions), end(initPositions), begin(positions));

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
			Vector2f vecPrevious = Vector2f();
			if (rit + 1 != positions.rend()) {
				vecPrevious = rit->pos - (rit + 1)->pos;
				vecPrevious.Normalize();
			}
			//���ݒ������̓_�ƁA�ЂƂ������ɋ߂��_�Ńx�N�g�������(�����p�x�̂���)

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
			if (fabs(dot-1.0f) <epsilon){//���S�ɓ���(�������͔��Ό���)�������ꍇ�͋Ȃ�����Ȃ����
				if (tarLen<endLen  ) {
					if (fabsf(rit->limit.minimum) < fabsf(rit->limit.maximum)) {
						angle = acos(tarLen / endLen)/30.0f;
					}
					else {
						angle = -acos(tarLen / endLen)/30.0f;
					}
				}
			}
			else {
				
			}
			auto tmpAngle=Clampf(angle, rit->limit.minimum, rit->limit.maximum);
			angle = atan2f(Cross(vecToEnd, vecToTarget), Dot(vecToEnd, vecToTarget));
			if (angle == tmpAngle) {
				remainAngle = 0.0f;
			}
			else {
				remainAngle = angle - tmpAngle;//�{���͉�]���Ă����c��p�x��ۑ�
				angle = tmpAngle;
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

void CCD_IK(Position2f target, vector<IKPoint>& initPositions, vector<IKPoint>& positions, std::vector<float>& edgeLens, unsigned int cyclic = 4) {
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
		point.limit = Limit(-0 / 12.0f, DX_PI_F / 2.0f);
	}
	positions[0].limit = Limit(-DX_PI_F, DX_PI_F);//���������Ȃ�

	copy(begin(positions), end(positions), begin(initialPositions));

	vector<Node> nodes(positions.size());
	for (int i = 0; i < positions.size();++i) {
		nodes[i].pos = positions[i].pos;
	}

	float angle = 0.0f;
	
	while (ProcessMessage() == 0) {
		GetHitKeyStateAll(keystate);
		//R���������烊�X�^�[�g
		if (keystate[KEY_INPUT_R] && lastKeyState[KEY_INPUT_R]) {
			InitPositions(positions, edgeLens, target);
		}

		//�R���g���[���|�C���g�̈ړ�
		//MoveControlPoint(positions, overlapped, lastMouseInput, captured, capturedNo, edgeLens);
		MoveTarget(target, lastMouseInput, captured, targetOverlapped);

		//CCD_IK(target, initialPositions, positions,edgeLens,4);
		LimitedCCD_IK(target, initialPositions, positions, edgeLens, 8);

		//�`��
		Draw(rt, screen_width, screen_height, positions, target);

		ScreenFlip();
		copy(begin(keystate), end(keystate), begin(lastKeyState));
	}
	DxLib_End();
}
