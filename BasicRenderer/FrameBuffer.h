#pragma once

#include "ImageFormats.h"
#include "PrimitiveTypes.h"

typedef Color ColorFormat;

class FrameBuffer
{
protected:
	int width = 640;
	int height = 480;

	ColorFormat* colorBuffer = nullptr;
	float* depthBuffer = nullptr;


public:
	FrameBuffer(const int width_, const int height_) : width(width_), height(height_)
	{
		colorBuffer = new ColorFormat[width_ * height_];
		depthBuffer = new float[width_ * height_];
	}
	~FrameBuffer();

	bool WriteToColor(int w, int h, const ColorFormat& col);
	bool WriteToDepth(int w, int h, float col);

	float GetDepth(int w, int h) const;

	void Fill(const ColorFormat& col);
	ColorFormat* GetBufferCopy() const;

	RGB888* GetBufferCopy_RGB888() const;
};

