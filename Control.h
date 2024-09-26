#ifndef _control_h
#define _control_h

Navi UpdateNotch(int key[256], int joypad[8], Navi navi, Train train);
Navi UpdateSpeed(Navi navi, Train train, Fps fps);
void GetKey(int key[256]);
void GetJoypad(int joypad[8], bool isJoypad);

#endif
