#include "ImageExporter.h"
#include <fstream>

bool ImageExporter::ExportToRaw(const char * path, const std::shared_ptr<const FrameBuffer> fBuf)
{
	std::ofstream outfile;
	outfile.open(*path + "render.tga", std::ios::binary);

	return true;
}
