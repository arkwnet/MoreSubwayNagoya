#ifndef _common_h
#define _common_h

class Fps {
	int mStartTime;
	int mCount;
	float mFps;
	static const int N = 30;
	static const int FPS = 30;
	public:
		Fps();
		bool Update();
		void Draw();
		void Wait();
		int Get();
};

typedef struct {
	int mode;
	int status;
	int count;
	int clock;
} Game;

typedef struct {
	float x;
	float y;
	float z;
	float a;
} Position;

typedef struct {
	int h;
	int m;
	int s;
} Time;

typedef struct {
	int b;
	int p;
	Time time;
	double speed;
	int distance;
	int section;
} Navi;

typedef struct {
	wchar_t name[12];
	Time arr;
	Time dep;
} Station;

typedef struct {
	int max;
	double a;
	double dn;
	double de;
	int b;
	int p;
	int bp[9];
} Train;

typedef struct {
	int in;
	int out;
	int count;
} BrakePressure;

typedef struct {
	bool exist;
	int handle;
} TObject;

static const int PAD_INPUT[7] = {
	PAD_INPUT_DOWN, PAD_INPUT_LEFT, PAD_INPUT_RIGHT, PAD_INPUT_UP,
	PAD_INPUT_1, PAD_INPUT_2, PAD_INPUT_3,
};
static const int PAD_DOWN = 0;
static const int PAD_LEFT = 1;
static const int PAD_RIGHT = 2;
static const int PAD_UP = 3;
static const int PAD_1 = 4;
static const int PAD_2 = 5;
static const int PAD_3 = 6;

#endif
