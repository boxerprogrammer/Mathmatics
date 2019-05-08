#pragma once
//‰æ–Ê—h‚êƒNƒ‰ƒX
class ScreenShaker
{
private:
	int _screenH;//‰æ–Ê
	float _shakeScale;//—h‚ê•
public:
	ScreenShaker();
	~ScreenShaker();
	//—h‚êƒgƒŠƒK[
	void Shake();

	//‰æ–Ê—h‚ê€”õ()
	void PrepareShake();

	//‰æ–Ê—h‚êXV
	void UpdateShake();
};

