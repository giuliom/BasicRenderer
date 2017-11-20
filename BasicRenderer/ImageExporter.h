#pragma once

#include <stdio.h>
#include <memory>
#include "FrameBuffer.h"

class ImageExporter
{

public:

	static bool ExportToRaw(const char* path, const std::shared_ptr<const FrameBuffer> fBuf);



};