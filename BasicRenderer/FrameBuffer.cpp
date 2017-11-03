#include "FrameBuffer.h"
#include <limits>


FrameBuffer::~FrameBuffer()
{
	delete[] colorBuffer;
	delete[] depthBuffer;
}

bool FrameBuffer::WriteToColor(int w, int h, const Color& col)
{
	if (w < 0 || h < 0 || w >= width || h >= height) return false;

	colorBuffer[width * h + w] = col;
	return true;
}

bool FrameBuffer::WriteToColor(int index, const Color & col)
{
	if (index < 0 || index >= size) return false;
	colorBuffer[index] = col;
	return true;
}

bool FrameBuffer::WriteToDepth(int w, int h, float col)
{
	if (w < 0 || h < 0 || w >= width || h >= height) return false;
	depthBuffer[width * h + w] = col;
	return true;
}

bool FrameBuffer::WriteToDepth(int index, float col)
{
	if (index < 0 || index >= size) return false;
	depthBuffer[index] = col;
	return true;
}

float FrameBuffer::GetDepth(int w, int h) const
{
	return depthBuffer[width * h + w];
}

float FrameBuffer::GetDepth(int index) const
{
	return depthBuffer[index];
}

const Color& FrameBuffer::GetColor(int w, int h) const
{
	return colorBuffer[width * h + w];
}

const Color & FrameBuffer::GetColor(int index) const
{
	return colorBuffer[index];
}

void FrameBuffer::Fill(const Color& col)
{
	float x = col.x;
	float y = col.y;
	float z = col.z;
	for (int i = 0; i < size; ++i)
	{
		colorBuffer[i].x = x;
		colorBuffer[i].y = y;
		colorBuffer[i].z = z;
		depthBuffer[i] = std::numeric_limits<float>::max();
	}
}

Color* FrameBuffer::GetBufferCopy() const
{
	Color* copy = new Color[width * height];

	for (int i = 0; i < width*height; ++i)
	{
		copy[i] = colorBuffer[i];
	}
	return copy;
}

