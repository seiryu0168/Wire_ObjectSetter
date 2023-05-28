#pragma once

namespace Easing
{
	float EaseLinear(float time);
	
	float EaseInSine(float time);
	float EaseOutSine(float time);
	float EaseInOutSine(float time);

	float EaseInQuad(float time);
	float EaseOutQuad(float time);
	float EaseINOutQuad(float time);

	float EaseInCubic(float time);
	float EaseOutCubic(float time);
	float EaseInOutCubic(float time);

	float EaseInQuart(float time);
	float EaseOutQuart(float time);
	float EaseInOutQuart(float time);

	float EaseInQuint(float time);
	float EaseOutQuint(float time);
	float EaseInOutQuint(float time);

	float EaseInExpo(float time);
	float EaseOutExpo(float time);
	float EaseInOutExpo(float time);

	float EaseInCirc(float time);
	float EaseOutCirc(float time);
	float EaseInOutCirc(float time);



	float EaseOutIn(float time,float tan);
};

