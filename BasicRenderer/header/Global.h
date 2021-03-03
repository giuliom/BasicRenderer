#pragma once

// TODO move to pch
#include <chrono>

typedef unsigned int uint;
typedef std::chrono::high_resolution_clock TimeClock;
typedef TimeClock::time_point TimePoint;
typedef std::chrono::nanoseconds TimeDuration;

namespace BasicRenderer 
{
	const float EPSILON = 0.0000001f;

	template<typename OutputType, typename DurationMeasure, typename DurationType>
	OutputType ConvertChronoDuration(const DurationType& duration)
	{
		return std::chrono::duration_cast<std::chrono::duration<OutputType, DurationMeasure::period>>(duration).count();
	}
}
