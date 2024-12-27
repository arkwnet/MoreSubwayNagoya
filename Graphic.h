#ifndef _graphic_h
#define _graphic_h

void SetFont(const wchar_t* name, int size, int thick);
void DrawCab(int bufferHandle, int backgroundHandle, int spriteHandle, int tabletHandle, Navi navi, Train train, int pressure, int ampere);

#endif
