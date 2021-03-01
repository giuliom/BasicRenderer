#pragma once

#include <chrono>

typedef unsigned int uint;
typedef std::chrono::high_resolution_clock TimeClock;
typedef TimeClock::time_point TimePoint;
typedef std::chrono::nanoseconds TimeDuration;

namespace BasicRenderer 
{
	const float EPSILON = 0.0000001f;

	template<typename DurationMeasure, typename DurationType, typename OutputType>
	void ConvertChronoDuration(const DurationType& duration, OutputType& output)
	{
		output = std::chrono::duration_cast<DurationMeasure>(duration).count();
	}
}
