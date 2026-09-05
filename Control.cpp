#include <cmath>
#include "DxLib.h"
#include "Common.h"
#include "Control.h"

Navi UpdateNotch(int key[256], int joypad[8], Navi navi, Train train, int soundHandleNotch1, int soundHandleNotch2, int soundHandleBrake) {
	if (key[KEY_INPUT_LEFT] == 1 || joypad[PAD_LEFT] == 1) {
		if (navi.cb > 0) {
			navi.cb--;
			PlaySoundMem(soundHandleNotch2, DX_PLAYTYPE_BACK);
			if (navi.b == 0 || navi.speed <= 5) {
				PlaySoundMem(soundHandleBrake, DX_PLAYTYPE_BACK);
			}
		}
	}
	if (key[KEY_INPUT_RIGHT] == 1 || joypad[PAD_RIGHT] == 1) {
		if (navi.cb < train.b + 1) {
			navi.cb++;
			PlaySoundMem(soundHandleNotch2, DX_PLAYTYPE_BACK);
		}
	}
	if (key[KEY_INPUT_UP] == 1 || joypad[PAD_UP] == 1) {
		if (navi.cp > 0) {
			navi.cp--;
			PlaySoundMem(soundHandleNotch1, DX_PLAYTYPE_BACK);
		}
	}
	if (key[KEY_INPUT_DOWN] == 1 || joypad[PAD_DOWN] == 1) {
		if (navi.cp < train.p) {
			navi.cp++;
			PlaySoundMem(soundHandleNotch1, DX_PLAYTYPE_BACK);
		}
	}
	int diff = ceil(navi.speed) - navi.atc;
	if (diff >= 1 && navi.autobrake == false) {
		navi.autobrake = true;
		navi.score -= 5;
	} else if (diff <= -3 && navi.autobrake == true) {
		if (navi.b > navi.cb) {
			PlaySoundMem(soundHandleBrake, DX_PLAYTYPE_BACK);
		}
		navi.autobrake = false;
	}
	if (navi.autobrake == true) {
		if (diff >= 10) {
			navi.b = 7;
			navi.p = 0;
		} else if (diff < 10 && diff >= 3) {
			navi.b = 5;
			navi.p = 0;
		} else if (diff < 3) {
			navi.b = 3;
			navi.p = 0;
		}
		if (navi.cb > navi.b) {
			navi.b = navi.cb;
		}
	} else {
		navi.b = navi.cb;
		navi.p = navi.cp;
	}
	return navi;
}

Navi UpdateSpeed(Navi navi, Train train, Fps fps, float gradient) {
	if (navi.b >= 1) {
		if (navi.speed >= 0.05) {
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
	if (navi.speed >= 0.1 && navi.p == 0 && navi.b == 0) {
		navi.speed -= 0.005;
	}
	if (navi.speed >= 0.05) {
		navi.speed -= gradient / 4;
	}
	if (navi.speed >= 80) {
		navi.speed = 80;
	}
	return navi;
}

BrakePressure UpdateBrakePressure(BrakePressure brakePressure, Navi navi, Train train, int soundHandleBrake) {
	if (navi.b == 0) {
		brakePressure.in = 0;
		brakePressure.count = 0;
	} else if (navi.b >= 1 && navi.b <= train.b) {
		if (navi.speed >= 4 && train.bp[navi.b] > 40 && brakePressure.count >= 60) {
			if (brakePressure.count == 60) {
				PlaySoundMem(soundHandleBrake, DX_PLAYTYPE_BACK);
			}
			brakePressure.in = 40;
		} else {
			brakePressure.in = train.bp[navi.b];
		}
		if (brakePressure.count <= 60) {
			brakePressure.count++;
		}
	} else if (navi.b >= train.b + 1) {
		brakePressure.in = 320;
	}
	double error = brakePressure.in - brakePressure.out;
	brakePressure.vel += error * 0.008;
	brakePressure.vel *= 0.78;
	brakePressure.out += brakePressure.vel;
	return brakePressure;
}

BrakePressure UpdateCurrent(BrakePressure current, Navi navi, Train train) {
	if (navi.b >= 1 && navi.b <= train.b) {
		if (navi.speed >= 4 && current.count >= 60) {
			current.in = (1.0 / train.b) * navi.b * (500.0 * (navi.speed / 80));
			if (current.in < 40) {
				current.in = 40;
			}
		} else {
			current.in = 0;
		}
		if (current.count < 60) {
			current.count++;
		}
	} else if (navi.p >= 1) {
		current.in = (1.0 / train.p) * navi.p * 600.0;
	} else {
		current.in = 0;
		current.count = 0;
	}
	double error = current.in - current.out;
	current.vel += error * 0.012;
	current.vel *= 0.78;
	current.out += current.vel;
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
