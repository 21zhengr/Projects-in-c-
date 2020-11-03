#include "SrcMain.h"
#include <iostream>
//#include "Fasta.hpp"
#include "DNATranslate.hpp"
#include "NWCompare.hpp"
#include <string>

void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO
	if (argc == 2) {
		Fasta f(argv[1]);
		DNATranslate d(f.mSequence);
		//now output
		std::ofstream ofile("amino.txt");
		ofile << f.mHeader << "\n";
		d.printAminoCount(ofile);
	}
	else {
		Fasta f(argv[1]);
		Fasta w(argv[2]);
		NWCompare g(f, w);
		std::ofstream ofile("match.result");
		g.outputComparison(ofile);
	}
	

}
