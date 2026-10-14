#include "DxLib.h"
#include "Common.h"
#include "Interface.h"

void DrawInterfaceHeader(int commonHandle, int spriteHandle) {
	DrawBox(0, 0, BUFFER_WIDTH, 120, COLOR_BLACK, TRUE);
	DrawRectGraph(0, 0, 0, 0, 640, 120, spriteHandle, TRUE);
	DrawRectGraph(BUFFER_WIDTH - 380, 0, 0, 0, 380, 120, commonHandle, TRUE);
}

void DrawInterfaceFooter(int commonHandle, int controller) {
	if (controller == 0) {
		DrawBox(792, 930, BUFFER_WIDTH, 1030, COLOR_BLACK, TRUE);
		DrawCircleAA(792, 980, 50, 32, COLOR_BLACK, TRUE);
		DrawRectGraph(812, 945, 0, 120, 310, 70, commonHandle, TRUE);
		DrawRectGraph(1308, 945, 0, 190, 146, 70, commonHandle, TRUE);
		DrawRectGraph(1640, 945, 0, 260, 116, 70, commonHandle, TRUE);
		DrawRectGraph(1152, 956, 0, 330, 96, 48, commonHandle, TRUE);
		DrawRectGraph(1484, 956, 0, 380, 96, 48, commonHandle, TRUE);
	} else {
		DrawBox(914, 930, BUFFER_WIDTH, 1030, COLOR_BLACK, TRUE);
		DrawCircleAA(914, 980, 50, 32, COLOR_BLACK, TRUE);
		DrawRectGraph(934, 945, 0, 120, 310, 70, commonHandle, TRUE);
		DrawRectGraph(1430, 945, 160, 190, 70, 70, commonHandle, TRUE);
		DrawRectGraph(1686, 945, 160, 260, 70, 70, commonHandle, TRUE);
		DrawRectGraph(1274, 956, 0, 330, 96, 48, commonHandle, TRUE);
		DrawRectGraph(1530, 956, 0, 380, 96, 48, commonHandle, TRUE);
	}
	DrawRectGraph(1786, 956, 0, 430, 96, 48, commonHandle, TRUE);
}
