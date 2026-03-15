#pragma once

// TODO move to pch
#include <chrono>
#include <random>

typedef unsigned int uint;
typedef std::chrono::high_resolution_clock TimeClock;
typedef TimeClock::time_point TimePoint;
typedef std::chrono::nanoseconds TimeDuration;

namespace BasicRenderer 
{
	constexpr float EPSILON = 0.0000001f;
	constexpr float PI = 3.1415926f;
	constexpr float TWO_PI = 2.f * PI;

	template<typename OutputType, typename DurationMeasure, typename DurationType>
	OutputType ConvertChronoDuration(const DurationType& duration)
	{
		return std::chrono::duration_cast<std::chrono::duration<OutputType, typename DurationMeasure::period>>(duration).count();
	}

	// Thread-local uniform distribution for float [0,1]
	inline float UniformDist_mt19937()
	{
		thread_local std::mt19937 gen{ std::random_device{}() };
		thread_local std::uniform_real_distribution<float> dist01{ 0.0f, 1.0f };
		return dist01(gen);
	}

	// xorshift32 — vastly faster than mt19937 for per-pixel sampling
	inline float UniformDist_xorshift32()
	{
		thread_local uint32_t state = std::random_device{}();
		state ^= state << 13;
		state ^= state >> 17;
		state ^= state << 5;
		return static_cast<float>(state) * 2.3283064365386963e-10f; // / 2^32
	}

	inline constexpr auto UniformDist = &UniformDist_xorshift32;
}
