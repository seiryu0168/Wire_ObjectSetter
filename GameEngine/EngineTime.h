#pragma once

namespace EngineTime
{
//public:
//	EngineTime();
//	~EngineTime();
	void Initialize();
	unsigned long GetTime();
	float GetDeltaTime();
	float GetTimeScale();
	void SetTimeScale(float scale);
	void SetTime();
	void Update();
};
