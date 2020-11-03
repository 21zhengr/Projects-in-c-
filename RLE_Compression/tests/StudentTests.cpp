#include "catch.hpp"
#include "RleData.h"
#include "RleFile.h"
#include "SrcMain.h"

// Helper function declarations (don't change these)
extern bool BuffersAreSame(const char* expected, const char* actual, size_t size);
extern bool RunCompressionTest(char* test, size_t testSize,
							   char* expected, size_t expectedSize);
extern bool RunDecompressionTest(char* test, size_t testSize,
								 char* expected, size_t expectedSize);
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);

TEST_CASE("RLE Compression", "[student]")
{
	SECTION("Basic positive run")
	{
		char test[] = "aaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbc"
		"ccdddaaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbcccddd";
		char expected[] = "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d";	
		
		bool result = RunCompressionTest(test, sizeof(test) - 1,
										 expected, sizeof(expected) - 1);
		REQUIRE(result);
	}
	
	// TODO: Add more test case sections!!
	SECTION("Long positive run")
	{
		char test[] = "aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiiijjjjj"
			"kkkkklllllmmmmmnnnnnooooopppppqqqqqrrrrrssssstttttuuuuuvvvvvwwwwwxxxxxyyyyyzzzzz"
			"aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiiijjjjj"
			"kkkkklllllmmmmmnnnnnooooopppppqqqqqrrrrrssssstttttuuuuuvvvvvwwwwwxxxxxyyyyyzzzzz"
			"aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiiijjjjj"
			"kkkkklllllmmmmmnnnnnooooopppppqqqqqrrrrrssssstttttuuuuuvvvvvwwwwwxxxxxyyyyyzzzzz"
			"aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiiijjjjj"
			"kkkkklllllmmmmmnnnnnooooopppppqqqqqrrrrrssssstttttuuuuuvvvvvwwwwwxxxxxyyyyyzzzzz";
		
		char expected[] = "\x05" "a" "\x05" "b" "\x05" "c" "\x05" "d" "\x05" "e" "\x05" "f"
			"\x05" "g" "\x05" "h" "\x05" "i" "\x05" "j" "\x05" "k" "\x05" "l" "\x05" "m"
			"\x05" "n" "\x05" "o" "\x05" "p" "\x05" "q" "\x05" "r" "\x05" "s" "\x05" "t"
			"\x05" "u" "\x05" "v" "\x05" "w" "\x05" "x" "\x05" "y" "\x05" "z"
			"\x05" "a" "\x05" "b" "\x05" "c" "\x05" "d" "\x05" "e" "\x05" "f"
			"\x05" "g" "\x05" "h" "\x05" "i" "\x05" "j" "\x05" "k" "\x05" "l" "\x05" "m"
			"\x05" "n" "\x05" "o" "\x05" "p" "\x05" "q" "\x05" "r" "\x05" "s" "\x05" "t"
			"\x05" "u" "\x05" "v" "\x05" "w" "\x05" "x" "\x05" "y" "\x05" "z"
			"\x05" "a" "\x05" "b" "\x05" "c" "\x05" "d" "\x05" "e" "\x05" "f"
			"\x05" "g" "\x05" "h" "\x05" "i" "\x05" "j" "\x05" "k" "\x05" "l" "\x05" "m"
			"\x05" "n" "\x05" "o" "\x05" "p" "\x05" "q" "\x05" "r" "\x05" "s" "\x05" "t"
			"\x05" "u" "\x05" "v" "\x05" "w" "\x05" "x" "\x05" "y" "\x05" "z"
			"\x05" "a" "\x05" "b" "\x05" "c" "\x05" "d" "\x05" "e" "\x05" "f"
			"\x05" "g" "\x05" "h" "\x05" "i" "\x05" "j" "\x05" "k" "\x05" "l" "\x05" "m"
			"\x05" "n" "\x05" "o" "\x05" "p" "\x05" "q" "\x05" "r" "\x05" "s" "\x05" "t"
			"\x05" "u" "\x05" "v" "\x05" "w" "\x05" "x" "\x05" "y" "\x05" "z";

		bool result = RunCompressionTest(test, sizeof(test) - 1, 
										 expected, sizeof(expected) - 1);

		REQUIRE(result);
	}

	SECTION("Basic negative run")
	{
		char test[] = "abcdefghijklmnopqrstuvwxyz";

		char expected[] = "\xe6" "abcdefghijklmnopqrstuvwxyz";

		bool result = RunCompressionTest(test, sizeof(test) - 1,
										 expected, sizeof(expected) - 1);

		REQUIRE(result);
	}

	SECTION("Long negative run")
	{
		char test[] = "abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdef"
					"abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdef";

		char expected[] = "\x82" "abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdef"
								"abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdef";

		bool result = RunCompressionTest(test, sizeof(test) - 1,
										 expected, sizeof(expected) - 1);

		REQUIRE(result);
	}

	SECTION("Basic alternating run")
	{
		char test[] = "aabbcdeaabbcdeaabbcdeaabbcdeaabbcdeaabbcdeaabbcdeaabbcde";

		char expected[] = "\x02" "a" "\x02" "b" "\xfd" "cde" "\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde" "\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde" "\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde""\x02" "a" "\x02" "b" "\xfd" "cde";

		bool result = RunCompressionTest(test, sizeof(test) - 1,
			expected, sizeof(expected) - 1);

		REQUIRE(result);
	}
	
	SECTION("Long alternating run")
	{
		char test[] = "aabbcdeaabbcdeaabbcdeaabbcdeaabbcdeaabbcdeaabbcdeaabbcde"
			"aabbcdeaabbcdeaabbcdeaabbcdeaabbcdeaabbcdeaabbcdeaabbcde"
			"aabbcdeaabbcdeaabbcdeaabbcdeaabbcdeaabbcdeaabbcdeaabbcde";

		char expected[] = "\x02" "a" "\x02" "b" "\xfd" "cde" "\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde" "\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde" "\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde""\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde" "\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde" "\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde" "\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde""\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde" "\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde" "\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde" "\x02" "a" "\x02" "b" "\xfd" "cde"
			"\x02" "a" "\x02" "b" "\xfd" "cde""\x02" "a" "\x02" "b" "\xfd" "cde";

		bool result = RunCompressionTest(test, sizeof(test) - 1,
			expected, sizeof(expected) - 1);

		REQUIRE(result);
	}

	SECTION("Greater-than-max-run-size postive run") //don't forget edge case with short negative run at the end
	{
		char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"; //200 a's

		char expected[] = "\x7f" "a" "\x49" "a"; // 127 a 73 a

		bool result = RunCompressionTest(test, sizeof(test) - 1,
			expected, sizeof(expected) - 1);

		REQUIRE(result);
	}

	SECTION("Greater-than-max-run-size negative run")
	{
		char test[] = "abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqr"; //200 letters -> just looped through alphabet 7x = 182 and added 18 of the 8th alphabet set

		char expected[] = "\x81" "abcdefghijklmnopqrstuvwxyz"
								"abcdefghijklmnopqrstuvwxyz"
								"abcdefghijklmnopqrstuvwxyz"
								"abcdefghijklmnopqrstuvwxyz"
								"abcdefghijklmnopqrstuvw" 
							"\xb7" "xyz"
								"abcdefghijklmnopqrstuvwxyz"
								"abcdefghijklmnopqr"; 

		bool result = RunCompressionTest(test, sizeof(test) - 1,
			expected, sizeof(expected) - 1);

		REQUIRE(result);
	}
}

