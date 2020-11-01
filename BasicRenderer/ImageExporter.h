#pragma once

#include <stdio.h>
#include <memory>
#include <vector>
#include <string>
#include "FrameBuffer.h"

namespace BasicRenderer
{
	class ImageExporter
	{

	public:

		static bool ExportToPPM(std::string& path, std::string& filename, const FrameBuffer& fBuf);
		static bool ExportToBMP(std::string& path, std::string& filename, const FrameBuffer& fBuf);

		static void Fill4Bytes(std::vector<uint8_t>& dest, const uint32_t value);
		static void Fill2Bytes(std::vector<uint8_t>& dest, const uint16_t value);
	};
}