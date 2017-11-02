#include "FrameBuffer.h"
#include <limits>


FrameBuffer::~FrameBuffer()
{
	delete[] colorBuffer;
	delete[] depthBuffer;
}

bool FrameBuffer::WriteToColor(int w, int h, const ColorFormat& col)
{
	if (w < 0 || h < 0 || w >= width || h >= height) return false;

	colorBuffer[width * h + w] = col;
	return true;
}

bool FrameBuffer::WriteToDepth(int w, int h, float col)
{
	if (w < 0 || h < 0 || w >= width || h >= height) return false;
	depthBuffer[width * h + w] = col;
	return true;
}

float FrameBuffer::GetDepth(int w, int h) const
{
	return depthBuffer[width * h + w];
}

void FrameBuffer::Fill(const ColorFormat& col)
{
	for (int i = 0; i < width * height; ++i)
	{
		colorBuffer[i] = col;
		depthBuffer[i] = std::numeric_limits<float>::max();
	}
}

ColorFormat* FrameBuffer::GetBufferCopy() const
{
	ColorFormat* copy = new ColorFormat[width * height];

	for (int i = 0; i < width*height; ++i)
	{
		copy[i] = colorBuffer[i];
	}
	return copy;
}

RGB888 * FrameBuffer::GetBufferCopy_RGB888() const
{
	RGB888* copy = new RGB888[width * height];

	for (int i = 0; i < width*height; ++i)
	{
		copy[i] = RGB888((int) (colorBuffer[i].x * 255), (int) (colorBuffer[i].y * 255), (int) (colorBuffer[i].z * 255));
	}

	return copy;
}
