#include "FrameBuffer.h"
#include <limits>


FrameBuffer::~FrameBuffer()
{
	delete[] colorBuffer;
	delete[] depthBuffer;
}

bool FrameBuffer::WriteToColor(int w, int h, const Color& col)
{
	assert(w >= 0 && w < width && h >= 0 && h <width);
	colorBuffer[width * h + w] = col;
	return true;
}

bool FrameBuffer::WriteToColor(int index, const Color & col)
{
	assert(index >= 0 && index < size);
	colorBuffer[index] = col;
	return true;
}

bool FrameBuffer::WriteToDepth(int w, int h, float col)
{
	assert(w >= 0 && w < width && h >= 0 && h <width);
	depthBuffer[width * h + w] = col;
	return true;
}

bool FrameBuffer::WriteToDepth(int index, float col)
{
	assert(index >= 0 && index < size);
	depthBuffer[index] = col;
	return true;
}

float FrameBuffer::GetDepth(int w, int h) const
{
	assert(w >= 0 && w < width && h >= 0 && h <width);
	return depthBuffer[width * h + w];
}

float FrameBuffer::GetDepth(int index) const
{
	assert(index >= 0 && index < size);
	return depthBuffer[index];
}

const Color& FrameBuffer::GetColor(int w, int h) const
{
	assert(w >= 0 && w < width && h >= 0 && h <width);
	return colorBuffer[width * h + w];
}

const Color & FrameBuffer::GetColor(int index) const
{
	assert(index >= 0 && index < size);
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

const Color * const FrameBuffer::GetColorBuffer() const
{
	return colorBuffer;
}

Color* FrameBuffer::GetColorBufferCopy() const
{
	Color* copy = new Color[width * height];

	for (int i = 0; i < width*height; ++i)
	{
		copy[i] = colorBuffer[i];
	}
	return copy;
}

