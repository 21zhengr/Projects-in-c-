#include "SrcMain.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "HashTable.hpp"
#include "sha1.h"

#include <tbb/parallel_for_each.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO: Implement
	std::string argv1 = argv[1];
	size_t hashTableSize = std::stoi(argv[2]);
	HashTable<std::string> ht(hashTableSize);
	std::ofstream ofile("solved.txt");

	if (argv1 == "dictionary") {
		std::ifstream dictionaryFile(argv[3]),
						passwordFile(argv[4]);
		

		if (!dictionaryFile.fail() && !passwordFile.fail()) {
			std::string line, hexStr;
			unsigned char hash[20];
			//read file line by line and insert into HashTable
			while (dictionaryFile >> line) {
				sha1::Calc(line.c_str(), line.length(), hash);
				
				hexStr.resize(40);
				sha1::ToHexString(hash, &hexStr[0]);
				
				ht.Insert(hexStr, line);
			}

			//now start looping through password file and outputing to outfile
			while (passwordFile >> line) {
				std::string *plainTxtPwd = ht.Find(line);
				ofile << line << "," << *plainTxtPwd << "\n";
			}
		}

	}
	else {
		std::ifstream wordsFile(argv[3]),
			passwordFile(argv[4]);
		if (!wordsFile.fail() && !passwordFile.fail()) {
			std::string line; /*hexStr;
			unsigned char hash[20];*/

			std::vector<std::string> words, permutations;
			while (wordsFile >> line) {
				words.push_back(line);
			}

			int cnt = 0;
			while (passwordFile >> line) {
				cnt += 1;
				ht.Insert(line,std::to_string(cnt)); // to store the order
			}
			std::string *order = new std::string[ht.Size()];

			tbb::parallel_for(tbb::blocked_range<size_t>(0, words.size()), 
				[&words, &ht, &order](const tbb::blocked_range<size_t>& r) {
					std::string str;
					int cnt = 0;
					std::string hexStr;
					unsigned char hash[20];
					for (auto one = r.begin(); one != r.end(); ++one) {
						//line = words[one];
						for (auto two = 0; two < words.size(); ++two) {
							//line += words[two];
							for (auto three = 0; three < words.size(); ++three) {
								//line += words[three];
								for (auto four = 0; four < words.size(); ++four) {
									str = words[one];
									str += words[two];
									str += words[three]; 
									str += words[four];
									
									sha1::Calc(str.c_str(), str.length(), hash);

									hexStr.resize(40);
									sha1::ToHexString(hash, &hexStr[0]);

									if (ht.Find(hexStr) != nullptr) {
										cnt = std::stoi(*ht.Find(hexStr));
										cnt -= 1;
										order[cnt] = (hexStr + "," + str);
										*ht.Find(hexStr) = str;
									}

								}
							}
						}
					}

				});

			//for (auto one = 0; one < words.size(); ++one) {
			//	//line = words[one];
			//	for (auto two = 0; two < words.size(); ++two) {
			//		//line += words[two];
			//		for (auto three = 0; three < words.size(); ++three) {
			//			//line += words[three];
			//			for (auto four = 0; four < words.size(); ++four) {
			//				//line += words[four];
			//				permutations.push_back(words[one] + words[two] + words[three] + words[four]);
			//			}
			//		}
			//	}
			//}

			/*std::for_each(permutations.begin(), permutations.end(),
				[&cnt, &hexStr, &hash, &ht, &order, &line] (const std::string phrase){
					sha1::Calc(phrase.c_str(), phrase.length(), hash);

					hexStr.resize(40);
					sha1::ToHexString(hash, &hexStr[0]);

					if (ht.Find(hexStr) != nullptr) {
						cnt = std::stoi(*ht.Find(hexStr));
						cnt -= 1;
						order[cnt] = (hexStr + "," + phrase);
						*ht.Find(hexStr) = phrase;
					}

				});*/

			for (int i = 0; i < ht.Size(); ++i) {
				ofile << order[i] << "\n";

			}
			
			delete[] order;


		}
	}
}
