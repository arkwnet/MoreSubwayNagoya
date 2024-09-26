#include "DxLib.h"
#include "Common.h"

void ControlVVVFSound(Navi navi, int soundHandle) {
	if (CheckSoundMem(soundHandle) == 1) {
		if (navi.p >= 1) {
			if (navi.speed <= 3) {
				SetFrequencySoundMem(37000, soundHandle);
			} else if (navi.speed > 3 && navi.speed <= 8) {
				SetFrequencySoundMem(37000 + (8000 * (navi.speed - 3)), soundHandle);
			} else if (navi.speed > 8 && navi.speed <= 16) {
				SetFrequencySoundMem(37000 + 40000, soundHandle);
			} else if (navi.speed > 16 && navi.speed <= 25) {
				SetFrequencySoundMem(37000 + 40000 + (2000 * (navi.speed - 16)), soundHandle);
			} else {
				SetFrequencySoundMem(16000 + (562 * (navi.speed - 25)), soundHandle);
			}
		}
		if (navi.b >= 1) {
			if (navi.speed <= 8) {
				SetFrequencySoundMem(37000, soundHandle);
			} else if (navi.speed > 8 && navi.speed <= 25) {
				SetFrequencySoundMem(37000 + (2000 * (navi.speed - 8)), soundHandle);
			} else {
				SetFrequencySoundMem(16000 + (562 * (navi.speed - 25)), soundHandle);
			}
		}
	}
	if (navi.p == 0 && navi.b == 0) {
		ChangeVolumeSoundMem(0, soundHandle);
	} else {
		if (navi.speed == 0) {
			ChangeVolumeSoundMem(0, soundHandle);
		} else if (navi.speed <= 1) {
			ChangeVolumeSoundMem(255 * (navi.speed * navi.speed), soundHandle);
		} else {
			ChangeVolumeSoundMem(255, soundHandle);
		}
	}
}
