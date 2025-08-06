#include "DxLib.h"
#include "Common.h"

float GetRailAngle(int i, float a) {
	if (i >= 0 && i < 100) { a -= 0.001f; }
	if (i >= 630 && i < 725) { a += 0.0018f; }
	if (i >= 790 && i < 860) { a += 0.0014f; }
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
