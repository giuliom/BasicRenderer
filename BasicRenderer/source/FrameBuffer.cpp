#include "FrameBuffer.h"
#include <cmath>
#include <cstring>

namespace BasicRenderer
{
	FrameBuffer::~FrameBuffer()
	{
		delete[] colorBuffer;
		delete[] depthBuffer;
	}

	bool FrameBuffer::WriteToColor(uint w, uint h, const Color& col)
	{
		assert(w >= 0 && w < width&& h >= 0 && h < width);
		colorBuffer[width * h + w] = col;
		return true;
	}

	bool FrameBuffer::WriteToDepth(uint w, uint h, float col)
	{
		assert(w >= 0 && w < width&& h >= 0 && h < width);
		depthBuffer[width * h + w] = col;
		return true;
	}

	float FrameBuffer::GetDepth(uint w, uint h) const
	{
		assert(w >= 0 && w < width&& h >= 0 && h < width);
		return depthBuffer[width * h + w];
	}

	const Color& FrameBuffer::GetColor(uint w, uint h) const
	{
		assert(w >= 0 && w < width&& h >= 0 && h < width);
		return colorBuffer[width * h + w];
	}

	const Color* const FrameBuffer::GetColorBuffer() const
	{
		return colorBuffer;
	}

	void FrameBuffer::CorrectGamma()
	{
		for (uint i = 0u; i < size; ++i)
		{
			// gamma correction to 2.f instead of 2.2 to avoid the extremely expensive std::powf()
			Color& c = colorBuffer[i];
			c.r = sqrtf(c.r);
			c.g = sqrtf(c.g);
			c.b = sqrtf(c.b);
		}
	}

	Color* FrameBuffer::GetColorBufferCopy() const
	{
		Color* copy = new Color[width * height];
		memcpy(copy, colorBuffer, sizeof(Color) * width * height);
		return copy;
	}
}
