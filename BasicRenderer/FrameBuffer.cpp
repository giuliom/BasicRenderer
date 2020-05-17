#include "FrameBuffer.h"

namespace BasicRenderer
{
	FrameBuffer::~FrameBuffer()
	{
		delete[] colorBuffer;
		delete[] depthBuffer;
	}

	bool FrameBuffer::WriteToColor(int w, int h, const Color& col)
	{
		assert(w >= 0 && w < width&& h >= 0 && h < width);
		colorBuffer[width * h + w] = col;
		return true;
	}

	bool FrameBuffer::WriteToDepth(int w, int h, float col)
	{
		assert(w >= 0 && w < width&& h >= 0 && h < width);
		depthBuffer[width * h + w] = col;
		return true;
	}

	float FrameBuffer::GetDepth(int w, int h) const
	{
		assert(w >= 0 && w < width&& h >= 0 && h < width);
		return depthBuffer[width * h + w];
	}

	const Color& FrameBuffer::GetColor(int w, int h) const
	{
		assert(w >= 0 && w < width&& h >= 0 && h < width);
		return colorBuffer[width * h + w];
	}

	const Color* const FrameBuffer::GetColorBuffer() const
	{
		return colorBuffer;
	}

	Color* FrameBuffer::GetColorBufferCopy() const
	{
		Color* copy = new Color[width * height];
		memcpy(copy, colorBuffer, sizeof(Color) * width * height);
		return copy;
	}
}
