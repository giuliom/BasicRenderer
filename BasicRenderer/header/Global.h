#pragma once

#include <chrono>

typedef unsigned int uint;

namespace BasicRenderer 
{
	const float EPSILON = 0.0000001f;

	template<typename DurationMeasure, typename DurationType, typename OutputType>
	void ConvertChronoDuration(const DurationType& duration, OutputType& output)
	{
		output = std::chrono::duration_cast<DurationMeasure>(duration).count();
	}
}
