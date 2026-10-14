#include <math.h>
#include "DxLib.h"
#include "Common.h"
#include "Control.h"
#include "Graphic.h"
#include "Interface.h"
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
	float cameraAngle[2] = { 0.0f, 0.0f };
	Position rail;
	Position point;
	Navi navi;
	Train train = { 120, 3.0, 3.5, 4.0, 8, 5, {0, 50, 80, 110, 140, 170, 200, 230, 260} };
	BrakePressure brakePressure = { 0, 0, 0, 0.0 };
	BrakePressure current = { 0, 0, 0, 0.0 };

	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	SetUse3DFlag(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWindowIconID(100);
	SetMainWindowText(L"もっと! 地下鉄 大名古屋");
	int savedata[2][4] = {
		{ 0, 7, 0, 0 },
		{ 3, 0, 0, 0 },
	};
	FILE* fp;
	errno_t error;
	error = fopen_s(&fp, "savedata", "r");
	if (fp == NULL) {
		error = fopen_s(&fp, "savedata", "wb");
		if (fp != NULL) {
			fwrite(&savedata, sizeof(savedata), 1, fp);
			fclose(fp);
		}
	} else {
		fclose(fp);
		error = fopen_s(&fp, "savedata", "rb");
		if (fp != NULL) {
			fread(&savedata, sizeof(savedata), 1, fp);
			fclose(fp);
		}
	}
	if (savedata[1][0] == 0) {
		ChangeWindowMode(FALSE);
		screenWidth = 1920;
		screenHeight = 1080;
	} else {
		ChangeWindowMode(TRUE);
		if (savedata[1][0] == 1) {
			screenWidth = 1920;
			screenHeight = 1080;
		} else if (savedata[1][0] == 2) {
			screenWidth = 1600;
			screenHeight = 900;
		} else if (savedata[1][0] == 3) {
			screenWidth = 1366;
			screenHeight = 768;
		} else if (savedata[1][0] == 4) {
			screenWidth = 1280;
			screenHeight = 720;
		}
	}
	SetGraphMode(screenWidth, screenHeight, 32);
	SetWindowSize(screenWidth, screenHeight);
	if (DxLib_Init() == -1) {
		return -1;
	}

	int bufferHandle = MakeScreen(BUFFER_WIDTH, BUFFER_HEIGHT, TRUE);
	int backgroundHandle[2];
	backgroundHandle[1] = LoadGraph(L"Assets\\Image\\Cab.png");
	int spriteHandle[16];
	spriteHandle[1] = LoadGraph(L"Assets\\Image\\Tablet.png");
	spriteHandle[2] = LoadGraph(L"Assets\\Image\\Station.png");
	spriteHandle[3] = LoadGraph(L"Assets\\Image\\CommonMap.png");
	InitSound();

	float mRailPosition[2000][3];
	int mRailHandle[4][200];
	int mTunnelHandle[2][200];
	int mPlatformHandle[2][200];
	int mPlatformHandleBase[2];
	static const int C_DISTANCE = sizeof(mRailHandle[0]) / sizeof(mRailHandle[0][0]);
	const int mStopHandle = MV1LoadModel(L"Assets\\Model\\Sign\\Stop.mqo");
	const int mRailHandleBase = MV1LoadModel(L"Assets\\Model\\Rail\\1067.mqo");
	const int mTunnelHandleBase = MV1LoadModel(L"Assets\\Model\\Tunnel\\Sakuradori1.mqo");
	const int mStationHandleBase = MV1LoadModel(L"Assets\\Model\\Tunnel\\Sakuradori2.mqo");

	int runDistance, totalDistance, drawDistance, drawStart;
	int pad, padX, padY;
	int padNum = GetJoypadNum();
	if (padNum >= 1) {
		isJoypad = true;
		SetJoypadDeadZone(DX_INPUT_PAD1, 0.0);
	} else {
		savedata[1][1] = 0;
	}
	int controlBuffer;
	bool status = true;
	bool mclean = false;

	Fps fps;
	Game game = {
		-1,		// mode
		0,		// submode
		0,		// status
		0,		// count
		0		// clock
	};

	for (int i = 0; i < C_DISTANCE; i++) {
		for (int j = 0; j < 2; j++) {
			mTunnelHandle[j][i] = -1;
			mRailHandle[j][i] = -1;
			mPlatformHandle[j][i] = -1;
		}
	}

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
					PlaySoundMem(soundHandle[4], DX_PLAYTYPE_BACK);
					game.status = 0;
					game.submode = 3;
					game.mode = 2;
					mclean = true;
				}
				if (key[KEY_INPUT_ESCAPE] == 1) {
					status = false;
				}
				break;
			case 2:
				if (game.status == 0) {
					backgroundHandle[0] = LoadGraph(L"Assets\\Image\\Menu.jpg");
					spriteHandle[0] = LoadGraph(L"Assets\\Image\\MenuMap.png");
					game.status = 1;
				} else if (game.status == 1) {
					SetDrawScreen(bufferHandle);
					DrawGraph(0, 120, backgroundHandle[0], FALSE);
					DrawInterfaceHeader(spriteHandle[3], spriteHandle[0]);
					for (int i = 0; i < 3; i++) {
						DrawRectGraph(145, 190 + (i * 220), 0, 140, 800, 220, spriteHandle[0], TRUE);
						DrawRectGraph(995, 190 + (i * 220), 0, 140, 800, 220, spriteHandle[0], TRUE);
					}
					int c = 0;
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 2; j++) {
							if (game.submode == c) {
								SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
							} else {
								SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
							}
							DrawRectGraph(155 + (j * 850), 200 + (i * 220), 0, 380 + (c * 180), 760, 180, spriteHandle[0], FALSE);
							c++;
						}
					}
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					DrawInterfaceFooter(spriteHandle[3], savedata[1][1]);
					SetDrawScreen(DX_SCREEN_BACK);
					game.status = 2;
				}
				DrawExtendGraph(0, 0, screenWidth, screenHeight, bufferHandle, FALSE);
				if ((key[KEY_INPUT_UP] == 1 || joypad[PAD_UP] == 1) && game.submode == 5) {
					PlaySoundMem(soundHandle[5], DX_PLAYTYPE_BACK);
					game.submode = 3;
					game.status = 1;
				}
				if ((key[KEY_INPUT_DOWN] == 1 || joypad[PAD_DOWN] == 1) && game.submode == 3) {
					PlaySoundMem(soundHandle[5], DX_PLAYTYPE_BACK);
					game.submode = 5;
					game.status = 1;
				}
				if (key[KEY_INPUT_SPACE] == 1 || joypad[PAD_3] == 1) {
					PlaySoundMem(soundHandle[4], DX_PLAYTYPE_BACK);
					mclean = true;
					switch (game.submode) {
						case 3:
							PlaySoundMem(soundHandle[4], DX_PLAYTYPE_BACK);
							navi.cb = 9;
							navi.cp = 0;
							navi.section = 2620;
							navi.score = 100;
							navi.autobrake = false;
							game.count = -10;
							game.status = 0;
							game.clock = 0;
							game.mode = 100;
							totalDistance = 0;
							break;
						case 5:
							game.count = -10;
							game.status = 0;
							game.mode = 3;
							break;
					}
				}
				if (key[KEY_INPUT_ESCAPE] == 1) {
					PlaySoundMem(soundHandle[1], DX_PLAYTYPE_BACK);
					game.count = -10;
					game.mode = 1;
				}
				break;
			case 3:
				if (game.status == 0) {
					backgroundHandle[0] = LoadGraph(L"Assets\\Image\\Menu.jpg");
					spriteHandle[0] = LoadGraph(L"Assets\\Image\\SettingsMap.png");
					game.submode = 0;
					game.status = 1;
				} else if (game.status == 1) {
					SetDrawScreen(bufferHandle);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					DrawGraph(0, 0, backgroundHandle[0], FALSE);
					DrawInterfaceHeader(spriteHandle[3], spriteHandle[0]);
					DrawRectGraph(105, 185, 15, 165, 1710, 130, spriteHandle[0], TRUE);
					DrawRectGraph(105, 335, 15, 165, 1710, 130, spriteHandle[0], TRUE);
					// DrawRectGraph(105, 485, 15, 165, 1710, 130, spriteHandle[0], TRUE);
					DrawRectGraph(102, 150 * game.submode + 182, 12, 302, 1716, 136, spriteHandle[0], TRUE);
					DrawRectGraph(120, 200, 30, 450, 1680, 100, spriteHandle[0], TRUE);
					DrawRectGraph(120, 350, 30, 450, 1680, 100, spriteHandle[0], TRUE);
					// DrawRectGraph(120, 500, 30, 450, 1680, 100, spriteHandle[0], TRUE);
					DrawRectGraph(150, 215, 20, 565, 600, 70, spriteHandle[0], TRUE);
					DrawRectGraph(150, 365, 20, 635, 600, 70, spriteHandle[0], TRUE);
					// DrawRectGraph(150, 515, 20, 705, 600, 70, spriteHandle[0], TRUE);
					DrawRectGraph(1077, 213, 675, savedata[1][0] * 70 + 563, 495, 70, spriteHandle[0], TRUE);
					DrawRectGraph(1045, 363, 1195, savedata[1][1] * 70 + 563, 560, 70, spriteHandle[0], TRUE);
					// DrawRectGraph(1045, 513, 1195, savedata[1][2] * 70 + 773, 560, 70, spriteHandle[0], TRUE);
					DrawInterfaceFooter(spriteHandle[3], savedata[1][1]);
					SetDrawScreen(DX_SCREEN_BACK);
					game.status = 2;
				} else if (game.status == 2) {
					if (key[KEY_INPUT_UP] == 1 || joypad[PAD_UP] == 1) {
						if (game.submode > 0) {
							PlaySoundMem(soundHandle[5], DX_PLAYTYPE_BACK);
							game.submode--;
							game.status = 1;
						}
					}
					if (key[KEY_INPUT_DOWN] == 1 || joypad[PAD_DOWN] == 1) {
						if (game.submode < 1) {
							PlaySoundMem(soundHandle[5], DX_PLAYTYPE_BACK);
							game.submode++;
							game.status = 1;
						}
					}
					if (key[KEY_INPUT_LEFT] == 1 || joypad[PAD_LEFT] == 1) {
						if (savedata[1][game.submode] > 0) {
							PlaySoundMem(soundHandle[5], DX_PLAYTYPE_BACK);
							savedata[1][game.submode]--;
							game.status = 1;
						}
					}
					if (key[KEY_INPUT_RIGHT] == 1 || joypad[PAD_RIGHT] == 1) {
						if (game.submode == 0 && savedata[1][0] < 4) {
							PlaySoundMem(soundHandle[5], DX_PLAYTYPE_BACK);
							savedata[1][0]++;
							game.status = 1;
						}
						if (game.submode == 1 && savedata[1][1] < 2) {
							PlaySoundMem(soundHandle[5], DX_PLAYTYPE_BACK);
							savedata[1][1]++;
							game.status = 1;
						}
						if (game.submode == 2 && savedata[1][2] < 1) {
							PlaySoundMem(soundHandle[5], DX_PLAYTYPE_BACK);
							savedata[1][2]++;
							game.status = 1;
						}
					}
					if (key[KEY_INPUT_SPACE] == 1 || joypad[PAD_3] == 1) {
						PlaySoundMem(soundHandle[4], DX_PLAYTYPE_NORMAL);
						error = fopen_s(&fp, "savedata", "wb");
						if (fp != NULL) {
							fwrite(&savedata, sizeof(savedata), 1, fp);
							fclose(fp);
						}
						game.status = 10;
					}
					if (key[KEY_INPUT_ESCAPE] == 1) {
						PlaySoundMem(soundHandle[1], DX_PLAYTYPE_BACK);
						game.status = 10;
					}
				} else if (game.status == 10) {
					game.count = -10;
					game.status = 0;
					game.submode = 5;
					game.mode = 2;
					mclean = true;
				}
				DrawExtendGraph(0, 0, screenWidth, screenHeight, bufferHandle, FALSE);
				break;
			case 100:
				if (game.status == 0) {
					spriteHandle[0] = LoadGraph(L"Assets\\Image\\GameMap.png");
					rail = { 0.0f, 0.0f, 0.0f, 0.0f };
					point = { 0.0f, 0.0f, 0.0f, 0.0f };
					navi.speed = 0.0;
					if (navi.cb == 0) {
						navi.cb = 1;
					}
					if (navi.cp != 0) {
						navi.cp = 0;
					}
					if (navi.section == 2620) {
						navi.time = -25;
						navi.arrtime = 85;
						navi.atc = 0;
						navi.distance = 860;
						mPlatformHandleBase[0] = MV1LoadModel(L"Assets\\Model\\Station\\Platform\\620.mqo");
						mPlatformHandleBase[1] = MV1LoadModel(L"Assets\\Model\\Station\\Platform\\619.mqo");
						soundHandle[14] = LoadSoundMem(L"Assets\\Sound\\Announcement\\62200.wav");
						soundHandle[15] = LoadSoundMem(L"Assets\\Sound\\Announcement\\62201.wav");
					} else if (navi.section == 2619) {
						navi.time = -14;
						navi.arrtime = 120;
						navi.distance = 1370;
						mPlatformHandleBase[0] = MV1LoadModel(L"Assets\\Model\\Station\\Platform\\619.mqo");
						mPlatformHandleBase[1] = MV1LoadModel(L"Assets\\Model\\Station\\Platform\\618.mqo");
						soundHandle[14] = LoadSoundMem(L"Assets\\Sound\\Announcement\\62190.wav");
						soundHandle[15] = LoadSoundMem(L"Assets\\Sound\\Announcement\\62191.wav");
					}
					camera = VGet(0.0f, 0.5f, 0.0f);
					cameraAngle[0] = 0.0f;
					cameraAngle[1] = 0.0f;
					runDistance = 0;
					if (navi.section == 2620) {
						drawDistance = 0;
					} else if (navi.section == 2619) {
						drawDistance = 860;
					}
					drawStart = drawDistance;
					for (int i = 0; i < C_DISTANCE; i++) {
						mRailHandle[0][i] = MV1DuplicateModel(mRailHandleBase);
						rail = GetRailAngle(drawDistance, rail);
						MV1SetPosition(mRailHandle[0][i], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 0.0f, -2.229f));
						MV1SetRotationXYZ(mRailHandle[0][i], VGet(-rail.ay, rail.ax, 0.0f));
						if (i == 2) {
							MV1SetPosition(mStopHandle, GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 0.0f, -2.229f));
							MV1SetRotationXYZ(mStopHandle, VGet(-rail.ay, rail.ax, 0.0f));
						}
						if (i <= 10) {
							mTunnelHandle[0][i] = MV1DuplicateModel(mStationHandleBase);
							mTunnelHandle[1][i] = MV1DuplicateModel(mStationHandleBase);
							mPlatformHandle[0][i] = MV1DuplicateModel(mPlatformHandleBase[0]);
							MV1SetRotationXYZ(mPlatformHandle[0][i], VGet(-rail.ay, rail.ax, 0.0f));
							mPlatformHandle[1][i] = MV1DuplicateModel(mPlatformHandleBase[0]);
							MV1SetRotationXYZ(mPlatformHandle[1][i], VGet(-rail.ay, DX_PI_F + rail.ax, 0.0f));
							if (navi.section == 2620) {
								MV1SetPosition(mPlatformHandle[0][i], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 4.5f, -2.229f - 0.175f));
								MV1SetPosition(mPlatformHandle[1][i], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 4.5f, -2.229f - 0.175f));
							} else if (navi.section == 2619) {
								MV1SetPosition(mPlatformHandle[0][i], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 4.0f, -2.229f - 0.175f));
								MV1SetPosition(mPlatformHandle[1][i], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 4.0f, -2.229f - 0.175f));
							}
						} else {
							mTunnelHandle[0][i] = MV1DuplicateModel(mTunnelHandleBase);
							if (i < 40) {
								mTunnelHandle[1][i] = MV1DuplicateModel(mTunnelHandleBase);
							}
						}
						MV1SetPosition(mTunnelHandle[0][i], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 0.0f, 0.0f));
						MV1SetRotationXYZ(mTunnelHandle[0][i], VGet(-rail.ay, rail.ax, 0.0f));
						if (i < 40) {
							if (navi.section == 2620) {
								MV1SetPosition(mTunnelHandle[1][i], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 8.8f, 0.0f));
							} else if (navi.section == 2619) {
								MV1SetPosition(mTunnelHandle[1][i], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 7.8f, 0.0f));
							}
							MV1SetRotationXYZ(mTunnelHandle[1][i], VGet(-rail.ay, DX_PI_F + rail.ax, 0.0f));
						}
						rail.x += sinf(rail.ax);
						rail.y += sinf(rail.ay);
						rail.z += fabsf(sinf(rail.ay)) + fabsf(cosf(rail.ax) * cosf(rail.ay));
						mRailPosition[i][0] = rail.z;
						mRailPosition[i][1] = rail.ax;
						mRailPosition[i][2] = rail.ay;
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
					float cameraMove = static_cast<float>(navi.speed * 1000.0 / 60.0 / 60.0 / fps.Get());
					point.x += cameraMove * sinf(point.ax);
					point.y += cameraMove * sinf(point.ay);
					point.z += cameraMove * (fabsf(sinf(point.ay)) + fabsf(cosf(point.ax) * cosf(point.ay)));
					camera.x += cameraMove * sinf(point.ax);
					camera.y += cameraMove * sinf(point.ay);
					camera.z += cameraMove * (fabsf(sinf(point.ay)) + fabsf(cosf(point.ax) * cosf(point.ay)));
					cameraZDistance += cameraMove * (fabsf(sinf(point.ay)) + fabsf(cosf(point.ax) * cosf(point.ay)));
					point.ax = mRailPosition[runDistance][1];
					point.ay = mRailPosition[runDistance][2];
					if (mRailPosition[runDistance][0] <= camera.z) {
						cameraZDistance = 0.0f;
						cameraZLength = fabsf(sinf(point.ay)) + fabsf(cosf(point.ax) * cosf(point.ay));
						runDistance++;
						totalDistance++;
						navi = UpdateATCSpeed(navi, totalDistance);
						rail = GetRailAngle(drawDistance, rail);
						mRailPosition[drawDistance - drawStart][0] = rail.z;
						mRailPosition[drawDistance - drawStart][1] = rail.ax;
						mRailPosition[drawDistance - drawStart][2] = rail.ay;
						MV1SetPosition(mRailHandle[0][(drawDistance - drawStart) % C_DISTANCE], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 0.0f, -2.229f));
						MV1SetRotationXYZ(mRailHandle[0][(drawDistance - drawStart) % C_DISTANCE], VGet(-rail.ay, rail.ax, 0.0f));
						if (drawDistance >= 200 && drawDistance <= 210) {
							DeleteObject(mPlatformHandle[0][(drawDistance - drawStart) % C_DISTANCE]);
							DeleteObject(mTunnelHandle[0][(drawDistance - drawStart) % C_DISTANCE]);
							mTunnelHandle[0][(drawDistance - drawStart) % C_DISTANCE] = MV1DuplicateModel(mTunnelHandleBase);
							DeleteObject(mPlatformHandle[1][(drawDistance - drawStart) % C_DISTANCE]);
						}
						if (drawDistance == 745 || drawDistance == 2115) {
							for (int i = 0; i < 165; i++) {
								if (i < 125) {
									mTunnelHandle[1][i] = MV1DuplicateModel(mStationHandleBase);
								} else {
									mTunnelHandle[1][i] = MV1DuplicateModel(mTunnelHandleBase);
								}
							}
						}
						if (drawDistance >= 745 && drawDistance <= 870) {
							mPlatformHandle[0][drawDistance - 745] = MV1DuplicateModel(mPlatformHandleBase[1]);
							MV1SetPosition(mPlatformHandle[0][drawDistance - 745], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 4.0f, -2.229f - 0.175f));
							MV1SetRotationXYZ(mPlatformHandle[0][drawDistance - 745], VGet(-rail.ay, rail.ax, 0.0f));
							mPlatformHandle[1][drawDistance - 745] = MV1DuplicateModel(mPlatformHandleBase[1]);
							MV1SetPosition(mPlatformHandle[1][drawDistance - 745], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 4.0f, -2.229f - 0.175f));
							MV1SetRotationXYZ(mPlatformHandle[1][drawDistance - 745], VGet(-rail.ay, DX_PI_F + rail.ax, 0.0f));
							DeleteObject(mTunnelHandle[0][(drawDistance - drawStart) % C_DISTANCE]);
							mTunnelHandle[0][(drawDistance - drawStart) % C_DISTANCE] = MV1DuplicateModel(mStationHandleBase);
							MV1SetPosition(mTunnelHandle[1][drawDistance - 745], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 7.8f, 0.0f));
							MV1SetRotationXYZ(mTunnelHandle[1][drawDistance - 745], VGet(-rail.ay, DX_PI_F + rail.ax, 0.0f));
						}
						if (drawDistance >= 870 && drawDistance < 910) {
							MV1SetPosition(mTunnelHandle[1][drawDistance - 745], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 7.8f, 0.0f));
							MV1SetRotationXYZ(mTunnelHandle[1][drawDistance - 745], VGet(-rail.ay, DX_PI_F + rail.ax, 0.0f));
						}
						if (drawDistance >= 2115 && drawDistance <= 2240) {
							mPlatformHandle[0][drawDistance - 2115] = MV1DuplicateModel(mPlatformHandleBase[1]);
							MV1SetPosition(mPlatformHandle[0][drawDistance - 2115], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 3.95f, -2.229f - 0.175f));
							MV1SetRotationXYZ(mPlatformHandle[0][drawDistance - 2115], VGet(-rail.ay, rail.ax, 0.0f));
							mPlatformHandle[1][drawDistance - 2115] = MV1DuplicateModel(mPlatformHandleBase[1]);
							MV1SetPosition(mPlatformHandle[1][drawDistance - 2115], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 3.95f, -2.229f - 0.175f));
							MV1SetRotationXYZ(mPlatformHandle[1][drawDistance - 2115], VGet(-rail.ay, DX_PI_F + rail.ax, 0.0f));
							DeleteObject(mTunnelHandle[0][(drawDistance - drawStart) % C_DISTANCE]);
							mTunnelHandle[0][(drawDistance - drawStart) % C_DISTANCE] = MV1DuplicateModel(mStationHandleBase);
							MV1SetPosition(mTunnelHandle[1][drawDistance - 2115], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 7.7f, 0.0f));
							MV1SetRotationXYZ(mTunnelHandle[1][drawDistance - 2115], VGet(-rail.ay, DX_PI_F + rail.ax, 0.0f));
						}
						if (drawDistance >= 2240 && drawDistance < 2280) {
							MV1SetPosition(mTunnelHandle[1][drawDistance - 2115], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 7.7f, 0.0f));
							MV1SetRotationXYZ(mTunnelHandle[1][drawDistance - 2115], VGet(-rail.ay, DX_PI_F + rail.ax, 0.0f));
						}
						if ((drawDistance >= 945 && drawDistance <= 1070) || (drawDistance >= 2315 && drawDistance <= 2440)) {
							DeleteObject(mTunnelHandle[0][(drawDistance - drawStart) % C_DISTANCE]);
							mTunnelHandle[0][(drawDistance - drawStart) % C_DISTANCE] = MV1DuplicateModel(mTunnelHandleBase);
						}
						if (drawDistance == 865 || drawDistance == 2235) {
							MV1SetPosition(mStopHandle, GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 0.0f, -2.229f));
							MV1SetRotationXYZ(mStopHandle, VGet(-rail.ay, rail.ax, 0.0f));
						}
						if (runDistance == 40) {
							for (int i = 0; i < 40; i++) {
								DeleteObject(mTunnelHandle[1][i]);
							}
						}
						if (runDistance == 20) {
							PlaySoundMem(soundHandle[14], DX_PLAYTYPE_BACK);
						}
						if (navi.distance == 200) {
							PlaySoundMem(soundHandle[15], DX_PLAYTYPE_BACK);
						}
						if (navi.distance == -5) {
							navi = SetATCSpeed(navi, 0);
						}
						MV1SetPosition(mTunnelHandle[0][(drawDistance - drawStart) % C_DISTANCE], GetPosition(rail.x, rail.y, rail.z, -rail.ay, rail.ax, 0.0f, 0.0f));
						MV1SetRotationXYZ(mTunnelHandle[0][(drawDistance - drawStart) % C_DISTANCE], VGet(-rail.ay, rail.ax, 0.0f));
						rail.x += sinf(rail.ax);
						rail.y += sinf(rail.ay);
						rail.z += fabsf(sinf(rail.ay)) + fabsf(cosf(rail.ax) * cosf(rail.ay));
						drawDistance++;
						navi.distance--;
					}
					if (fabsf(point.ax - cameraAngle[0]) > 0.001f) {
						if (point.ax < cameraAngle[0]) {
							cameraAngle[0] -= fabsf(point.ax - cameraAngle[0]) / 10.0f;
						} else if (point.ax > cameraAngle[0]) {
							cameraAngle[0] += fabsf(point.ax - cameraAngle[0]) / 10.0f;
						}
					} else {
						cameraAngle[0] = point.ax;
					}
					if (fabsf(point.ay - cameraAngle[1]) > 0.001f) {
						if (point.ay < cameraAngle[1]) {
							cameraAngle[1] -= fabsf(point.ay - cameraAngle[1]) / 10.0f;
						} else if (point.ay > cameraAngle[1]) {
							cameraAngle[1] += fabsf(point.ay - cameraAngle[1]) / 10.0f;
						}
					} else {
						cameraAngle[1] = point.ay;
					}
					if (game.clock == 0) {
						navi.time++;
						if (navi.section == 2620 && navi.time == -24) {
							PlaySoundMem(soundHandle[13], DX_PLAYTYPE_BACK);
						}
						if (navi.time == -13) {
							if (navi.section == 2620) {
								navi = SetATCSpeed(navi, 75);
							} else if (navi.section == 2619) {
								if (navi.atc != 55) {
									navi = SetATCSpeed(navi, 55);
								}
							}
						}
						if (navi.time == -12) {
							PlaySoundMem(soundHandle[11], DX_PLAYTYPE_BACK);
						}
						if (navi.time == -1) {
							PlaySoundMem(soundHandle[9], DX_PLAYTYPE_BACK);
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
					SetCameraPositionAndAngle(camera, -cameraAngle[1] + 0.35f, cameraAngle[0], 0.0f);
					Draw3DRail(mRailHandle, mTunnelHandle, mPlatformHandle);
					MV1DrawModel(mStopHandle);
					navi = UpdateNotch(key, joypad, savedata[1][1], navi, train, soundHandle[4], soundHandle[5], soundHandle[1]);
					navi = UpdateSpeed(navi, train, fps, brakePressure, current, cameraAngle[1]);
					brakePressure = UpdateBrakePressure(brakePressure, navi, train, soundHandle[1]);
					current = UpdateCurrent(current, navi, train);
					DrawCab(bufferHandle, backgroundHandle[1], spriteHandle[0], spriteHandle[1], spriteHandle[2], navi, train, brakePressure.out, current.out);
					DrawExtendGraph(0, 0, screenWidth, screenHeight, bufferHandle, TRUE);
				} else if (game.status == 2) {
					DrawFillBox(0, 0, screenWidth, screenHeight, COLOR_BLACK);
					SetCameraNearFar(0.1f, 1000.0f);
					SetCameraPositionAndAngle(camera, -cameraAngle[1] + 0.35f, cameraAngle[0], 0.0f);
					Draw3DRail(mRailHandle, mTunnelHandle, mPlatformHandle);
					MV1DrawModel(mStopHandle);
					DrawCab(bufferHandle, backgroundHandle[1], spriteHandle[0], spriteHandle[1], spriteHandle[2], navi, train, brakePressure.out, current.out);
					DrawExtendGraph(0, 0, screenWidth, screenHeight, bufferHandle, TRUE);
					if (game.count == 60) {
						PlaySoundMem(soundHandle[10], DX_PLAYTYPE_BACK);
						StopSoundMem(soundHandle[0]);
						StopSoundMem(soundHandle[2]);
						StopSoundMem(soundHandle[3]);
					}
					if (navi.section == 2620) {
						if (game.count == 310) {
							mclean = true;
							navi.section = 2619;
							game.count = -10;
							game.status = 0;
							game.clock = 0;
							game.mode = 100;
							mclean = true;
						}
					} else if (navi.section == 2619) {
						if (game.count == 310) {
							PlaySoundMem(soundHandle[12], DX_PLAYTYPE_BACK);
						}
						if (game.count == 490) {
							game.mode = -1;
						}
					}
				}
				break;
		}
		if (mclean == true) {
			for (int i = 0; i < C_DISTANCE; i++) {
				for (int j = 0; j < 2; j++) {
					DeleteObject(mTunnelHandle[j][i]);
					DeleteObject(mRailHandle[j][i]);
					DeleteObject(mPlatformHandle[j][i]);
				}
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
