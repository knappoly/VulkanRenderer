#include "Engine/File/File.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "ThirdParty/stb/stb_image_write.h"

//-----------------------------------------------------------------------------------------------
// Reads the file into a buffer
//
void* FileReadToNewBuffer(char const *filename, size_t *out_size /*=nullptr */)
{
	
	FILE *fp = nullptr;
	fopen_s( &fp, filename, "r" );

	if (fp == nullptr) {
		return nullptr;
	}

	size_t size = 0U;
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	unsigned char *buffer = (unsigned char*) malloc(size + 1U); // space for NULL

	size_t read = fread( buffer, 1, size, fp );
	fclose(fp);

	buffer[read] = NULL;
	
	// Returns size if the argument is passed
	if(out_size)
	{
		*out_size = read;
	}
	return buffer; 
	
}

//-----------------------------------------------------------------------------------------------
// Reads a binary file into a buffer
//
void* FileBinaryReadToNewBuffer(const char* fileName, size_t* outSize /*= nullptr */)
{
	FILE *fp = nullptr;
	fopen_s( &fp, fileName, "rb" );

	if (fp == nullptr) {
		return nullptr;
	}

	size_t size = 0U;
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	unsigned char *buffer = (unsigned char*) malloc(size + 1U); // space for NULL

	size_t read = fread( buffer, 1, size, fp );
	fclose(fp);

	buffer[read] = NULL;

	// Returns size if the argument is passed
	if(outSize)
	{
		*outSize = size;
	}
	return buffer; 
}

//-----------------------------------------------------------------------------------------------
// Writes data into a file
//
bool FileWriteToNewFile(const char* fileName, const char* bufferData, size_t count)
{
	FILE *fp = nullptr;
	fopen_s( &fp, fileName, "w" );

	if (fp == nullptr) {
		return false;
	}

	fseek(fp, 0L, SEEK_SET);

	fwrite(bufferData, sizeof(char), count, fp);
	fclose(fp);

	return true;
}

//-----------------------------------------------------------------------------------------------
// Writes data into a png
//
bool WriteToPng(const char* filename, const unsigned char* data, int width, int height, int numComponents)
{
	stbi_flip_vertically_on_write(true);
	if(stbi_write_png(filename, width, height, numComponents, data, 0) == 0)
		return false;
	stbi_flip_vertically_on_write(false);
	return true;
}
