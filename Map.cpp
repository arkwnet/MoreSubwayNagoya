#include "DxLib.h"
#include "Common.h"

Position GetRailAngle(int i, Position position) {
	// Tokushige: 0m
	if (i >= 0 && i < 100) { position.ax -= 0.001f; }
	if (i >= 80 && i < 130) { position.ay += 0.0007f; }
	if (i >= 580 && i < 630) { position.ay -= 0.0007f; }
	if (i >= 630 && i < 725) { position.ax += 0.0018f; }
	if (i >= 790 && i < 860) { position.ax += 0.0014f; }
	// Kamisawa: 860m (+860m)
	if (i >= 910 && i < 960) { position.ay -= 0.0007f; }
	if (i >= 1110 && i < 1160) { position.ay += 0.0007f; }
	if (i >= 1210 && i < 1670) { position.ax -= 0.0014f; }
	if (i >= 1920 && i < 1990) { position.ax += 0.0014f; }
	// Aioiyama: 2230m (+1370m)
	return position;
}

Navi UpdateATCSpeed(Navi navi, int distance) {
	if (distance == 215) {
		navi.atc = 75;
	}
	if (distance == 625) {
		navi.atc = 55;
	}
	return navi;
}
