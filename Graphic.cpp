#include "DxLib.h"
#include "Common.h"

void DebugInformation(Navi navi) {
	ChangeFontFromHandle(FONT_DEBUG);
	DrawFormatString(30, 30, COLOR_WHITE, L"ブレーキ: %d", navi.b);
	DrawFormatString(30, 55, COLOR_WHITE, L"マスコン: %d", navi.p);
	DrawFormatString(30, 80, COLOR_WHITE, L"速度: %.1f km/h", navi.speed);
	DrawFormatString(30, 105, COLOR_WHITE, L"停止位置まであと %d m", navi.distance);
}
