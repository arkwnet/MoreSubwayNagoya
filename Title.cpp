#include "DxLib.h"
#include "Common.h"

void DrawArakawaLogo(Game game, int backgroundHandle, int screenWidth, int screenHeight) {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (game.count >= 60 && game.count < 90) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (game.count - 60) * (255 / 30));
	}
	if (game.count >= 210 && game.count < 240) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - ((game.count - 210) * (255 / 30)));
	}
	if (game.count >= 60 && game.count < 240) {
		DrawGraph(screenWidth / 2 - 301, screenHeight / 2 - 110, backgroundHandle, FALSE);
	}
}
