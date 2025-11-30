#ifndef _graphic_h
#define _graphic_h

void SetFont(const wchar_t* name, int size, int thick);
void DrawNaviInteger(int y, int d, int spriteHandle);
void DrawCab(int bufferHandle, int backgroundHandle, int spriteHandle, int tabletHandle, int stationHandle, Navi navi, Train train, int pressure, int ampere);

#endif
