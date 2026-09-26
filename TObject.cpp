#include <cmath>
#include "DxLib.h"

VECTOR GetPosition(float x, float y, float z, float ax, float ay, float d) {
	return VGet(x + cosf(ay) * d, y + sinf(ax) * sinf(ay) * d, z - sinf(ay) * cosf(ax) * d);
}

void DeleteObject(int& handle) {
	if (handle != -1) {
		MV1DeleteModel(handle);
		handle = -1;
	}
}

void DrawObject(int handle) {
	if (handle != -1) {
		MV1DrawModel(handle);
	}
}

void Draw3DRail(int mRailHandle[4][200], int mTunnelHandle[2][200], int mPlatformHandle[2][200]) {
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 2; j++) {
			DrawObject(mTunnelHandle[j][i]);
			DrawObject(mRailHandle[j][i]);
			DrawObject(mPlatformHandle[j][i]);
		}
	}
}
