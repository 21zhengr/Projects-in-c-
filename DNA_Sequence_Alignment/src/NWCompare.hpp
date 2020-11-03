#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "Fasta.hpp"

enum direction : char {aboveLeft = 0,above = 1,left = 2};

class NWCompare {
public:
	Fasta a, b;
	std::vector<std::vector<short>> mScore;
	std::vector<std::vector<char>> mDirection;
	short match = 1, gap = -1, mismatch = -1;
	std::string mSequenceA, mSequenceB, mPrettyPrint;

	NWCompare(Fasta A, Fasta B):a(A), b(B) {
		mScore = std::vector<std::vector<short>> (b.mSequence.size() + 1, std::vector<short>(a.mSequence.size() + 1));
		mDirection = std::vector<std::vector<char>>(b.mSequence.size() + 1, std::vector<char>(a.mSequence.size() + 1));
		//initialize the mScore arary
		mScore[0][0] = 0;

		//direction d = left;
		for (int i = 1; i < a.mSequence.size() + 1; ++i) {
			mScore[0][i] = -1 * i;
			mDirection[0][i] = left;
		}

		//d = above;
		for (int i = 1; i < b.mSequence.size() + 1; ++i) {
			mScore[i][0] = -1 * i;
			mDirection[i][0] = above;
		}

		mSequenceA.reserve(a.mSequence.size());
		mSequenceB.reserve(b.mSequence.size());
		mPrettyPrint.reserve(b.mSequence.size());

		compare(); //debug
	}

	void outputComparison(std::ofstream& ofile) {
		ofile << "A: " << a.mHeader << "\n" << "B: " << b.mHeader << "\n";
		ofile << "Score: " << mScore[b.mSequence.size()][a.mSequence.size()] << "\n\n";

		int count = mSequenceA.size() / 70;
		
		int tracking = 0;

		for (int i = 0; i < count; ++i) {
			for (int j = tracking; j < (tracking + 70); ++j) {
				ofile << mSequenceA[j];
			}
			ofile << "\n";

			for (int j = tracking; j < (tracking + 70); ++j) {
				ofile << mPrettyPrint[j];
			}
			ofile << "\n";

			for (int j = tracking; j < (tracking + 70); ++j) {
				ofile << mSequenceB[j];
			}
			ofile << "\n\n";

			tracking += 70;
		}

		for (int i = tracking; i < mSequenceA.size(); ++i) {
			ofile <<  mSequenceA[i];
		}
		ofile << "\n";
		for (int i = tracking; i < mSequenceA.size(); ++i) {
			ofile << mPrettyPrint[i];
		}
		ofile << "\n";
		for (int i = tracking; i < mSequenceA.size(); ++i) {
			ofile << mSequenceB[i];
		}
		ofile << "\n\n";
		
		

	}

	void compare() {
		for (int i = 1; i < b.mSequence.size() + 1; ++i) { //perhaps mScore.size() and mScore[i].size()
			for (int j = 1; j < a.mSequence.size() + 1; ++j) {
				//score of neighbor cell to the above left + match or mimatch score
				short aboveLeftScore = (a.mSequence[j -1] == b.mSequence[i -1]) ? match : mismatch;
				aboveLeftScore += mScore[i - 1][j - 1];

				short aboveScore = mScore[i - 1][j] + gap,
					leftScore = mScore[i][j - 1] + gap;
				//case where all of them are equal
				if(aboveLeftScore == aboveScore && aboveScore == leftScore){
					mDirection[i][j] = aboveLeft;
					mScore[i][j] = aboveLeftScore;
				}
				else {
					short maxScore = std::max({ aboveLeftScore, aboveScore, leftScore });
					mScore[i][j] = maxScore;

					if (((maxScore == aboveLeftScore) && (maxScore == aboveScore)) ||
						((maxScore == aboveLeftScore) && (maxScore == leftScore))) {
						mDirection[i][j] = aboveLeft;
					}
					else if ((maxScore == aboveScore) && (maxScore == leftScore)) {
						mDirection[i][j] = left;
					}
					else { //no ties
						if (maxScore == aboveLeftScore) {
							mDirection[i][j] = aboveLeft;
						}
						else if (maxScore == aboveScore) {
							mDirection[i][j] = above;
						}
						else if (maxScore == leftScore) {
							mDirection[i][j] = left;
						}
					}
				}

			}
		}
		//creating the resulting sequence strings
		int i = b.mSequence.size(),
			j = a.mSequence.size();

		while ((i > 0) || (j > 0)) {
			if (mDirection[i][j] == aboveLeft) { //either mismatch or a match
				mSequenceA += a.mSequence[j - 1];
				mSequenceB += b.mSequence[i - 1];
				mPrettyPrint += (a.mSequence[j - 1] == b.mSequence[i - 1]) ? "|": " ";
				i -= 1;
				j -= 1;
			}
			else if (mDirection[i][j] == above) { //write letter in sequence B and blank in sequence A
				mSequenceA += "_";
				mSequenceB += b.mSequence[i - 1];
				mPrettyPrint += " ";
				i -= 1;
			}
			else if (mDirection[i][j] == left) { //write letter in sequence A and blank in sequence B
				mSequenceA += a.mSequence[j - 1];
				mSequenceB += "_";
				mPrettyPrint += " ";
				j -= 1;
			}
		}

		std::reverse(mSequenceA.begin(), mSequenceA.end());
		std::reverse(mSequenceB.begin(), mSequenceB.end());
		std::reverse(mPrettyPrint.begin(), mPrettyPrint.end());

		//int bruhbruhbruh = mScore[b.mSequence.size()][a.mSequence.size()];
		//int debug = 420;
	}
	
};