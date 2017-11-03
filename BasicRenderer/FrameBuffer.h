#pragma once

#include "ImageFormats.h"
#include "PrimitiveTypes.h"


class FrameBuffer
{
protected:
	int width = 640;
	int height = 480;
	int size = 640 * 480;

	Color* colorBuffer = nullptr;
	float* depthBuffer = nullptr;


public:
	FrameBuffer(const int width_, const int height_) : width(width_), height(height_)
	{
		colorBuffer = new Color[width_ * height_];
		depthBuffer = new float[width_ * height_];
		size = width_ * height_;
	}
	~FrameBuffer();

	bool WriteToColor(int w, int h, const Color& col);
	bool WriteToColor(int index, const Color& col);
	bool WriteToDepth(int w, int h, float col);
	bool WriteToDepth(int index, float col);

	float GetDepth(int w, int h) const;
	float GetDepth(int index) const;
	const Color& GetColor(int w, int h) const;
	const Color& GetColor(int index) const;

	void Fill(const Color& col);
	Color* GetBufferCopy() const;

};

