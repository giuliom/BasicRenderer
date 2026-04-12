#pragma once

#include <cassert>
#include <limits>
#include <stdio.h>
#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	class FrameBuffer
	{
		friend class Renderer;

	protected:
		uint width = 640;
		uint height = 480;
		uint size = 640 * 480;

		std::vector<Color> colorBuffer;
		std::vector<float> depthBuffer;

	public:
		FrameBuffer() = delete;
		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer(uint width_ = 640, uint height_ = 480) : width(width_), height(height_)
		{
			assert(width_ > 0 && height_ > 0);
			colorBuffer.resize(width_ * height_);
			depthBuffer.resize(width_ * height_);
			size = width_ * height_;
		}
		virtual ~FrameBuffer();

		FrameBuffer& operator==(const FrameBuffer& buf) = delete;
		FrameBuffer& operator=(const FrameBuffer& buf) = delete;

		bool WriteToColor(uint w, uint h, const Color& col) noexcept;
		bool WriteToDepth(uint w, uint h, float col) noexcept;
		float GetDepth(uint w, uint h) const noexcept;
		const Color& GetColor(uint w, uint h) const noexcept;

		std::vector<Color> GetColorBufferCopy() const;
		const Color* GetColorBuffer() const noexcept;


		inline uint GetWidth() const noexcept { return width; }
		inline uint GetHeight() const noexcept { return height; }
		inline uint NumPixels() const noexcept { return size;}

		void CorrectGamma() noexcept;

		inline bool WriteToColor(uint index, const Color& col) noexcept
		{
			assert(index >= 0 && index < size);
			colorBuffer[index] = col;
			return true;
		}
		inline bool WriteToDepth(uint index, float col) noexcept
		{
			assert(index >= 0 && index < size);
			depthBuffer[index] = col;
			return true;
		}
		inline float GetDepth(uint index) const noexcept
		{
			assert(index >= 0 && index < size);
			return depthBuffer[index];
		}
		inline const Color& GetColor(uint index) const noexcept
		{
			assert(index >= 0 && index < size);
			return colorBuffer[index];
		}

		inline void Fill(const Color& col) noexcept
		{
			constexpr float d = std::numeric_limits<float>::max();
			for (uint i = 0; i < size; ++i)
			{
				colorBuffer[i] = col;
				depthBuffer[i] = d;
			}
		}
	};
}