TEST_CASE("RLE Decompression", "[student]")
{
	SECTION("Basic positive run")
	{
		char test[] = "\x28" "x";
		char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		
		bool result = RunDecompressionTest(test, sizeof(test) - 1,
										   expected, sizeof(expected) - 1);
		REQUIRE(result);
	}
	
	// TODO: Add more test case sections!!
	SECTION("Basic negative run")
	{
		char test[] = "\xf9" "abcdefg";
		char expected[] = "abcdefg";

		bool result = RunDecompressionTest(test, sizeof(test) - 1,
			expected, sizeof(expected) - 1);
		REQUIRE(result);
	}

	SECTION("Basic Alternating run")
	{
		char test[] = "\xf9" "abcdefg" "\x28" "x";
		char expected[] = "abcdefg" "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

		bool result = RunDecompressionTest(test, sizeof(test) - 1,
			expected, sizeof(expected) - 1);
		REQUIRE(result);
	}

	SECTION("Long Alternating run")
	{
		char test[] = "\xf9" "abcdefg" "\x28" "x"
						"\xf9" "abcdefg" "\x28" "x"
						"\xf9" "abcdefg" "\x28" "x";

		char expected[] = "abcdefg" "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
							"abcdefg" "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
							"abcdefg" "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

		bool result = RunDecompressionTest(test, sizeof(test) - 1,
			expected, sizeof(expected) - 1);
		REQUIRE(result);
	}
	

}

TEST_CASE("Command line arguments", "[student]")
{
	SECTION("instructions.txt.rl1")
	{
		const char* argv[] = {
			"tests/tests",
			"data/instructions.txt.rl1"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckFileMD5("data/instructions.txt", "5e042d2e4bf014e714dd7cfc4dc25aab");
		REQUIRE(result);
	}
}

TEST_CASE("File compression", "[student]")
{
	RleFile r;
	SECTION("rle.bmp")
	{
		r.CreateArchive("data/rle.bmp");
		bool result = CheckFileMD5("data/rle.bmp.rl1", "f2a9d8425d53c664e45d9eb1b53137b9");
		REQUIRE(result);
	}
	SECTION("pic.jpg")
	{
		r.CreateArchive("data/pic.jpg");
		bool result = CheckFileMD5("data/pic.jpg.rl1", "0bbf2a5109b30d79939d2061ea8c74aa");
		REQUIRE(result);
	}
	SECTION("Conquest.ogg")
	{
		r.CreateArchive("data/Conquest.ogg");
		bool result = CheckFileMD5("data/Conquest.ogg.rl1", "ec29ff368ec5100bfba22635ddc5ba5c");
		REQUIRE(result);
	}
}

TEST_CASE("File decompression", "[student]")
{
	RleFile r;
	SECTION("works.bmp.rl1")
	{
		r.ExtractArchive("data/works.bmp.rl1");
		bool result = CheckFileMD5("data/works.bmp", "8baad647acefae2f8c36ee111582a875");
		REQUIRE(result);
	}
	SECTION("xkcd.bmp.rl1")
	{
		r.ExtractArchive("data/xkcd.bmp.rl1");
		bool result = CheckFileMD5("data/xkcd.bmp", "a4d7efa89d47a94a76f8050dd8d60cd0");
		REQUIRE(result);
	}
	SECTION("logo.png.rl1")
	{
		r.ExtractArchive("data/logo.png.rl1");
		bool result = CheckFileMD5("data/logo.png", "95403309460632f60efa647ef59b78fc");
		REQUIRE(result);
	}
}

