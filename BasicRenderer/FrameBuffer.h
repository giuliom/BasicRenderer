#pragma once

#include <cassert>
#include <limits>
#include <stdio.h>
#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	class FrameBuffer
	{
	protected:
		int width = 640;
		int height = 480;
		int size = 640 * 480;

		Color* colorBuffer = nullptr;
		float* depthBuffer = nullptr;


	public:
		FrameBuffer() = delete;
		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer(int width_ = 640, int height_ = 480) : width(width_), height(height_)
		{
			assert(width_ > 0 && height > 0);
			colorBuffer = new Color[width_ * height_];
			depthBuffer = new float[width_ * height_];
			size = width_ * height_;
		}
		~FrameBuffer();

		FrameBuffer& operator==(const FrameBuffer& buf) = delete;

		bool WriteToColor(int w, int h, const Color& col);
		bool WriteToDepth(int w, int h, float col);
		float GetDepth(int w, int h) const;
		const Color& GetColor(int w, int h) const;

		Color* GetColorBufferCopy() const;
		const Color* const GetColorBuffer() const;


		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }

		inline bool WriteToColor(int index, const Color& col)
		{
			assert(index >= 0 && index < size);
			colorBuffer[index] = col;
			return true;
		}
		inline bool WriteToDepth(int index, float col)
		{
			assert(index >= 0 && index < size);
			depthBuffer[index] = col;
			return true;
		}
		inline float GetDepth(int index) const
		{
			assert(index >= 0 && index < size);
			return depthBuffer[index];
		}
		inline const Color& GetColor(int index) const
		{
			assert(index >= 0 && index < size);
			return colorBuffer[index];
		}

		inline void Fill(const Color& col)
		{
			const float d = std::numeric_limits<float>::max();
			for (int i = 0; i < size; ++i)
			{
				colorBuffer[i] = col;
				depthBuffer[i] = d;
			}
		}
	};
}

