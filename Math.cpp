#include <cmath>
#include "DxLib.h"
#include "Math.h"

double DegreeToRadian(int degree) {
	return degree * (DX_PI_F / 180.0);
}

int GetDigit(int num) {
	if (num == 0) {
		return 1;
	}
	else {
		return std::log10(num) + 1;
	}
}
