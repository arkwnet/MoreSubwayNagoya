#include "DxLib.h"
#include "Common.h"

int soundHandle[16];

void InitSound() {
	soundHandle[0] = LoadSoundMem(L"Assets\\Sound\\Inverter.wav");
	soundHandle[1] = LoadSoundMem(L"Assets\\Sound\\BrakeDecompress.wav");
	soundHandle[2] = LoadSoundMem(L"Assets\\Sound\\BrakeStop.wav");
	soundHandle[3] = LoadSoundMem(L"Assets\\Sound\\Tunnel.wav");
	soundHandle[4] = LoadSoundMem(L"Assets\\Sound\\Notch1.wav");
	soundHandle[5] = LoadSoundMem(L"Assets\\Sound\\Notch2.wav");
	soundHandle[6] = LoadSoundMem(L"Assets\\Sound\\ATCBell.wav");
	soundHandle[9] = LoadSoundMem(L"Assets\\Sound\\Buzzer.wav");
	soundHandle[10] = LoadSoundMem(L"Assets\\Sound\\DoorOpen.wav");
	soundHandle[11] = LoadSoundMem(L"Assets\\Sound\\DoorClose.wav");
	soundHandle[12] = LoadSoundMem(L"Assets\\Sound\\Announcement\\End.wav");
	soundHandle[13] = LoadSoundMem(L"Assets\\Sound\\Announcement\\62210.wav");
}

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
