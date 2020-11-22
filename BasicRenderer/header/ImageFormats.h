#pragma once

#include <stdint.h>
#include "FrameBuffer.h"

namespace BasicRenderer
{
	class ImageFormats
	{

	public:

		static uint32_t* ToRGBA8888(const FrameBuffer* const buf);
	};
}