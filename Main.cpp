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
	int screenWidth, screenHeight;
	VECTOR camera;
	float cameraZLength = 1.0f;
	float cameraZDistance = 0.0f;
	float cameraAngle = 0.0f;
	Position rail;
	Position point;
	Navi navi;
	Train train = { 120, 3.0, 3.5, 4.0, 8, 5, {0, 50, 80, 110, 140, 170, 200, 230, 260} };
	BrakePressure brakePressure = { 0, 0, 0 };
	BrakePressure current = { 0, 0, 0 };

	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	SetUse3DFlag(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWindowIconID(100);
	SetMainWindowText(L"もっと! 地下鉄 大名古屋");
	FILE* fp;
	errno_t error;
	error = fopen_s(&fp, "fullscreen", "r");
	if (fp == NULL) {
		ChangeWindowMode(TRUE);
		screenWidth = 1366;
		screenHeight = 768;
	} else {
		fclose(fp);
		ChangeWindowMode(FALSE);
		screenWidth = 1920;
		screenHeight = 1080;
	}
	SetGraphMode(screenWidth, screenHeight, 32);
	SetWindowSize(screenWidth, screenHeight);
	if (DxLib_Init() == -1) {
		return -1;
	}

	int bufferHandle = MakeScreen(1920, 1080, TRUE);
	int backgroundHandle[2];
	backgroundHandle[1] = LoadGraph(L"Assets\\Image\\Cab.png");
	int spriteHandle[16];
	spriteHandle[0] = LoadGraph(L"Assets\\Image\\GameMap.png");
	spriteHandle[1] = LoadGraph(L"Assets\\Image\\Tablet.png");
	int soundHandle[16];
	soundHandle[0] = LoadSoundMem(L"Assets\\Sound\\Inverter.wav");
	soundHandle[1] = LoadSoundMem(L"Assets\\Sound\\BrakeDecompress.wav");
	soundHandle[2] = LoadSoundMem(L"Assets\\Sound\\BrakeStop.wav");
	soundHandle[3] = LoadSoundMem(L"Assets\\Sound\\Tunnel.wav");
	soundHandle[4] = LoadSoundMem(L"Assets\\Sound\\Notch1.wav");
	soundHandle[5] = LoadSoundMem(L"Assets\\Sound\\Notch2.wav");
	soundHandle[9] = LoadSoundMem(L"Assets\\Sound\\Buzzer.wav");
	soundHandle[10] = LoadSoundMem(L"Assets\\Sound\\DoorOpen.wav");
	soundHandle[11] = LoadSoundMem(L"Assets\\Sound\\DoorClose.wav");
	soundHandle[12] = LoadSoundMem(L"Assets\\Sound\\Announcement\\End.wav");
	soundHandle[13] = LoadSoundMem(L"Assets\\Sound\\Announcement\\62210.wav");
	soundHandle[14] = LoadSoundMem(L"Assets\\Sound\\Announcement\\62200.wav");
	soundHandle[15] = LoadSoundMem(L"Assets\\Sound\\Announcement\\62201.wav");

	float mRailPosition[2000][2];
	int mRailHandle[4][200];
	int mTunnelHandle[200];
	int mPlatformHandle[2][200];
	static const int C_DISTANCE = sizeof(mRailHandle[0]) / sizeof(mRailHandle[0][0]);
	const int mRailHandleBase = MV1LoadModel(L"Assets\\Model\\Rail\\1067.mqo");
	const int mTunnelHandleBase = MV1LoadModel(L"Assets\\Model\\Tunnel\\Sakuradori1.mqo");
	const int mStationHandleBase = MV1LoadModel(L"Assets\\Model\\Tunnel\\Sakuradori2.mqo");
	const int mPlatformHandleBase = MV1LoadModel(L"Assets\\Model\\Station\\Platform\\620.mqo");

	int runDistance, drawDistance;
	int pad, padX, padY;
	int padNum = GetJoypadNum();
	if (padNum >= 1) {
		isJoypad = true;
		SetJoypadDeadZone(DX_INPUT_PAD1, 0.0);
	}
	int controlBuffer;
	bool status = true;
	bool mclean = false;

	Fps fps;
	Game game = {
		-1,		// mode
		0,		// status
		0,		// count
		0		// clock
	};

	while (ProcessMessage() == 0 && status == true) {
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
				if (game.count < 210 && (key[KEY_INPUT_SPACE] == 1 || joypad[PAD_3] == 1)) {
					game.count = 210;
				}
				if (game.count == 300) {
					game.count = -10;
					game.mode = 1;
				}
				if (key[KEY_INPUT_ESCAPE] == 1) {
					status = false;
				}
				break;
			case 1:
				if (game.count < 0) {
					backgroundHandle[0] = LoadGraph(L"Assets\\Image\\Title.png");
					SetDrawScreen(bufferHandle);
					DrawGraph(0, 0, backgroundHandle[0], FALSE);
					SetFont(L"ＭＳ ゴシック", 28, 6);
					DrawString((1920 - GetDrawStringWidth(VERSION, -1)) / 2, 940, VERSION, GetColor(255, 255, 255));
					DrawString((1920 - GetDrawStringWidth(COPYRIGHT, -1)) / 2, 974, COPYRIGHT, GetColor(255, 255, 255));
					SetDrawScreen(DX_SCREEN_BACK);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					game.count = 0;
				} else {
					DrawExtendGraph(0, 0, screenWidth, screenHeight, bufferHandle, FALSE);
				}
				if (key[KEY_INPUT_SPACE] == 1 || joypad[PAD_3] == 1) {
					game.count = -10;
					game.status = 0;
					game.clock = 0;
					game.mode = 100;
				}
				if (key[KEY_INPUT_ESCAPE] == 1) {
					status = false;
				}
				break;
			case 100:
				if (game.status == 0) {
					rail = { 0.0f, 0.0f, 0.0f, 0.0f };
					point = { 0.0f, 0.0f, 0.0f, 0.0f };
					navi = { 9, 0, -25, 80, 0, 55, 895, 2620, 100 };
					camera = VGet(0.0f, 2.7f, 0.0f);
					cameraAngle = 0.0f;
					runDistance = 0;
					drawDistance = 0;
					for (int i = 0; i < C_DISTANCE; i++) {
						mRailHandle[0][i] = MV1DuplicateModel(mRailHandleBase);
						rail.a = GetRailAngle(i, rail.a, navi.section);
						MV1SetPosition(mRailHandle[0][i], VGet(rail.x, rail.y, rail.z));
						MV1SetRotationXYZ(mRailHandle[0][i], VGet(0.0f, rail.a, 0.0f));
						if (i <= 10) {
							mTunnelHandle[i] = MV1DuplicateModel(mStationHandleBase);
							mPlatformHandle[0][i] = MV1DuplicateModel(mPlatformHandleBase);
							MV1SetPosition(mPlatformHandle[0][i], VGet(rail.x + 4.5f, rail.y, rail.z));
							MV1SetRotationXYZ(mPlatformHandle[0][i], VGet(0.0f, rail.a, 0.0f));
						} else {
							mTunnelHandle[i] = MV1DuplicateModel(mTunnelHandleBase);
						}
						MV1SetPosition(mTunnelHandle[i], VGet(rail.x, rail.y + 4.5f, rail.z));
						MV1SetRotationXYZ(mTunnelHandle[i], VGet(0.0f, rail.a, 0.0f));
						rail.x += sin(rail.a);
						rail.z += cos(rail.a);
						mRailPosition[drawDistance][0] = rail.z;
						mRailPosition[drawDistance][1] = rail.a;
						drawDistance++;
					}
					PlaySoundMem(soundHandle[0], DX_PLAYTYPE_LOOP);
					ChangeVolumeSoundMem(0, soundHandle[0]);
					PlaySoundMem(soundHandle[2], DX_PLAYTYPE_LOOP);
					ChangeVolumeSoundMem(0, soundHandle[2]);
					PlaySoundMem(soundHandle[3], DX_PLAYTYPE_LOOP);
					ChangeVolumeSoundMem(0, soundHandle[3]);
					game.status = 1;
				} else if (game.status == 1) {
					DrawFillBox(0, 0, screenWidth, screenHeight, COLOR_BLACK);
					ControlVVVFSound(soundHandle[0], navi, current.out);
					ControlBrakeSound(soundHandle[2], navi);
					ControlTunnelSound(soundHandle[3], navi);
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
						if (drawDistance >= 775 && drawDistance <= 900) {
							mPlatformHandle[0][drawDistance - 775] = MV1DuplicateModel(mPlatformHandleBase);
							MV1SetPosition(mPlatformHandle[0][drawDistance - 775], VGet(rail.x + 4.5f, rail.y, rail.z));
							MV1SetRotationXYZ(mPlatformHandle[0][drawDistance - 775], VGet(0.0f, rail.a, 0.0f));
							MV1DeleteModel(mTunnelHandle[drawDistance % C_DISTANCE]);
							mTunnelHandle[drawDistance % C_DISTANCE] = MV1DuplicateModel(mStationHandleBase);
						}
						if (drawDistance >= 200 && drawDistance <= 210) {
							MV1DeleteModel(mPlatformHandle[0][drawDistance % C_DISTANCE]);
							MV1DeleteModel(mTunnelHandle[drawDistance % C_DISTANCE]);
							mTunnelHandle[drawDistance % C_DISTANCE] = MV1DuplicateModel(mTunnelHandleBase);
						}
						if (drawDistance >= 975 && drawDistance <= 1100) {
							MV1DeleteModel(mTunnelHandle[drawDistance % C_DISTANCE]);
							mTunnelHandle[drawDistance % C_DISTANCE] = MV1DuplicateModel(mTunnelHandleBase);
						}
						if (navi.distance == 850) {
							PlaySoundMem(soundHandle[14], DX_PLAYTYPE_BACK);
						}
						if (navi.distance == 200) {
							PlaySoundMem(soundHandle[15], DX_PLAYTYPE_BACK);
						}
						MV1SetPosition(mTunnelHandle[drawDistance % C_DISTANCE], VGet(rail.x, rail.y + 4.5f, rail.z));
						MV1SetRotationXYZ(mTunnelHandle[drawDistance % C_DISTANCE], VGet(0.0f, rail.a, 0.0f));
						rail.x += sin(rail.a);
						rail.z += cos(rail.a);
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
						if (navi.time == -24) {
							PlaySoundMem(soundHandle[13], DX_PLAYTYPE_BACK);
						}
						if (navi.time == -12) {
							PlaySoundMem(soundHandle[11], DX_PLAYTYPE_BACK);
						}
						if (navi.time == -1) {
							PlaySoundMem(soundHandle[9], DX_PLAYTYPE_BACK);
						}
						if (navi.speed >= 76) {
							navi.score -= navi.speed - 75;
						}
						if (navi.speed >= 1 && (navi.time <= -2 || navi.b == train.b + 1)) {
							navi.score -= 2;
						}
					}
					if (navi.speed == 0 && navi.distance <= 5) {
						navi.score -= abs(navi.distance);
						navi.score -= abs(navi.arrtime - navi.time);
						navi.score -= (navi.b - 1) * 2;
						game.status = 2;
						game.count = 0;
					}
					if (navi.score < 0) {
						navi.score = 0;
					}
					SetCameraPositionAndAngle(camera, 0.0f, cameraAngle, 0.0f);
					Draw3DRail(mRailHandle, mTunnelHandle, mPlatformHandle);
					navi = UpdateNotch(key, joypad, navi, train, soundHandle[4], soundHandle[5], soundHandle[1]);
					navi = UpdateSpeed(navi, train, fps);
					brakePressure = UpdateBrakePressure(brakePressure, navi, train);
					current = UpdateCurrent(current, navi, train);
					DrawCab(bufferHandle, backgroundHandle[1], spriteHandle[0], spriteHandle[1], navi, train, brakePressure.out, current.out);
					DrawExtendGraph(0, 0, screenWidth, screenHeight, bufferHandle, TRUE);
				} else if (game.status == 2) {
					DrawFillBox(0, 0, screenWidth, screenHeight, COLOR_BLACK);
					SetCameraNearFar(0.1f, 1000.0f);
					SetCameraPositionAndAngle(camera, 0.0f, cameraAngle, 0.0f);
					Draw3DRail(mRailHandle, mTunnelHandle, mPlatformHandle);
					DrawCab(bufferHandle, backgroundHandle[1], spriteHandle[0], spriteHandle[1], navi, train, brakePressure.out, current.out);
					DrawExtendGraph(0, 0, screenWidth, screenHeight, bufferHandle, TRUE);
					if (game.count == 60) {
						PlaySoundMem(soundHandle[10], DX_PLAYTYPE_BACK);
					}
					if (game.count == 310) {
						PlaySoundMem(soundHandle[12], DX_PLAYTYPE_BACK);
					}
					if (game.count == 490) {
						mclean = true;
						game.mode = -1;
					}
				}
				break;
		}
		if (mclean == true) {
			for (int i = 0; i < 200; i++) {
				MV1DeleteModel(mRailHandle[0][i]);
				MV1DeleteModel(mPlatformHandle[0][i]);
			}
			mclean = false;
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
