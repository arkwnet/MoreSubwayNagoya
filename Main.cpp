#include <math.h>
#include "DxLib.h"
#include "Common.h"
#include "Control.h"
#include "Graphic.h"
#include "Map.h"
#include "Sound.h"
#include "Title.h"
#include "TObject.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int key[256];
	bool isJoypad = false;
	int joypad[8];
	int screenWidth = 1366;
	int screenHeight = 768;
	VECTOR camera;
	float cameraZLength = 1.0f;
	float cameraZDistance = 0.0f;
	float cameraAngle = 0.0f;
	Position rail;
	Position point;
	Navi navi;
	Train train = { 120, 3.0, 3.5, 4.0, 8, 5, {0, 20, 40, 80, 115, 150, 185, 260, 300} };

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

	int backgroundHandle[2];
	int spriteHandle[16];
	int soundHandle[16];

	float mRailPosition[2000][2];
	int mRailHandle[4][200];
	int mTunnelHandle[200];
	int mPlatformHandle[2][200];
	static const int C_DISTANCE = sizeof(mRailHandle[0]) / sizeof(mRailHandle[0][0]);
	const int mRailHandleBase = MV1LoadModel(L"Assets\\Model\\Rail\\1067.mqo");
	const int mTunnelHandleBase = MV1LoadModel(L"Assets\\Model\\Tunnel\\Sakuradori1.mqo");
	const int mPlatformHandleBase = MV1LoadModel(L"Assets\\Model\\Station\\Platform\\620.mqo");

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

	while (ProcessMessage() == 0) {
		fps.Update();
		ClearDrawScreen();
		GetKey(key);
		GetJoypad(joypad, isJoypad);
		switch (game.mode) {
			case -1:
				game.count = -10;
				game.mode = 0;
				break;
			case 0:
				if (game.count < 0) {
					backgroundHandle[0] = LoadGraph(L"Assets\\Image\\Arakawa.png");
					game.count = 0;
				}
				DrawArakawaLogo(game, backgroundHandle[0], screenWidth, screenHeight);
				if (game.count < 105 && (key[KEY_INPUT_SPACE] == 1 || joypad[PAD_3] == 1)) {
					game.count = 105;
				}
				if (game.count == 180) {
					game.count = -10;
					game.mode = 1;
				}
				break;
			case 100:
				if (game.status == 0) {
					rail = { 0.0f, 8.0f, 0.0f, 0.0f };
					point = { 0.0f, 8.0f, 0.0f, 0.0f };
					navi = { 9, 0, 0, 80, 0, 55, 895, 2620 };
					camera = VGet(0.0f, 10.1f, 0.0f);
					cameraAngle = 0.0f;
					runDistance = 0;
					drawDistance = 0;
					for (int i = 0; i < C_DISTANCE; i++) {
						mRailHandle[0][i] = MV1DuplicateModel(mRailHandleBase);
						rail.a = GetRailAngle(i, rail.a, navi.section);
						MV1SetPosition(mRailHandle[0][i], VGet(rail.x, rail.y, rail.z));
						MV1SetRotationXYZ(mRailHandle[0][i], VGet(0.0f, rail.a, 0.0f));
						mTunnelHandle[i] = MV1DuplicateModel(mTunnelHandleBase);
						MV1SetPosition(mTunnelHandle[i], VGet(rail.x, rail.y + 2.5f, rail.z));
						MV1SetRotationXYZ(mTunnelHandle[i], VGet(0.0f, rail.a, 0.0f));
						rail.x += sin(rail.a);
						rail.z += cos(rail.a);
						mRailPosition[drawDistance][0] = rail.z;
						mRailPosition[drawDistance][1] = rail.a;
						drawDistance++;
					}
					soundHandle[0] = LoadSoundMem(L"Assets\\Sound\\Inverter.wav");
					PlaySoundMem(soundHandle[0], DX_PLAYTYPE_LOOP);
					ChangeVolumeSoundMem(0, soundHandle[0]);
					game.status = 1;
				} else if (game.status == 1) {
					DrawFillBox(0, 0, screenWidth, screenHeight, COLOR_BLACK);
					ControlVVVFSound(navi, soundHandle[0]);
					SetCameraNearFar(0.1f, 1000.0f);
					double cameraMove = navi.speed * 1000 / 60 / 60 / fps.Get();
					point.x += cameraMove * sin(point.a);
					point.z += cameraMove * cos(point.a);
					camera.x += cameraMove * sin(point.a);
					camera.z += cameraMove * cos(point.a);
					cameraZDistance += cameraMove * cos(point.a);
					point.a = mRailPosition[runDistance][1];
					if (mRailPosition[runDistance][0] <= camera.z) {
						cameraZDistance = 0.0f;
						cameraZLength = cos(point.a);
						runDistance++;
						navi = UpdateATCSpeed(navi, runDistance);
						rail.a = GetRailAngle(drawDistance, rail.a, navi.section);
						mRailPosition[drawDistance][0] = rail.z;
						mRailPosition[drawDistance][1] = rail.a;
						MV1SetPosition(mRailHandle[0][drawDistance % C_DISTANCE], VGet(rail.x, rail.y, rail.z));
						MV1SetRotationXYZ(mRailHandle[0][drawDistance % C_DISTANCE], VGet(0.0f, rail.a, 0.0f));
						MV1SetPosition(mTunnelHandle[drawDistance % C_DISTANCE], VGet(rail.x, rail.y + 2.5f, rail.z));
						MV1SetRotationXYZ(mTunnelHandle[drawDistance % C_DISTANCE], VGet(0.0f, rail.a, 0.0f));
						rail.x += sin(rail.a);
						rail.z += cos(rail.a);
						if (drawDistance >= 775 && drawDistance <= 900) {
							mPlatformHandle[0][drawDistance - 775] = MV1DuplicateModel(mPlatformHandleBase);
							MV1SetPosition(mPlatformHandle[0][drawDistance - 775], VGet(rail.x + 4.5f, rail.y, rail.z));
							MV1SetRotationXYZ(mPlatformHandle[0][drawDistance - 775], VGet(0.0f, rail.a, 0.0f));
						}
						drawDistance++;
						navi.distance--;
					}
					if (fabsf(point.a - cameraAngle) > 0.001f) {
						if (point.a < cameraAngle) {
							cameraAngle -= fabsf(point.a - cameraAngle) / 10.0f;
						} else if (point.a > cameraAngle) {
							cameraAngle += fabsf(point.a - cameraAngle) / 10.0f;
						}
					} else {
						cameraAngle = point.a;
					}
					if (game.clock == 0) {
						navi.time++;
					}
					SetCameraPositionAndAngle(camera, 0.0f, cameraAngle, 0.0f);
					Draw3DRail(mRailHandle, mTunnelHandle, mPlatformHandle);
					navi = UpdateNotch(key, joypad, navi, train);
					navi = UpdateSpeed(navi, train, fps);
					DebugInformation(navi);
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
