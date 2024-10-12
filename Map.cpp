float GetRailAngle(int i, float a, int section) {
	switch (section) {
	case 2620:
		if (i >= 0 && i < 127) { a -= 0.0005f; }
		if (i >= 739 && i < 783) { a += 0.001f; }
		if (i >= 845 && i < 895) { a += 0.001f; }
		break;
	}
	return a;
}
