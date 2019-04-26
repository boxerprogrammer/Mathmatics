#pragma once

class Ojisan;

class World
{
private:
	Ojisan& _Ojisan;
	float _angle;
public:
	World(Ojisan& Ojisan);
	~World(void);
	void Update();
	void Draw();
	float getAngle();

	///XÀ•W‚ğ“ü—Í‚·‚ê‚Î‚‚³‚ªo‚Ü‚·
	float getHeight(float x);
};

