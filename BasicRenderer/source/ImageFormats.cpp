#include "ImageFormats.h"

namespace BasicRenderer
{
	uint32_t* ImageFormats::ToRGBA8888(const FrameBuffer* const buf)
	{
		const int size = buf->GetWidth() * buf->GetHeight();
		uint32_t* output = new uint32_t[size];

		const Color* c = buf->GetColorBuffer();

		for (int i = 0; i < size; ++i)
		{
			uint32_t ua = 0;
			ua |= static_cast<uint32_t>(c[i].r * 255.99f) << 24;
			ua |= static_cast<uint32_t>(c[i].g * 255.99f) << 16;
			ua |= static_cast<uint32_t>(c[i].b * 255.99f) << 8;
			ua |= 255;

			output[i] = ua;
		}

		return output;
	}
}
