#include "DxLib.h"
#include "Common.h"

Fps::Fps() {
	mStartTime = 0;
	mCount = 0;
	mFps = 0;
}

bool Fps::Update() {
	if (mCount == 0) {
		mStartTime = GetNowCount();
	}
	if (mCount == N) {
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void Fps::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	DrawFormatString(10, 10, GetColor(255, 255, 255), L"%.1f fps", mFps);
}

void Fps::Wait() {
	int tookTime = GetNowCount() - mStartTime;
	int waitTime = mCount * 1000 / FPS - tookTime;
	if (waitTime > 0) {
		Sleep(waitTime);
	}
}

int Fps::Get() {
	return FPS;
}
