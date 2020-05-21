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

		Color* colorBuffer = nullptr;
		float* depthBuffer = nullptr;

	public:
		FrameBuffer() = delete;
		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer(uint width_ = 640, uint height_ = 480) : width(width_), height(height_)
		{
			assert(width_ > 0 && height > 0);
			colorBuffer = new Color[width_ * height_];
			depthBuffer = new float[width_ * height_];
			size = width_ * height_;
		}
		~FrameBuffer();

		FrameBuffer& operator==(const FrameBuffer& buf) = delete;

		bool WriteToColor(uint w, uint h, const Color& col);
		bool WriteToDepth(uint w, uint h, float col);
		float GetDepth(uint w, uint h) const;
		const Color& GetColor(uint w, uint h) const;

		Color* GetColorBufferCopy() const;
		const Color* const GetColorBuffer() const;


		inline uint GetWidth() const { return width; }
		inline uint GetHeight() const { return height; }
		inline uint NumPixels() const { return size;}

		void CorrectGamma();

		inline bool WriteToColor(uint index, const Color& col)
		{
			assert(index >= 0 && index < size);
			colorBuffer[index] = col;
			return true;
		}
		inline bool WriteToDepth(uint index, float col)
		{
			assert(index >= 0 && index < size);
			depthBuffer[index] = col;
			return true;
		}
		inline float GetDepth(uint index) const
		{
			assert(index >= 0 && index < size);
			return depthBuffer[index];
		}
		inline const Color& GetColor(uint index) const
		{
			assert(index >= 0 && index < size);
			return colorBuffer[index];
		}

		inline void Fill(const Color& col)
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

