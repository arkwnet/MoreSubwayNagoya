#include "DxLib.h"
#include "Common.h"

Navi UpdateNotch(int key[256], int joypad[8], Navi navi, Train train) {
	if (key[KEY_INPUT_LEFT] == 1 || joypad[PAD_LEFT] == 1) {
		if (navi.b > 0) {
			navi.b--;
		}
	}
	if (key[KEY_INPUT_RIGHT] == 1 || joypad[PAD_RIGHT] == 1) {
		if (navi.b < train.b + 1) {
			navi.b++;
		}
	}
	if (key[KEY_INPUT_UP] == 1 || joypad[PAD_UP] == 1) {
		if (navi.p > 0) {
			navi.p--;
		}
	}
	if (key[KEY_INPUT_DOWN] == 1 || joypad[PAD_DOWN] == 1) {
		if (navi.p < train.p) {
			navi.p++;
		}
	}
	return navi;
}

Navi UpdateSpeed(Navi navi, Train train, Fps fps) {
	if (navi.b >= 1) {
		if (navi.speed >= 0.1) {
			if (navi.b >= train.b + 1) {
				navi.speed -= static_cast<double>(train.de) / fps.Get();
			} else {
				navi.speed -= train.dn * (static_cast<double>(train.bp[navi.b]) / train.bp[train.b]) / fps.Get();
			}
		} else {
			navi.speed = 0;
		}
	}
	if (navi.speed < train.max && navi.p >= 1) {
		navi.speed += (train.a * (1.0 / train.p * navi.p) / fps.Get());
	}
	if (navi.speed >= 0.1) {
		navi.speed -= 0.01;
	}
	return navi;
}

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
