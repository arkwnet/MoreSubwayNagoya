#include "DxLib.h"
#include "Common.h"

void DrawArakawaLogo(Game game, int backgroundHandle, int screenWidth, int screenHeight) {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (game.count >= 30 && game.count < 45) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (game.count - 30) * (255 / 15));
	}
	if (game.count >= 105 && game.count < 120) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - ((game.count - 105) * (255 / 15)));
	}
	if (game.count >= 30 && game.count < 120) {
		DrawGraph(screenWidth / 2 - 301, screenHeight / 2 - 110, backgroundHandle, FALSE);
	}
}
