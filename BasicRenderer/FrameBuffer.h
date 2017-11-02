#pragma once

#include "ImageFormats.h"
#include "PrimitiveTypes.h"


class FrameBuffer
{
protected:
	int width = 640;
	int height = 480;

	Color* colorBuffer = nullptr;
	float* depthBuffer = nullptr;


public:
	FrameBuffer(const int width_, const int height_) : width(width_), height(height_)
	{
		colorBuffer = new Color[width_ * height_];
		depthBuffer = new float[width_ * height_];
	}
	~FrameBuffer();

	bool WriteToColor(int w, int h, const Color& col);
	bool WriteToDepth(int w, int h, float col);

	float GetDepth(int w, int h) const;
	const Color& GetColor(int w, int h) const;

	void Fill(const Color& col);
	Color* GetBufferCopy() const;

};

