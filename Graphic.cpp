#include <cmath>
#include <math.h>
#include "DxLib.h"
#include "Common.h"
#include "Math.h"
#include "Graphic.h"

void DebugInformation(Navi navi) {
	ChangeFontFromHandle(FONT_DEBUG);
	DrawFormatString(30, 30, COLOR_WHITE, L"時間: %d 秒", navi.time);
	DrawFormatString(30, 55, COLOR_WHITE, L"目標時間: %d 秒", navi.arrtime);
	DrawFormatString(30, 80, COLOR_WHITE, L"速度: %.1f km/h (ATC %d)", navi.speed, navi.atc);
	DrawFormatString(30, 105, COLOR_WHITE, L"停止位置まであと %d m", navi.distance);
	DrawFormatString(30, 130, COLOR_WHITE, L"力行: %d", navi.p);
	DrawFormatString(30, 155, COLOR_WHITE, L"ブレーキ: %d", navi.b);
}

void DrawCab(int bufferHandle, int backgroundHandle, Navi navi) {
	SetDrawScreen(bufferHandle);
	ClearDrawScreen();
	DrawGraph(0, 0, backgroundHandle, TRUE);
	double radSpeed = DegreeToRadian(210.0 + 2.5 * navi.speed);
	DrawLineAA(795, 818, 795 + 76 * sin(radSpeed), 818 - 76 * cos(radSpeed), GetColor(0, 0, 0), 6.0f);
	SetDrawScreen(DX_SCREEN_BACK);
}
