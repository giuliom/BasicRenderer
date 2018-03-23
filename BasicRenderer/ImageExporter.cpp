#include "ImageExporter.h"
#include <fstream>
#include "ImageFormats.h"

bool ImageExporter::ExportToRaw(const char * path, const std::shared_ptr<const FrameBuffer> fBuf)
{
	std::ofstream outfile;
	outfile.open(*path + "render.tga", std::ios::binary);

	return true;
}

bool ImageExporter::ExportToBMP(const char * path, const std::shared_ptr<const FrameBuffer> fBuf)
{
	std::ofstream outfile;
	outfile.open(*path + "render.bmp", std::ios::binary);

	std::vector<uint8_t> bmpData;

	bmpData.push_back('B');
	bmpData.push_back('M');
	size_t fileSize_offset = bmpData.size();
	Fill4Bytes(bmpData, 0x00); //Size, to be filled later TODO
	Fill4Bytes(bmpData, 0x00); //Reserved data
	size_t pixelInfo_offset = bmpData.size();
	Fill4Bytes(bmpData, 0x00); //Pixel offset TODO
	Fill4Bytes(bmpData, 40); //BITMAPINFOHEADER
	Fill4Bytes(bmpData, fBuf->GetWidth());
	Fill4Bytes(bmpData, fBuf->GetHeight());
	Fill2Bytes(bmpData, 1); //Color planes
	Fill2Bytes(bmpData, 24); //Bits per Pixel
	Fill4Bytes(bmpData, 0); //Compression disabled
	size_t rawDataSize_offset = bmpData.size();
	Fill4Bytes(bmpData, 0); //size of raw data in pixel array, TODO
	Fill4Bytes(bmpData, 2835); //Horizontal resolution
	Fill4Bytes(bmpData, 2835); //Vertical resolution
	Fill4Bytes(bmpData, 0); //Number of colors
	Fill4Bytes(bmpData, 0); //Important colors

	uint32_t dataSize = bmpData.size();
	memcpy(&bmpData[pixelInfo_offset], &dataSize, 4);

	return false;
}

void ImageExporter::Fill4Bytes(std::vector<uint8_t>& dest, const uint32_t value)
{
	uint8_t* temp = (uint8_t*) &value;
	dest.push_back(temp[0]);
	dest.push_back(temp[1]);
	dest.push_back(temp[2]);
	dest.push_back(temp[3]);
}

void ImageExporter::Fill2Bytes(std::vector<uint8_t>& dest, const uint16_t value)
{
	uint8_t* temp = (uint8_t*)&value;
	dest.push_back(temp[0]);
	dest.push_back(temp[1]);
}


