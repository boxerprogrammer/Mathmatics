#include<DxLib.h>
#include<vector>
#include<algorithm>

struct Vector2 {
	float x, y;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	constexpr int div = 40;
	std::vector<Vector2> curveZ(div+1);
	std::vector<Vector2> curveW(div+1);
	std::vector<Vector2> curveZW(div+1);

	float nearZ = 1.0f;
	float farZ = 5.0f;

	


	constexpr float scale = 400.0f;



	constexpr int graph_offset_x = 30;
	constexpr int graph_offset_y = 30;
	float step = 1.0f / static_cast<float>(div);
	char keystate[256];
	while (ProcessMessage() != -1) {
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_UP]) {
			farZ += 1.0f;
		}
		else if (keystate[KEY_INPUT_DOWN]) {
			farZ -= 1.0f;
		}
		farZ = std::clamp(farZ, nearZ+1.0f, 1000.0f);

		const float stepZ = (farZ - nearZ) / static_cast<float>(div);
		float z = nearZ;
		for (int i = 0; i < div + 1; ++i) {
			float x = static_cast<float>(i) / static_cast<float>(div);
			curveZ[i] = { x,(z - nearZ) * (farZ) / (farZ - nearZ) };
			curveW[i] = { x, z };
			curveZW[i] = { x,curveZ[i].y / curveW[i].y };
			z += stepZ;
		}
		const float fscale = scale / farZ;




		float valueX = 0.0f;
		for (int i = 0; i <= 10; ++i) {
			DrawFormatString(0, 20+i*40, 0xffffff, L"%1.1f", static_cast<float>(i)/10.0f);
		}
		for (int i = 0; i <= 10; ++i) {
			DrawFormatString(10 + i * 40, 0, 0xffffff, L"%1.1f", static_cast<float>(i)/10.0f);
		}
		for (int i = 0; i <= div; ++i) {

			valueX += step;
			DrawCircle(graph_offset_x+curveZ[i].x * scale, 
						graph_offset_y+curveZ[i].y*fscale,2, 0xffaaaa);
			DrawCircle(graph_offset_x+ curveW[i].x * scale, 
						graph_offset_y+curveW[i].y * fscale, 2, 0xaaffaa);
			DrawCircle(graph_offset_x + curveZW[i].x * scale, 
						graph_offset_y+curveZW[i].y * scale,3, 0xaaaaff);
			if (i < div ) {
				DrawLineAA(graph_offset_x + curveZ[i].x * scale,
					graph_offset_y + curveZ[i].y * fscale,
					graph_offset_x + curveZ[i + 1].x * scale,
					graph_offset_y + curveZ[i + 1].y * fscale,
					0xff0000, 1.0f);
				DrawLineAA(graph_offset_x + curveW[i].x * scale,
					graph_offset_y + curveW[i].y * fscale,
					graph_offset_x + curveW[i + 1].x * scale,
					graph_offset_y + curveW[i + 1].y * fscale,
					0x00ff00, 1.0f);
				DrawLineAA(graph_offset_x + curveZW[i].x * scale,
					graph_offset_y + curveZW[i].y * scale,
					graph_offset_x + curveZW[i + 1].x * scale,
					graph_offset_y + curveZW[i + 1].y * scale,
					0xaaaaff, 2.0f);
			}
		}

		DrawLine(graph_offset_x+1.0f*scale, 0, graph_offset_x+1.0f * scale, 450, 0xffffff);
		DrawLine( 0, graph_offset_y+1.0*scale,450, graph_offset_y+1.0f * scale,  0xffffff);
		
		DrawString(graph_offset_x + 1.0f * scale+20,
			graph_offset_y, L"Z color ", 0xffaaaa);
		DrawLineAA(graph_offset_x + 1.0f * scale + 100, graph_offset_y+10,
			graph_offset_x + 1.0f * scale + 200, graph_offset_y+10,
			0xffaaaa,2.0f);

		DrawString(graph_offset_x + 1.0f * scale + 20,
			graph_offset_y+30, L"W color ", 0xaaffaa);
		DrawLineAA(graph_offset_x + 1.0f * scale + 100, graph_offset_y + 30+10,
			graph_offset_x + 1.0f * scale + 200, graph_offset_y + 30+10,
			0xaaffaa, 2.0f);

		DrawString(graph_offset_x + 1.0f * scale + 20,
			graph_offset_y+60, L"Z/W color ", 0xaaaaff);
		DrawLineAA(graph_offset_x + 1.0f * scale + 100, graph_offset_y + 60 + 10,
			graph_offset_x + 1.0f * scale + 200, graph_offset_y + 60 + 10,
			0xaaaaff, 2.0f);

		DrawFormatString(graph_offset_x + 1.0f * scale,
			graph_offset_y + 1.0 * scale, 0xffffff,
			L"near=%2.2f far=%2.2f",nearZ,farZ);

		ScreenFlip();
	}
	return 0;
}