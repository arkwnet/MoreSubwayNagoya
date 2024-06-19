#include "DxLib.h"
#include "Common.h"

void GetKey(int key[256]) {
	for (int i = 0; i < 256; i++) {
		if (CheckHitKey(i) == 1 && key[i] <= 1800) {
			key[i]++;
		} else {
			key[i] = 0;
		}
	}
}

void GetJoypad(int joypad[8], bool isJoypad) {
	if (isJoypad == true) {
		int state = GetJoypadInputState(DX_INPUT_PAD1);
		for (int i = 0; i < 8; i++) {
			if ((state & PAD_INPUT[i]) != 0 && joypad[i] <= 1800) {
				joypad[i]++;
			} else {
				joypad[i] = 0;
			}
		}
	} else {
		for (int i = 0; i < 8; i++) {
			joypad[i] = 0;
		}
	}
}
