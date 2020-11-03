#include <string>
#include <iostream>
#include <fstream>

class Fasta
{
public:
	std::string mHeader, mSequence;

	Fasta(const std::string& fileName) {
		std::ifstream::pos_type size;
		// Open the file for input, in binary mode, and start ATE (at the end)
		std::ifstream file(fileName, std::ios::in | std::ios::ate);
		if (file.is_open())
		{
			std::string line;
			size = file.tellg(); // Save the size of the file
			//string reserve for mSequence
			mSequence.reserve(size);
			file.seekg(0, std::ios::beg); // Seek back to start of file

			//first line is the header
			std::getline(file, mHeader);
			mHeader.erase(mHeader.begin());
			//read in rest of file
			while (std::getline(file, line)) {
				mSequence += line;
			}

			//std::string debugging_bruh = mSequence;
		}
	}
	
};