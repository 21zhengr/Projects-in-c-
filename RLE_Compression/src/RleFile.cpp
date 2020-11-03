#include "RleFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

void RleFile::CreateArchive(const std::string& source)
{
	std::ifstream::pos_type size;
	char* memblock;
	// Open the file for input, in binary mode, and start ATE (at the end)
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg(); // Save the size of the file
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg); // Seek back to start of file
		file.read(memblock, size);
		file.close();
		// File data is now in memblock array
		// (Do something with it here...) --> set mHeader?
		
		//original file size
		mHeader.mFileSize = static_cast<unsigned int>(size);
		//original file name
		mHeader.mFileName = source;
		//original file name length
		mHeader.mFileNameLength = source.length();
		//calling Compress on mRleData to compress memblock, passed in size of memblock array
		mRleData.Compress(memblock, static_cast<unsigned int>(size));

		// Open the file for output, in binary mode, and overwrite an existing file
		std::ofstream arc(source + ".rl1", std::ios::out | std::ios::binary | std::ios::trunc);
		if (arc.is_open())
		{
			// Use arc.write function to write data here
			//magic numbers
			arc.write(mHeader.mSig, 4);
			//original file size
			arc.write(reinterpret_cast<char*>(&(mHeader.mFileSize)), 4);
			//filename length
			char fileNameLength[1] = { static_cast<char>(mHeader.mFileNameLength) };
			arc.write(fileNameLength, 1);
			//filename
			const char* filenameCstr = mHeader.mFileName.c_str();
			arc.write(filenameCstr, mHeader.mFileNameLength);
			//data
			arc.write(mRleData.mData, mRleData.mSize);
		}

	
		// Make sure to clean up!
		delete[] memblock;
	}
}

void RleFile::ExtractArchive(const std::string& source)
{
	std::ifstream::pos_type size;
	char* memblock;
	// Open the file for input, in binary mode, and start ATE (at the end)
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg(); // Save the size of the file
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg); // Seek back to start of file
		file.read(memblock, size);
		file.close();
		// File data is now in memblock array

		//first grab original file size
		mHeader.mFileSize = *(reinterpret_cast<int*>(&memblock[4]));
		//then grab file name length at the 9th byte which is index 8
		mHeader.mFileNameLength =  memblock[8];
		int index = 9 + static_cast<unsigned int>(mHeader.mFileNameLength);
		//grab name of file
		for (int i = 9; i < index; ++i) {
			mHeader.mFileName += memblock[i];
		}

		//pass to decompress
		mRleData.Decompress(&memblock[index], (static_cast<unsigned int>(size) - index), mHeader.mFileSize);

		std::ofstream arc(mHeader.mFileName, std::ios::out | std::ios::binary | std::ios::trunc);
		if (arc.is_open()) 
		{
			arc.write(mRleData.mData, mHeader.mFileSize);
		}
		// Make sure to clean up!
		delete[] memblock;
	}
}
