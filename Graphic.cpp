#include <cmath>
#include <math.h>
#include "DxLib.h"
#include "Common.h"
#include "Math.h"
#include "Graphic.h"

void SetFont(const wchar_t* name, int size, int thick) {
	ChangeFont(name);
	SetFontSize(size);
	SetFontThickness(thick);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_8X8);
	return;
}

void DrawCab(int bufferHandle, int backgroundHandle, int spriteHandle, int tabletHandle, int stationHandle, Navi navi, Train train, int pressure, int current) {
	SetDrawScreen(bufferHandle);
	ClearDrawScreen();
	DrawGraph(0, 0, backgroundHandle, TRUE);
	double radSpeed = DegreeToRadian(210.0 + 2.5 * navi.speed);
	DrawLineAA(795, 818, 795 + 76 * sin(radSpeed), 818 - 76 * cos(radSpeed), GetColor(0, 0, 0), 6.0f);
	DrawRectGraph(730, 762, 0, 146, 130, 115, spriteHandle, TRUE);
	double radPressure = DegreeToRadian(206.0 + 0.25 * pressure);
	DrawRectRotaGraph2(472, 862, 134, 150, 32, 134, 16, 93, 1.0, radPressure, spriteHandle, TRUE);
	DrawRectGraph(454, 851, 0, 265, 36, 36, spriteHandle, TRUE);
	double radCurrent = DegreeToRadian(230.0 + 0.26 * current);
	DrawLineAA(245, 868, 245 + 64 * sin(radCurrent), 868 - 64 * cos(radCurrent), GetColor(0, 0, 0), 5.0f);
	DrawRectGraph(227, 851, 0, 265, 36, 36, spriteHandle, TRUE);
	if (navi.time >= -2) {
		DrawRectGraph(851, 982, 0, 300, 34, 34, spriteHandle, TRUE);
	}
	int tx = 1123;
	int ty = 685;
	DrawGraph(tx, ty, tabletHandle, FALSE);
	if (navi.section == 2620) {
		DrawRectGraph(tx + 59, ty + 64, 0, 100, 150, 50, stationHandle, TRUE);
		DrawRectGraph(tx + 379, ty + 64, 0, 50, 150, 50, stationHandle, TRUE);
	} else if (navi.section == 2619) {
		DrawRectGraph(tx + 59, ty + 64, 0, 50, 150, 50, stationHandle, TRUE);
		DrawRectGraph(tx + 379, ty + 64, 0, 0, 150, 50, stationHandle, TRUE);
	}
	ChangeFontType(DX_FONTTYPE_ANTIALIASING);
	ChangeFont(L"Tahoma Bold");
	SetFontSize(48);
	DrawFormatString(1478 - GetDrawFormatStringWidth(L"%d", navi.distance), 822, GetColor(191, 54, 12), L"%d", navi.distance);
	DrawFormatString(1478 - GetDrawFormatStringWidth(L"%d", navi.arrtime - navi.time), 872, GetColor(191, 54, 12), L"%d", navi.arrtime - navi.time);
	DrawFormatString(1478 - GetDrawFormatStringWidth(L"%d", navi.score), 922, GetColor(191, 54, 12), L"%d", navi.score);
	SetFontSize(44);
	if (navi.p == 0) {
		ChangeFont(L"ＭＳ ゴシック");
		DrawString(tx + 459 - GetDrawStringWidth(L"切", -1) / 2, 920, L"切", GetColor(33, 150, 243));
	} else if (navi.p >= 1 && navi.p <= train.p - 1) {
		DrawBox(tx + 434, ty + 177, tx + 486, 164 / train.p * navi.p + ty + 177, GetColor(139, 195, 74), TRUE);
		ChangeFont(L"Tahoma Bold");
		DrawFormatString(tx + 459 - GetDrawFormatStringWidth(L"%d", navi.p) / 2, 920, GetColor(33, 150, 243), L"%d", navi.p);
	} else if (navi.p == train.p) {
		DrawBox(tx + 434, ty + 177, tx + 486, ty + 341, GetColor(139, 195, 74), TRUE);
		ChangeFont(L"Tahoma Bold");
		DrawFormatString(tx + 459 - GetDrawFormatStringWidth(L"%d", navi.p) / 2, 920, GetColor(33, 150, 243), L"%d", navi.p);
	}
	if (navi.b == 0) {
		ChangeFont(L"ＭＳ ゴシック");
		SetFontSize(26);
		DrawString(tx + 534 - GetDrawStringWidth(L"弛め", -1) / 2, 931, L"弛め", GetColor(33, 150, 243));
	} else if (navi.b >= 1 && navi.b <= train.b - 1) {
		DrawBox(tx + 508, ty + 341 - 164 / train.b * navi.b, tx + 560, ty + 341, GetColor(255, 152, 0), TRUE);
		ChangeFont(L"Tahoma Bold");
		DrawFormatString(tx + 533 - GetDrawFormatStringWidth(L"%d", navi.b) / 2, 920, GetColor(33, 150, 243), L"%d", navi.b);
	} else if (navi.b == train.b) {
		DrawBox(tx + 508, ty + 177, tx + 560, ty + 341, GetColor(255, 152, 0), TRUE);
		ChangeFont(L"Tahoma Bold");
		DrawFormatString(tx + 533 - GetDrawFormatStringWidth(L"%d", navi.b) / 2, 920, GetColor(33, 150, 243), L"%d", navi.b);
	} else if (navi.b == train.b + 1) {
		DrawBox(tx + 508, ty + 177, tx + 560, ty + 341, GetColor(244, 67, 54), TRUE);
		ChangeFont(L"ＭＳ ゴシック");
		SetFontSize(26);
		DrawString(tx + 534 - GetDrawStringWidth(L"非常", -1) / 2, 931, L"非常", GetColor(33, 150, 243));
	}
	SetDrawScreen(DX_SCREEN_BACK);
}
