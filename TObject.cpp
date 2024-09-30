#include "DxLib.h"

void Draw3DRail(int mRailHandle[4][200], int mTunnelHandle[200], int mPlatformHandle[2][100]) {
	for (int i = 0; i < 200; i++) {
		MV1DrawModel(mTunnelHandle[i]);
		MV1DrawModel(mRailHandle[0][i]);
	}
}
