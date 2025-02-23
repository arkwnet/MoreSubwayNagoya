#include "DxLib.h"
#include "Common.h"
#include "Control.h"

Navi UpdateNotch(int key[256], int joypad[8], Navi navi, Train train, int soundHandleNotch, int soundHandleBrake) {
	if (key[KEY_INPUT_LEFT] == 1 || joypad[PAD_LEFT] == 1) {
		if (navi.b > 0) {
			navi.b--;
			PlaySoundMem(soundHandleNotch, DX_PLAYTYPE_BACK);
			if (navi.b <= 2 || navi.speed <= 6) {
				PlaySoundMem(soundHandleBrake, DX_PLAYTYPE_BACK);
			}
		}
	}
	if (key[KEY_INPUT_RIGHT] == 1 || joypad[PAD_RIGHT] == 1) {
		if (navi.b < train.b + 1) {
			navi.b++;
			PlaySoundMem(soundHandleNotch, DX_PLAYTYPE_BACK);
		}
	}
	if (key[KEY_INPUT_UP] == 1 || joypad[PAD_UP] == 1) {
		if (navi.p > 0) {
			navi.p--;
			PlaySoundMem(soundHandleNotch, DX_PLAYTYPE_BACK);
		}
	}
	if (key[KEY_INPUT_DOWN] == 1 || joypad[PAD_DOWN] == 1) {
		if (navi.p < train.p) {
			navi.p++;
			PlaySoundMem(soundHandleNotch, DX_PLAYTYPE_BACK);
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
		navi.speed -= 0.005;
	}
	if (navi.speed >= 80) {
		navi.speed = 80;
	}
	return navi;
}

BrakePressure UpdateBrakePressure(BrakePressure brakePressure, Navi navi, Train train) {
	if (navi.b == 0) {
		brakePressure.in = 0;
		brakePressure.count = 0;
	} else if (navi.b >= 1 && navi.b <= train.b) {
		if (navi.speed >= 6 && train.bp[navi.b] > 80 && brakePressure.count >= 120) {
			brakePressure.in = 80;
		} else {
			brakePressure.in = train.bp[navi.b];
		}
		if (brakePressure.count < 120) {
			brakePressure.count++;
		}
	} else if (navi.b >= train.b + 1) {
		brakePressure.in = 360;
	}
	if (brakePressure.in > brakePressure.out) {
		int diff = abs(brakePressure.in - brakePressure.out) / 20;
		if (diff > 1) {
			brakePressure.out += diff;
		} else {
			brakePressure.out += 1;
		}
	} else if (brakePressure.in < brakePressure.out) {
		int diff = abs(brakePressure.in - brakePressure.out) / 20;
		if (diff > 1) {
			brakePressure.out -= diff;
		} else {
			brakePressure.out -= 1;
		}
	} else {
		brakePressure.out = brakePressure.in;
	}
	return brakePressure;
}

BrakePressure UpdateCurrent(BrakePressure current, Navi navi, Train train) {
	if (navi.b >= 1 && navi.b <= train.b) {
		if (navi.speed >= 6 && current.count >= 120) {
			current.in = (1.0 / train.b) * navi.b * 500.0;
		} else {
			current.in = 0;
		}
		if (current.count < 120) {
			current.count++;
		}
	} else if (navi.p >= 1) {
		current.in = (1.0 / train.p) * navi.p * 600.0;
	} else {
		current.in = 0;
		current.count = 0;
	}
	if (current.in > current.out) {
		if (abs(current.in - current.out) > 5) {
			current.out += abs(current.in - current.out) / 20;
		} else {
			current.out += 1;
		}
	} else if (current.in < current.out) {
		if (abs(current.in - current.out) > 5) {
			current.out -= abs(current.in - current.out) / 20;
		} else {
			current.out -= 1;
		}
	} else {
		current.out = current.in;
	}
	return current;
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
