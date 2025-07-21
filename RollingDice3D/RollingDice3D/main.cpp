#include<DxLib.h>
#include<array>
#include"Geometry.h"
#include"Dice.h"
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	// Initialize the DxLib library
	if (DxLib_Init() == -1) {
		return -1; // Initialization failed
	}
	// Set the background color to white
	SetDrawScreen(DX_SCREEN_BACK);

	SetLightAmbColor(GetColorF(0.5,0.5,0.5,1.0)); // Set ambient light color to white
	SetGlobalAmbientLight(GetColorF(0.5, 0.5, 0.5, 1.0));
	MATERIALPARAM material = {};
	material.Ambient = {0.2,0.2,0.2,1.0};
	DxLib::SetMaterialParam(material);

	SetLightEnable(true);
	SetUseLighting(true); // Disable lighting
	DxLib::SetUseBackCulling(true); // Disable back face culling
	Dice dice;
	SetUseZBuffer3D(true); // Enable Z-buffer for 3D rendering

	auto imgH = LoadGraph(L"img/dice.png");
	auto seH = LoadSoundMem(L"se/rollrock.wav");

	std::array<VERTEX3D, 3> triangle3d;
	for(auto& v : triangle3d){
		v.dif = v.spc = { 255,255,255,255 };
	}
	triangle3d[0].pos = VGet(-10, 0, 0);
	triangle3d[1].pos = VGet(0, 10, 0);
	triangle3d[2].pos = VGet(10, 0, 0);
	DxLib::SetCameraScreenCenter(320, 240);
	constexpr float dice_size = 2.0f; // Size of the dice
	constexpr float dice_size_2 = dice_size * 2.0f;
	constexpr int cell_div = 6; //
	float angle = 0.0f;
	auto diceMat = TranslationMatrix({dice_size-dice_size_2*cell_div/2,0.0f,dice_size-dice_size_2*cell_div/2}); // Create a rotation matrix for the dice
	dice.Update(diceMat);

	std::array<char, 256> keystate={};
	std::array<char, 256> lastKeystate={};
	int rotateFrame = 0;
	constexpr int rotate_frame_max = 30; // Number of frames to rotate the dice
	Matrix4x4 mat = IdentityMatrix();

	VECTOR cameraPos = VGet(10, 20, -40.0f); // Camera position


	while (ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		ClearDrawScreen(); // Clear the drawing screen
		lastKeystate = keystate;
		GetHitKeyStateAll(keystate.data()); // Get the current key states
	//	DrawBox(0, 0, 640, 480, GetColor(0, 0, 255), true); // Draw a black background
		SetCameraNearFar(0.1f, 100.0f); // Set camera near and far planes
		SetupCamera_Perspective(DX_PI_F / 4.0f); // Set up perspective camera with 45 degrees field of view
		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));

		if(keystate[KEY_INPUT_W]){
			cameraPos.z += 0.1f; // Move camera forward
		}
		if(keystate[KEY_INPUT_S]){
			cameraPos.z -= 0.1f; // Move camera backward
		}
		if(keystate[KEY_INPUT_A]){
			cameraPos.x -= 0.1f; // Move camera left
		}
		if(keystate[KEY_INPUT_D]){
			cameraPos.x += 0.1f; // Move camera right
		}
		if(keystate[KEY_INPUT_Q]){
			cameraPos.y += 0.1f; // Move camera up
		}
		if(keystate[KEY_INPUT_E]){
			cameraPos.y -= 0.1f; // Move camera down
		}


		if(rotateFrame==0){
			mat = IdentityMatrix();
			if(keystate[KEY_INPUT_RIGHT] && !lastKeystate[KEY_INPUT_RIGHT]) {
				float angle = -(DX_PI_F / 2.0f)/static_cast<float>(rotate_frame_max);
				auto bb = dice.GetBoundingBox();
				mat = TranslationMatrix(bb.max.x,bb.min.y,0.0f) * RotationMatrixZ(angle)* TranslationMatrix(-bb.max.x,-bb.min.y,0.0f);
				rotateFrame = rotate_frame_max;
			}
			if(keystate[KEY_INPUT_LEFT] && !lastKeystate[KEY_INPUT_LEFT]) {
				float angle = (DX_PI_F / 2.0f)/static_cast<float>(rotate_frame_max);
				auto bb = dice.GetBoundingBox();
				mat = TranslationMatrix(bb.min.x,bb.min.y,0.0f) * RotationMatrixZ(angle)* TranslationMatrix(-bb.min.x,-bb.min.y,0.0f);
				rotateFrame = rotate_frame_max;
			}
			if(keystate[KEY_INPUT_UP] && !lastKeystate[KEY_INPUT_UP]) {
				float angle = (DX_PI_F / 2.0f)/static_cast<float>(rotate_frame_max);
				auto bb = dice.GetBoundingBox();
				mat = TranslationMatrix(0.0f,bb.min.y, bb.max.z) * RotationMatrixX(angle)* TranslationMatrix(0.0f,-bb.min.y, -bb.max.z);
				rotateFrame = rotate_frame_max;
			}
			if(keystate[KEY_INPUT_DOWN] && !lastKeystate[KEY_INPUT_DOWN]) {
				float angle = -(DX_PI_F / 2.0f)/static_cast<float>(rotate_frame_max);
				auto bb = dice.GetBoundingBox();
				mat = TranslationMatrix(0.0f,bb.min.y, bb.min.z) * RotationMatrixX(angle)* TranslationMatrix(0.0f,-bb.min.y, -bb.min.z);
				rotateFrame = rotate_frame_max;
			}
		}else{
			--rotateFrame;
			if(rotateFrame==0){
				mat = IdentityMatrix();
				PlaySoundMem(seH, DX_PLAYTYPE_BACK); // Play the sound effect
			}
		}


		//ògÇçÏÇÈ
		float lineX = -(cell_div / 2)*dice_size_2;
		float halfWidthZ = (cell_div / 2)*dice_size_2;
		float lineZ = -(cell_div / 2)*dice_size_2;
		float halfWidthX = (cell_div / 2)*dice_size_2;
		for(int i=0;i<= cell_div; ++i) {
			DrawLine3D(VGet(lineX, -dice_size, -halfWidthZ), VGet(lineX, -dice_size, halfWidthZ), 0xffffff);
			lineX += dice_size_2;
		}
		for(int i=0;i<= cell_div; ++i) {
			DrawLine3D(VGet(-halfWidthX, -dice_size, lineZ), VGet(halfWidthX, -dice_size, lineZ), 0xffffff);
			lineZ += dice_size_2;
		}
		


		dice.Update(mat);
		dice.Draw();
		
		DrawFormatString(10, 10, GetColor(255, 255, 255), L"FPS=%2.2f", GetFPS());
		ScreenFlip();
	}
	// Terminate the DxLib library
	DxLib_End();
	return 0;
}