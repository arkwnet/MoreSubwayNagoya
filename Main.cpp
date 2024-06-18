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
	int backgroundHandle[2];
	int spriteHandle[16];
	int soundHandle[16];

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
		-1,		// mode
		0,		// status
		0,		// count
		0		// clock
	};

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		fps.Update();
		ClearDrawScreen();
		switch (game.mode) {
			case -1:
				game.count = -10;
				game.mode = 0;
				break;
			case 0:
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				if (game.count < 0) {
					backgroundHandle[0] = LoadGraph(L"Assets\\Image\\Arakawa.png");
					// soundHandle[0] = LoadSoundMem(L"Assets\\Sound\\Decide.wav");
					game.count = 0;
				}
				if (game.count >= 30 && game.count < 45) {
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, (game.count - 30) * (255 / 15));
				}
				if (game.count >= 105 && game.count < 120) {
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - ((game.count - 105) * (255 / 15)));
				}
				if (game.count >= 30 && game.count < 120) {
					DrawGraph(screenWidth / 2 - 301, screenHeight / 2 - 110, backgroundHandle[0], FALSE);
				}
				// if (game.count < 105 && (key[KEY_INPUT_SPACE] == 1 || joypad[PAD_3] == 1)) {
				// 	game.count = 105;
				// }
				if (game.count == 180) {
					game.count = -10;
					game.mode = 1;
				}
				break;
		}
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
