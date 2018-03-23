#pragma once

#include <stdint.h>
#include "FrameBuffer.h"

class ImageFormats
{
	
public:
	
	static uint32_t* ToRGBA8888(const FrameBuffer * const buf);
};