#include"Application.h"
#include<Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	auto& app = Application::GetInstance();
	if (!app.Init()) {
		return -1;
	}
	app.Run();
	app.Terminate();
	return 0;
}