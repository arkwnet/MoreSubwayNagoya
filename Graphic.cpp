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

void DrawNaviInteger(int y, int d, int spriteHandle) {
	int x = 1448;
	int a = abs(d);
	int s;
	for (int i = 0; i < 4; i++) {
		int p = static_cast<int>(pow(10, i));
		if (i >= 1 && p > a) {
			break;
		}
		if (i == 0) {
			s = a % 10;
		} else {
			s = a / p % 10;
		}
		DrawRectGraph(x, y, 32 * s, 0, 32, 42, spriteHandle, TRUE);
		x -= 32;
	}
	if (d < 0) {
		DrawRectGraph(x, y, 320, 0, 32, 42, spriteHandle, TRUE);
	}
	return;
}

void DrawCab(int bufferHandle, int backgroundHandle, int spriteHandle, int tabletHandle, int stationHandle, Navi navi, Train train, int pressure, int current) {
	SetDrawScreen(bufferHandle);
	ClearDrawScreen();
	DrawGraph(0, 0, backgroundHandle, TRUE);
	double radSpeed = DegreeToRadian(210.0 + 2.5 * navi.speed);
	DrawLineAA(795, 818, 795 + 76 * sin(radSpeed), 818 - 76 * cos(radSpeed), GetColor(0, 0, 0), 6.0f);
	DrawRectGraph(730, 762, 0, 146, 130, 115, spriteHandle, TRUE);
	double radPressure = DegreeToRadian(210.0 + 0.26 * pressure);
	DrawRectRotaGraph2(472, 862, 134, 150, 32, 134, 16, 93, 1.0, radPressure, spriteHandle, TRUE);
	DrawRectGraph(454, 851, 0, 265, 36, 36, spriteHandle, TRUE);
	double radCurrent = DegreeToRadian(232.0 + 0.26 * current);
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
	DrawNaviInteger(826, navi.distance, spriteHandle);
	DrawNaviInteger(876, navi.arrtime - navi.time, spriteHandle);
	DrawNaviInteger(926, navi.score, spriteHandle);
	if (navi.cp >= 1 && navi.cp <= train.p - 1) {
		DrawBox(tx + 434, ty + 177, tx + 486, 164 / train.p * navi.cp + ty + 177, GetColor(139, 195, 74), TRUE);
	} else if (navi.cp == train.p) {
		DrawBox(tx + 434, ty + 177, tx + 486, ty + 341, GetColor(139, 195, 74), TRUE);
	}
	if (navi.cp == 0) {
		DrawRectGraph(tx + 434, 920, 384, 0, 52, 40, spriteHandle, TRUE);
	} else if (navi.cp >= 1 && navi.cp <= train.p - 1) {
		DrawRectGraph(tx + 434, 920, 384, 42 * (navi.cp + 1), 52, 40, spriteHandle, TRUE);
	} else if (navi.cp == train.p) {
		DrawRectGraph(tx + 434, 920, 384, 42 * (navi.cp + 1), 52, 40, spriteHandle, TRUE);
	}
	if (navi.cb >= 1 && navi.cb <= train.b - 1) {
		DrawBox(tx + 508, ty + 341 - 164 / train.b * navi.cb, tx + 560, ty + 341, GetColor(255, 152, 0), TRUE);
	} else if (navi.cb == train.b) {
		DrawBox(tx + 508, ty + 177, tx + 560, ty + 341, GetColor(255, 152, 0), TRUE);
	} else if (navi.cb == train.b + 1) {
		DrawBox(tx + 508, ty + 177, tx + 560, ty + 341, GetColor(244, 67, 54), TRUE);
	}
	if (navi.cb == 0) {
		DrawRectGraph(tx + 508, 920, 384, 42, 52, 40, spriteHandle, TRUE);
	} else {
		DrawRectGraph(tx + 508, 920, 384, 42 * (navi.cb + 1), 52, 40, spriteHandle, TRUE);
	}
	if (navi.atc == 0) {
		DrawRectGraph(718, 897, 0, 343, 37, 37, spriteHandle, TRUE);
	} else if (navi.atc == 55) {
		DrawRectGraph(756, 683, 111, 343, 37, 37, spriteHandle, TRUE);
	} else if (navi.atc == 75) {
		DrawRectGraph(845, 703, 148, 343, 37, 37, spriteHandle, TRUE);
	}
	if (navi.b >= 1) {
		DrawRectGraph(269, 667, 0, 380, 60, 53, spriteHandle, TRUE);
	}
	if (navi.autobrake == true) {
		DrawRectGraph(327, 667, 60, 380, 60, 53, spriteHandle, TRUE);
	}
	if (navi.b == train.b + 1) {
		DrawRectGraph(384, 667, 120, 380, 60, 53, spriteHandle, TRUE);
	}
	SetDrawScreen(DX_SCREEN_BACK);
}
