#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>


class DNATranslate {
public:
	//stop codon is going to be X
	char mTable[24][4] = { {0,0,1,0}, {2,0,1,0}, {0,0,1,'M'}, {4,9,14,19},
					{5,6,7,8}, {'F','F','L','L'}, {'S','S','S','S'}, {'Y','Y','X','X'},
					{'C','C','X','W'}, {10,11,12,13},{'L','L','L','L'}, {'P','P','P','P'},
					{'H','H','Q','Q'}, {'R','R','R','R'}, {15,16,17,18}, {'I','I','I','M'},
					{'T','T','T','T'}, {'N','N','K','K'}, {'S','S','R','R'}, {20,21,22,23},
					{'V','V','V','V'}, {'A','A','A','A'}, {'D','D','E','E'}, {'G','G','G','G'} };
	std::map<char, std::pair<std::string, int>> mCodonCount = { {'F', {"Phenylalanine",0}},{'L',{"Leucine",0}}, {'I',{"Isoleucine",0}},{'M', {"Methionine",0}}, {'V',{"Valine",0}}, {'S',{"Serine",0}},
												{'P',{"Proline",0}}, {'T', {"Threonine",0}},{'A',{"Alanine",0}},{'Y',{"Tyrosine",0}}, {'H',{"Histidine",0}}, {'Q',{"Glutamine",0}}, {'N',{"Asparagine",0}}, {'K',{"Lysine",0}}, {'D',{"Aspartic acid",0}},
												{'E',{"Glutamic acid",0}}, {'C',{"Cysteine",0}}, {'W',{"Tryptophan",0}}, {'R',{"Arginine",0}}, {'G',{"Glycine",0}} };
	int totalAminoAcid;

	DNATranslate(std::string& sequence){
		totalAminoAcid = 0;
		char base;
		int state = 0, column = 0; //0 = T, 1 = C, 2 = A, 3 = G
		bool start = false; //true when methionine is found
		for (int i = 0; i < sequence.size(); ++i) { //loop through each base in the sequence
			base = sequence[i];
			switch (base){
				case 'T':
					column = 0;
					break;
				case 'C':
					column = 1;
					break;
				case 'A':
					column = 2;
					break;
				case 'G':
					column = 3;
					break;
				default:
					break;
			}
			state = mTable[state][column];

			if (state >= 'A') { //we've got a base
				if (!start) { //have not encountered methionine yet
					if (state == 'M') {
						start = true;
						mCodonCount[state].second += 1;
						state = 3;
						totalAminoAcid += 1;
					}
				}
				else {
					if (state == 'X') {//encountered stop codon
						state = 0;
						start = false;
					}
					else {
						mCodonCount[state].second += 1;
						state = 3;
						totalAminoAcid += 1;
					}
				}
			}
		}
		//int bruh = 5;
	}

	void printAminoCount(std::ofstream& ofile) {
		ofile << "Total amino acids produced: " << totalAminoAcid << "\n";
		
		for (auto const& x : mCodonCount) {
			ofile << "(" << x.first << ") " << x.second.first << ": " << x.second.second << "\n";
		}
	}
};