#include "FrameBuffer.h"
#include <cmath>
#include <cstring>

namespace BasicRenderer
{
	FrameBuffer::~FrameBuffer()
	{
	}

	bool FrameBuffer::WriteToColor(uint w, uint h, const Color& col)
	{
		assert(w >= 0 && w < width&& h >= 0 && h < height);
		colorBuffer[width * h + w] = col;
		return true;
	}

	bool FrameBuffer::WriteToDepth(uint w, uint h, float col)
	{
		assert(w >= 0 && w < width&& h >= 0 && h < height);
		depthBuffer[width * h + w] = col;
		return true;
	}

	float FrameBuffer::GetDepth(uint w, uint h) const
	{
		assert(w >= 0 && w < width&& h >= 0 && h < height);
		return depthBuffer[width * h + w];
	}

	const Color& FrameBuffer::GetColor(uint w, uint h) const
	{
		assert(w >= 0 && w < width&& h >= 0 && h < height);
		return colorBuffer[width * h + w];
	}

	const Color* FrameBuffer::GetColorBuffer() const
	{
		return colorBuffer.data();
	}

	void FrameBuffer::CorrectGamma()
	{
		for (uint i = 0u; i < size; ++i)
		{
			// gamma correction to 2.f instead of 2.2 to avoid the extremely expensive std::powf()
			Color& c = colorBuffer[i];
			c.r = std::sqrt(c.r);
			c.g = std::sqrt(c.g);
			c.b = std::sqrt(c.b);
		}
	}

	std::vector<Color> FrameBuffer::GetColorBufferCopy() const
	{
		return colorBuffer;
	}
}
