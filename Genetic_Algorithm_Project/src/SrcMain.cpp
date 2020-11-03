#include "SrcMain.h"
#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>

void ProcessCommandArgs(int argc, const char* argv[])
{
	//inputfile popsize generations mutationchance seed
	std::string inputFile = argv[1],
		popSize = argv[2],
		generations = argv[3],
		mutationChance = argv[4],
		seed = argv[5];

	std::mt19937 g1(std::stoi(seed));
	
	std::vector<Location> loc = getLocations(inputFile);
	Population pop = generateRandomPopulation(std::stoi(popSize), loc.size(), g1);

	//output popyulation to log.txt

	std::ofstream ofile("log.txt");
	ofile << "INITIAL POPULATION:\n";
	std::for_each(pop.mMembers.begin(), pop.mMembers.end(), [&ofile](const std::vector<int> v) {
		ofile << v[0];
		std::for_each(v.begin()+1, v.end(), [&ofile](const int i) { ofile << "," << i;});
		ofile << "\n";
		});
	
	std::vector<std::pair<int, double>> vFitness = calculateFitness(pop, loc);
	ofile << "FITNESS:\n";
	std::for_each(vFitness.begin(), vFitness.end(), [&ofile](const std::pair<int, double> p) {
		ofile << p.first << ":" << p.second << "\n";
		});
	/*
	std::vector<std::pair<int, int>> vParents = selection(vFitness, g1);
	ofile << "SELECTED PAIRS:\n";
	std::for_each(vParents.begin(), vParents.end(), [&ofile](const std::pair<int, int> p) {
		ofile << "(" << p.first << "," << p.second << ")\n";
		});
*/
	double mutChance = std::stoi(mutationChance) / 100.0;
	Population newPop;
	for (int i = 1; i <= std::stoi(generations); ++i) {
		std::vector<std::pair<int, int>> vParents = selection(vFitness, g1);
		newPop = crossOver(vParents, pop, g1, mutChance);
		vFitness = calculateFitness(newPop, loc);

		ofile << "SELECTED PAIRS:\n";
		std::for_each(vParents.begin(), vParents.end(), [&ofile](const std::pair<int, int> p) {
			ofile << "(" << p.first << "," << p.second << ")\n";
			});
		ofile << "GENERATION: " << i << "\n";
		std::for_each(newPop.mMembers.begin(), newPop.mMembers.end(), [&ofile](const std::vector<int> v) {
			ofile << v[0];
			std::for_each(v.begin() + 1, v.end(), [&ofile](const int i) { ofile << "," << i; });
			ofile << "\n";
			});
		ofile << "FITNESS:\n";
		std::for_each(vFitness.begin(), vFitness.end(), [&ofile](const std::pair<int, double> p) {
			ofile << p.first << ":" << p.second << "\n";
			});
		pop = newPop;
	}

	std::sort(vFitness.begin(), vFitness.end(),
		[](const std::pair<int, double>& a, const std::pair<int, double>& b) {
			return a.second < b.second;
		});
	ofile << "SOLUTION:\n";
	ofile << loc[newPop.mMembers[vFitness[0].first][0]].mName << "\n";
	std::for_each(newPop.mMembers[vFitness[0].first].begin() + 1, newPop.mMembers[vFitness[0].first].end(), [&ofile, &loc](const int i) {
		ofile << loc[i].mName << "\n";
		});
	ofile << loc[0].mName << "\n"
		<< "DISTANCE: " << vFitness[0].second << " miles";



}
