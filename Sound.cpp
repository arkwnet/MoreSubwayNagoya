#include "DxLib.h"
#include "Common.h"

void ControlVVVFSound(Navi navi, int soundHandle) {
	if (CheckSoundMem(soundHandle) == 1) {
		if (navi.p >= 1) {
			if (navi.speed <= 20) {
				SetFrequencySoundMem(22050, soundHandle);
			} else {
				SetFrequencySoundMem(8000 + (400 * (navi.speed - 20)), soundHandle);
			}
		}
		if (navi.b >= 1) {
			if (navi.speed <= 20) {
				SetFrequencySoundMem(22050, soundHandle);
			} else {
				SetFrequencySoundMem(8000 + (400 * (navi.speed - 20)), soundHandle);
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
