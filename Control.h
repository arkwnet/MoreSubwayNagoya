#ifndef _control_h
#define _control_h

Navi UpdateNotch(int key[256], int joypad[8], Navi navi, Train train, int soundHandleNotch1, int soundHandleNotch2, int soundHandleBrake);
Navi UpdateSpeed(Navi navi, Train train, Fps fps);
BrakePressure UpdateBrakePressure(BrakePressure brakePressure, Navi navi, Train train);
BrakePressure UpdateCurrent(BrakePressure current, Navi navi, Train train);
void GetKey(int key[256]);
void GetJoypad(int joypad[8], bool isJoypad);

#endif
