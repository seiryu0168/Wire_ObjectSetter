#include "EngineTime.h"
#include<Windows.h>
#include<chrono>
#pragma comment(lib, "winmm.lib")

//EngineTime::EngineTime()
//	:time(0),deltaTime(0),timeScale(1)
//{
//
//}
//EngineTime::~EngineTime()
//{
//
//}
//namespace EngineTime
//{
//	unsigned long time;
//	float deltaTime;
//	float timeScale;
//};

namespace EngineTime
{
	double time;
	double fixedTime;
	float  deltaTime;
	float  fixedDeltaTime;
	float  timeScale;
	float maxmumDeltaTime;
};

void EngineTime::Initialize()
{
	time = 0;
	fixedDeltaTime = 0;
	deltaTime = 0;
	fixedDeltaTime = 0;
	timeScale = 1;
	maxmumDeltaTime = 0.0167f;
}

unsigned long EngineTime::GetTime()
{
	return time;
}
float EngineTime::GetDeltaTime()
{
	return deltaTime;
}
float EngineTime::GetTimeScale()
{
	return timeScale;
}
void EngineTime::SetTimeScale(float scale)
{
	timeScale = scale;
}
void EngineTime::SetTime()
{

	unsigned long nowTime = timeGetTime();
	static unsigned long prevTime = nowTime;
	deltaTime = (float)(nowTime - prevTime)/1000.0f;
	time += deltaTime;
	prevTime = nowTime;
}

void EngineTime::Update()
{
	
}