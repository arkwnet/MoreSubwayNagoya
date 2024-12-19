#include <cmath>
#include <math.h>
#include "DxLib.h"
#include "Common.h"
#include "Math.h"
#include "Graphic.h"

void DebugInformation(Navi navi) {
	SetFontSize(20);
	ChangeFont(L"ＭＳ ゴシック");
	ChangeFontType(-1);
	DrawFormatString(30, 30, COLOR_WHITE, L"力行: %d", navi.p);
	DrawFormatString(30, 55, COLOR_WHITE, L"ブレーキ: %d", navi.b);
}

void DrawCab(int bufferHandle, int backgroundHandle, int spriteHandle, int tabletHandle, Navi navi, int pressure, int current) {
	SetDrawScreen(bufferHandle);
	ClearDrawScreen();
	DrawGraph(0, 0, backgroundHandle, TRUE);
	double radSpeed = DegreeToRadian(210.0 + 2.5 * navi.speed);
	DrawLineAA(795, 818, 795 + 76 * sin(radSpeed), 818 - 76 * cos(radSpeed), GetColor(0, 0, 0), 6.0f);
	double radPressure = DegreeToRadian(206.0 + 0.25 * pressure);
	DrawRectRotaGraph2(472, 862, 134, 150, 32, 134, 16, 93, 1.0, radPressure, spriteHandle, TRUE);
	DrawRectGraph(454, 851, 0, 265, 36, 36, spriteHandle, TRUE);
	double radCurrent = DegreeToRadian(230.0 + 0.26 * current);
	DrawLineAA(245, 868, 245 + 64 * sin(radCurrent), 868 - 64 * cos(radCurrent), GetColor(0, 0, 0), 5.0f);
	DrawRectGraph(227, 851, 0, 265, 36, 36, spriteHandle, TRUE);
	DrawGraph(1123, 685, tabletHandle, FALSE);
	SetFontSize(48);
	ChangeFont(L"Tahoma Bold");
	ChangeFontType(DX_FONTTYPE_ANTIALIASING);
	DrawFormatString(1478 - GetDrawFormatStringWidth(L"%d", navi.distance), 822, GetColor(191, 54, 12), L"%d", navi.distance);
	DrawFormatString(1478 - GetDrawFormatStringWidth(L"%d", navi.time), 872, GetColor(191, 54, 12), L"%d", navi.time);
	DrawFormatString(1478 - GetDrawFormatStringWidth(L"%d", navi.score), 922, GetColor(191, 54, 12), L"%d", navi.score);
	SetDrawScreen(DX_SCREEN_BACK);
}
