#include "ImageExporter.h"
#include <fstream>
#include "ImageFormats.h"

namespace BasicRenderer
{
	bool ImageExporter::ExportToPPM(const char* path, const std::shared_ptr<const FrameBuffer> fBuf)
	{
		std::ofstream outfile;
		outfile.open(*path + "render.ppm");

		int width = fBuf->GetWidth();
		int height = fBuf->GetHeight();

		outfile << "P3\n" << width << " " << height << "\n255\n";

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Color c = fBuf->GetColor(i * width + j);
				int r = int(255.99f * c.x);
				int g = int(255.99f * c.y);
				int b = int(255.99f * c.z);

				outfile << r << " " << g << " " << b << "\n";
			}
		}

		return true;
	}

	bool ImageExporter::ExportToBMP(const char* path, const std::shared_ptr<const FrameBuffer> fBuf)
	{
		std::ofstream outfile;
		std::string filePath = path;
		outfile.open((filePath + "render.bmp").c_str(), std::ofstream::binary);

		if (!outfile.is_open()) return false;

		std::vector<uint8_t> bmpData;
		uint32_t width = fBuf->GetWidth();
		uint32_t height = fBuf->GetHeight();

		bmpData.emplace_back(0x42);
		bmpData.emplace_back(0x4D);
		size_t fileSize_offset = bmpData.size();
		Fill4Bytes(bmpData, 0xFFFFFFFF); //Size, to be filled later 
		bmpData.emplace_back(0x00); //Reserved data
		bmpData.emplace_back(0x00);
		bmpData.emplace_back(0x00);
		bmpData.emplace_back(0x00);
		size_t pixelInfo_offset = bmpData.size();
		Fill4Bytes(bmpData, 0x00); //Pixel offset filled later
		Fill4Bytes(bmpData, 40); //BITMAPINFOHEADER
		Fill4Bytes(bmpData, width);
		Fill4Bytes(bmpData, height);
		Fill2Bytes(bmpData, 1); //Color planes
		Fill2Bytes(bmpData, 24); //Bits per Pixel
		Fill4Bytes(bmpData, 0); //Compression disabled
		size_t rawDataSize_offset = bmpData.size();
		Fill4Bytes(bmpData, 0); //size of raw data in pixel array filled later
		Fill4Bytes(bmpData, 2835); //Horizontal resolution
		Fill4Bytes(bmpData, 2835); //Vertical resolution
		Fill4Bytes(bmpData, 0); //Number of colors
		Fill4Bytes(bmpData, 0); //Important colors

		uint32_t headerPadding = bmpData.size() % 4;
		for (uint32_t p = 0; p < headerPadding; p++)
		{
			bmpData.emplace_back(0);
		}
		size_t headerSize = bmpData.size();
		memcpy(&bmpData[pixelInfo_offset], &headerSize, 4);

		const Color* cBuf = fBuf->GetColorBuffer();

		//Flipped image along the y axis
		for (uint32_t j = 0; j < height; j++)
		{
			for (uint32_t i = 0; i < width; i++)
			{
				//BGR
				uint32_t index = (height - j - 1) * width + i;
				Color c = cBuf[index];
				bmpData.emplace_back((uint8_t)(c.z * 255.99f));
				bmpData.emplace_back((uint8_t)(c.y * 255.99f));
				bmpData.emplace_back((uint8_t)(c.x * 255.99f));
			}

			uint32_t padding = bmpData.size() % 4;
			for (uint32_t p = 0; p < padding; p++)
			{
				bmpData.emplace_back(0);
			}
		}

		size_t fileSize = bmpData.size();
		memcpy(&bmpData[fileSize_offset], &fileSize, 4);

		size_t pixelDataSize = bmpData.size() - headerSize;
		memcpy(&bmpData[rawDataSize_offset], &pixelDataSize, 4);

		uint8_t* data = bmpData.data();
		outfile.write((const char*)data, bmpData.size());
		outfile.close();

		return false;
	}

	void ImageExporter::Fill4Bytes(std::vector<uint8_t>& dest, const uint32_t value)
	{
		uint8_t* temp = (uint8_t*)&value;
		dest.emplace_back(temp[0]);
		dest.emplace_back(temp[1]);
		dest.emplace_back(temp[2]);
		dest.emplace_back(temp[3]);
	}

	void ImageExporter::Fill2Bytes(std::vector<uint8_t>& dest, const uint16_t value)
	{
		uint8_t* temp = (uint8_t*)&value;
		dest.emplace_back(temp[0]);
		dest.emplace_back(temp[1]);
	}
}

