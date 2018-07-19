#pragma once

//-----------------------------------------------------------------------------------------------
// Reads the file into a buffer
void* FileReadToNewBuffer( char const *filename, size_t *out_size = nullptr );

//-----------------------------------------------------------------------------------------------
// Reads binary file into a buffer
void* FileBinaryReadToNewBuffer( const char* fileName, size_t* outSize = nullptr );

//-----------------------------------------------------------------------------------------------
// Writes a buffer into a file
bool FileWriteToNewFile( const char* filename, const char* data, size_t length );


//-----------------------------------------------------------------------------------------------
// Write to a png file
bool WriteToPng( const char* filename, const unsigned char* data, int width, int height, int numComponents );