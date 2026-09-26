#ifndef _tobject_h
#define _tobject_h

VECTOR GetPositionX(float x, float y, float z, float a, float d);
void DeleteObject(int& handle);
void DrawObject(int handle);
void Draw3DRail(int mRailHandle[4][200], int mTunnelHandle[2][200], int mPlatformHandle[2][200]);

#endif
