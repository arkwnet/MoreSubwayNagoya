#include "DxLib.h"
#include "Common.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int key[256];
	bool isJoypad = false;
	int joypad[7];
	int screenWidth = 1366;
	int screenHeight = 768;
	VECTOR camera;
	float cameraZLength, cameraZDistance, cameraAngle;

	ChangeWindowMode(TRUE);
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	ChangeWindowMode(TRUE);
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	SetUse3DFlag(TRUE);
	SetUseZBuffer3D(TRUE);
	SetMainWindowText(L"もっと! 地下鉄 大名古屋");
	SetWindowSize(screenWidth, screenHeight);
	SetGraphMode(screenWidth, screenHeight, 32);
	if (DxLib_Init() == -1) {
		return -1;
	}

	int runDistance, drawDistance;
	int pad, padX, padY;
	int padNum = GetJoypadNum();
	if (padNum >= 1) {
		isJoypad = true;
		SetJoypadDeadZone(DX_INPUT_PAD1, 0.0);
	}
	int controlBuffer;

	Fps fps;
	Game game = {
		0,		// mode
		0,		// status
		0,		// count
		0		// clock
	};

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		fps.Update();
		ClearDrawScreen();
		// TODO: ここにメインプログラムを書く
		game.count++;
		game.clock++;
		if (game.clock >= fps.Get()) {
			game.clock = 0;
		}
		ScreenFlip();
		fps.Wait();
	}
	DxLib_End();
	return 0; 
}
