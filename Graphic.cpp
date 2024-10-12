#include "DxLib.h"
#include "Common.h"

void DebugInformation(Navi navi) {
	ChangeFontFromHandle(FONT_DEBUG);
	DrawFormatString(30, 30, COLOR_WHITE, L"時間: %d 秒", navi.time);
	DrawFormatString(30, 55, COLOR_WHITE, L"目標時間: %d 秒", navi.arrtime);
	DrawFormatString(30, 80, COLOR_WHITE, L"速度: %.1f km/h (ATC %d)", navi.speed, navi.atc);
	DrawFormatString(30, 105, COLOR_WHITE, L"停止位置まであと %d m", navi.distance);
	DrawFormatString(30, 130, COLOR_WHITE, L"力行: %d", navi.p);
	DrawFormatString(30, 155, COLOR_WHITE, L"ブレーキ: %d", navi.b);
}
