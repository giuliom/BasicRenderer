#pragma once

#include <stdio.h>
#include <memory>
#include <vector>
#include "FrameBuffer.h"

class ImageExporter
{

public:

	static bool ExportToPPM(const char* path, const std::shared_ptr<const FrameBuffer> fBuf);

	static bool ExportToBMP(const char* path, const std::shared_ptr<const FrameBuffer> fBuf);

	static void Fill4Bytes(std::vector<uint8_t>& dest, const uint32_t value);
	static void Fill2Bytes(std::vector<uint8_t>& dest, const uint16_t value);
};