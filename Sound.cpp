#include "DxLib.h"
#include "Common.h"

void ControlVVVFSound(int soundHandle, Navi navi, int current) {
	if (CheckSoundMem(soundHandle) == 1) {
		if (navi.p >= 1) {
			if (navi.speed <= 20) {
				SetFrequencySoundMem(22050, soundHandle);
			} else {
				SetFrequencySoundMem(8000 + (350 * (navi.speed - 20)), soundHandle);
			}
		}
		if (navi.b >= 1) {
			if (navi.speed <= 20) {
				SetFrequencySoundMem(22050, soundHandle);
			} else {
				SetFrequencySoundMem(8000 + (350 * (navi.speed - 20)), soundHandle);
			}
		}
	}
	if (current <= 10) {
		ChangeVolumeSoundMem(0, soundHandle);
	} else {
		ChangeVolumeSoundMem(255, soundHandle);
	}
}

void ControlBrakeSound(int soundHandle, Navi navi) {
	if (navi.b >= 1 && navi.speed > 0 && navi.speed <= 4) {
		if (navi.speed > 3) {
			ChangeVolumeSoundMem(255 * (4 - navi.speed), soundHandle);
		} else if (navi.speed <= 1) {
			ChangeVolumeSoundMem(255 * navi.speed, soundHandle);
		} else {
			ChangeVolumeSoundMem(255, soundHandle);
		}
	} else {
		ChangeVolumeSoundMem(0, soundHandle);
	}
}

void ControlTunnelSound(int soundHandle, Navi navi) {
	SetFrequencySoundMem(5000 + (200 * navi.speed), soundHandle);
	if (navi.speed <= 5) {
		ChangeVolumeSoundMem(0, soundHandle);
	} else {
		if (navi.distance <= 120) {
			if (navi.speed <= 25) {
				ChangeVolumeSoundMem((180 / 20) * navi.speed, soundHandle);
			} else {
				ChangeVolumeSoundMem(180, soundHandle);
			}
		} else {
			if (navi.speed <= 25) {
				ChangeVolumeSoundMem((255 / 20) * navi.speed, soundHandle);
			} else {
				ChangeVolumeSoundMem(255, soundHandle);
			}
		}
	}
}
