#include "DxLib.h"
#include "Common.h"

float GetRailAngle(int i, float a) {
	// Tokushige: 0m
	if (i >= 0 && i < 100) { a -= 0.001f; }
	if (i >= 630 && i < 725) { a += 0.0018f; }
	if (i >= 790 && i < 860) { a += 0.0014f; }
	// Kamisawa: 860m (+860m)
	if (i >= 1210 && i < 1670) { a -= 0.0014f; }
	if (i >= 1920 && i < 1990) { a += 0.0014f; }
	// Aioiyama: 2230m (+1370m)
	return a;
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
