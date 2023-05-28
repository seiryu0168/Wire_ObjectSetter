#include "Easing.h"
#include<iostream>
#include"Engine/SAFE_DELETE_RELEASE.h"

void Clamp(float& number, float min=0.0f, float max=1.0f);
float Easing::EaseLinear(float time)
{
	Clamp(time);
	return time;
}
float Easing::EaseInSine(float time)
{
	Clamp(time);
	return (1.0f - std::cos((time*M_PI)/2.0f));
}
float Easing::EaseOutSine(float time)
{
	Clamp(time);
	return (1.0f - std::sin((time * M_PI) / 2.0f));
}
float Easing::EaseInOutSine(float time)
{
	Clamp(time);
	return (-(std::cosf(M_PI * time) - 1.0f) / 2.0f);

}


float Easing::EaseInQuad(float time)
{
	Clamp(time);
	return (time * time);
}
float Easing::EaseOutQuad(float time)
{
	Clamp(time);
	return (1.0f - (1.0f - time) * (1.0f - time));
}

float Easing::EaseINOutQuad(float time)
{
	Clamp(time);
	return (time < 0.5f ? 2 * time * time : 1 - std::pow((-2 * time + 2), 2) / 2);
}

float Easing::EaseInCubic(float time)
{
	Clamp(time);

	return powf(time,3);
}

float Easing::EaseOutCubic(float time)
{
	Clamp(time);
	return 1 - powf(1 - time, 3);
}

float Easing::EaseInOutCubic(float time)
{
	Clamp(time);
	return time < 0.5f ? 4.0f * (powf(time, 3.0f)) : 1.0f - powf((-2.0f * time + 2.0f), 3) / 2.0f;
}

float Easing::EaseInQuart(float time)
{
	Clamp(time);
	return powf(time, 4);
}

float Easing::EaseOutQuart(float time)
{
	Clamp(time);
	return 1.0f-powf(1.0f-time, 4);
}

float Easing::EaseInOutQuart(float time)
{
	Clamp(time);
	return time < 0.5f ? 8.0f * powf(time, 4) : 1.0f - powf(-2.0f * time + 2.0f, 4)/2.0f;
}

float Easing::EaseInQuint(float time)
{
	Clamp(time);
	return powf(time, 5);
}

float Easing::EaseOutQuint(float time)
{
	Clamp(time);
	return 1.0f-powf(1.0f-time, 5);
}

float Easing::EaseInOutQuint(float time)
{
	Clamp(time);
	return time < 0.5f ? 16.0f * powf(time, 5) : 1.0f - powf((-2.0f * time + 2.0f), 5) / 2.0f;	
}

float Easing::EaseInExpo(float time)
{
	Clamp(time);
	return time == 0 ? 0 : powf(2, 10 * time - 10);
}

float Easing::EaseOutExpo(float time)
{
	Clamp(time);
	return time == 1.0f ? 1.0f : 1.0f - powf(2.0f, -10.0f * time);
}

float Easing::EaseInOutExpo(float time)
{
	Clamp(time);
	if (time == 0.0f)
	{
		return 0;
	}
	else
	{
		if (time == 1.0f)
			return 1.0f;
		else
		{
			return time < 0.5f ? powf(2.0f, (20.0f * time - 10.0f)) / 2.0f :
							     2.0f - powf(2.0f, (-20.0f * time - 10.0f)) / 2.0f;
		}
	}
	return 0;
}

float Easing::EaseOutIn(float time, float tan)
{
	Clamp(time);
	return (powf((time - tan / tan), 3) * tan) + tan;
}

void Clamp(float &number,float min,float max)
{
	if (number > max)
	{
		 number = max;
	}

	if (number < min)
	{
		number = min;
	}
}