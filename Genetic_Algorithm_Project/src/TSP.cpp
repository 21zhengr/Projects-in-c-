#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>

std::vector<Location> getLocations(const std::string& fileName) {
	std::vector<Location> locations;
	std::ifstream ifile(fileName);
	std::string line;

	while (std::getline(ifile, line)) {

		std::vector<std::string> info = SplitSanjay(line);
		locations.emplace_back(Location{ info[0], std::stod(info[1]), std::stod(info[2]) });
	}

	return locations;
}

std::vector<std::string> SplitSanjay(const std::string& str) {
	const char delim = ',';
		std::vector<std::string> retVal;
	size_t start = 0;
	size_t delimLoc = str.find_first_of(delim, start);
	while (delimLoc != std::string::npos) {
		retVal.emplace_back(str.substr(start, delimLoc - start));
		start = delimLoc + 1;
		delimLoc = str.find_first_of(delim, start);
	}
	retVal.emplace_back(str.substr(start));
	return retVal;
}

Population generateRandomPopulation(const int& popSize, const size_t& numLocations, std::mt19937& generator) {//random generator passed by reference
	//Population p = { std::vector<std::vector<int>>(popSize) };
	std::vector<std::vector<int>> population(popSize);

	int size = static_cast<int>(numLocations);
	std::vector<int> initialVector(size);
	std::iota(initialVector.begin(), initialVector.end(), 0);

	std::generate(population.begin(), 
					population.end(), 
		[&generator, initialVector]() {
			std::vector<int> shuffledVector(initialVector);//so that its shuffled independently

			std::shuffle(shuffledVector.begin()+1, shuffledVector.end(), generator);
			return shuffledVector;
		});

	return Population{ population };
}

double haversineDistance(Location& loc1, Location& loc2) {
	double lon1 = 0.0174533 * loc1.mLongitude,
		lon2 = 0.0174533 * loc2.mLongitude,
		lat1 = 0.0174533 * loc1.mLatitude,
		lat2 = 0.0174533 * loc2.mLatitude;

	double dlon = lon2 - lon1;
	double dlat = lat2 - lat1;

	double a = (pow(sin(dlat / 2.0), 2.0))
		+ cos(lat1) * cos(lat2)
		* (pow(sin(dlon / 2.0), 2.0));
	double c = 2.0 * atan2(sqrt(a), sqrt(1 - a));

	return 3961.0 * c;
}

std::vector<std::pair<int, double>> calculateFitness(Population& pop, std::vector<Location>& loc) {
	std::vector<std::pair<int, double>> vPairs(pop.mMembers.size());
	std::vector<double> vdoubles;
	int idx = 0;
	std::generate(vPairs.begin(), vPairs.end(), 
		[&pop, &idx, &vdoubles, &loc]() {
			std::adjacent_difference(pop.mMembers[idx].begin(), pop.mMembers[idx].end(), std::back_inserter(vdoubles), 
				[&loc](int a, int b) { 
					return haversineDistance(loc[a], loc[b]); 
				});

			double returnTrip = haversineDistance(loc[pop.mMembers[idx].back()], loc[pop.mMembers[idx].front()]); //for return trip
			double fitness = std::accumulate(vdoubles.begin(), vdoubles.end(), returnTrip, 
								[](const double& a, const double& b) { 
									return a + b;
								});
			std::pair<int, double> p{ idx, fitness };
			idx += 1; //to move the index along
			vdoubles.clear(); //to reset the vpairs for next iteration
			return p;
		});
	return vPairs;
}

std::vector<std::pair<int, int>> selection(std::vector<std::pair<int, double>>& vFitness, std::mt19937& generator) {
	std::sort(vFitness.begin(), vFitness.end(),
		[](const std::pair<int, double>& a, const std::pair<int, double>& b) {
			return a.second < b.second;
		});

	int popSize = vFitness.size();
	std::vector<double> vProbability(popSize);

	std::generate(vProbability.begin(), vProbability.end(), [&popSize]() { return 1.0 / popSize; });
	//multiple probability of first two fittest individuals by 6.0
	vProbability[vFitness[0].first] *= 6.0;
	vProbability[vFitness[1].first] *= 6.0;

	int topHalfIndex = (popSize / 2) - 1;

	std::for_each(vFitness.begin() + 2, vFitness.begin() + topHalfIndex + 1, //allowed to use for loop as stated in writeup
		[&vProbability](const std::pair<int, double>& a) {
			vProbability[a.first] *= 3.0;
		});

	double sum = std::accumulate(vProbability.begin(), vProbability.end(), 0.0,
		[](const double& a, const double& b) {
			return a + b;
		});
	std::vector<double> vProbabilityNormalized;

	std::transform(vProbability.begin(), vProbability.end(), std::back_inserter(vProbabilityNormalized),
		[&sum](const double& a) {
			return a / sum;
		});

	//picking random parents
	std::vector<std::pair<int, int>> selectedParents(popSize);
	std::generate(selectedParents.begin(), selectedParents.end(),
		[&generator, &vProbabilityNormalized]() {
			std::uniform_real_distribution<double> urd(0.0, 1.0);
			double rand = urd(generator);
			double runningSum = 0.0;
			int i = 0;
			for (double d : vProbabilityNormalized) {
				runningSum += d;
				if (runningSum >= rand) { break; }
				i += 1;
			}
			//generate random for second parent
			rand = urd(generator);
			runningSum = 0;
			int j = 0;
			for (double d : vProbabilityNormalized) {
				runningSum += d;
				if (runningSum >= rand) {
					break;
				}
					
				j += 1;
			}

			return std::pair<int, int>{i, j};
		});
	
	return selectedParents;
}


Population crossOver(std::vector<std::pair<int, int>>& vParents, Population& pop, std::mt19937& generator, double& mutationChance) {
	std::vector<std::vector<int>> newPop;
			
	std::transform(vParents.begin(), vParents.end(), std::back_inserter(newPop),
		[&pop, &generator, &mutationChance](const std::pair<int, int>& p) {
			std::uniform_int_distribution<int> uid(1,(pop.mMembers[0].size() - 2));
			int crossoverIndex = uid(generator);

			std::uniform_int_distribution<int> uid2(0, 1);
			int isA = uid2(generator);

			std::vector<int> child((crossoverIndex + 1));
			std::vector<int> first, second;
			if (isA > 0) {
				first = pop.mMembers[p.first];
				second = pop.mMembers[p.second];
			}
			else {
				first = pop.mMembers[p.second];
				second = pop.mMembers[p.first];
			}
			
			std::copy_n(first.begin(), crossoverIndex + 1, child.begin()); //including crossover index
			std::copy_if(second.begin(), second.end(), std::back_inserter(child),
				[&child](const int& i) {
					auto found = std::find(child.begin(), child.end(), i);
					return (found == child.end());
				});

			std::uniform_real_distribution<double> urd(0.0, 1.0);
			double mutChance = urd(generator);
			if (mutChance <= mutationChance) {
				std::uniform_int_distribution<int> uid3(1, (child.size()-1));
				int firstIdx = uid3(generator);
				int secondIdx = uid3(generator);
				std::swap(child[firstIdx], child[secondIdx]);
			}

			return child;
		});
			
	return Population{ newPop };

}