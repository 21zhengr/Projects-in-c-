#include "SrcMain.h"
#include <string>
#include "RleFile.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO: Process argc and argv, and use to compress/decompress files
	if (argc == 2) {
		RleFile mRleFile;
		std::string filename = argv[1];
		std::size_t rl1 = filename.find(".rl1");

		if (rl1 != std::string::npos) { //means that it found .rl1 --> call extract
			mRleFile.ExtractArchive(filename);
		}
		else { // did not find --> create
			mRleFile.CreateArchive(filename);
		}
	}
}
