#pragma once
#include <string>
#include <vector>
#include <random>

struct Location
{
	std::string mName;
	double mLatitude;
	double mLongitude;
};

struct Population
{
	std::vector<std::vector<int>> mMembers;
};

std::vector<Location> getLocations(const std::string& fileName);
std::vector<std::string> SplitSanjay(const std::string& str); //from lecture slides
Population generateRandomPopulation(const int& popSize, const size_t& numLocations, std::mt19937& generator);
double haversineDistance(Location& loc1, Location& loc2);
std::vector<std::pair<int, double>> calculateFitness(Population& pop, std::vector<Location>& loc);
std::vector<std::pair<int, int>> selection(std::vector<std::pair<int, double>>& vFitness, std::mt19937& generator);
Population crossOver(std::vector<std::pair<int, int>>& vParents, Population& pop, std::mt19937& generator, double& mutationChance);