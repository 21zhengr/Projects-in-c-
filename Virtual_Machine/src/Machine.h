#pragma once
#include "Ops.h"
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "../img/bitmap_image.hpp"
#include "Exceptions.h"

class Machine
{
public:
	Machine();
	void readFile(const std::string& filename);
	//don't forget getters and setters for mFlags and mRegister
	int& getRegister(std::string registerName);
	bool& getFlag(std::string flagName);

	void setRegister(std::string key, int& value);
	void setFlag(std::string key, bool& value);


	void Execute();

	void writeRegisterAndFlags(std::ofstream& ofile);
	void writePcAndStack(std::ofstream& ofile);

	void pushStack(int& value);
	int popStack();
	int& stackIndex(int index);

	void forward(int reg1Value);

private:
	std::vector<std::shared_ptr<Op>> mOps;
	std::map<std::string, 
		std::function<std::shared_ptr<Op>()>>
		opMap;
	std::unordered_map<std::string, int> mRegister;
	std::unordered_map<std::string, bool> mFlags;
	std::vector<int> mStack;

	bitmap_image mImage;
	image_drawer mDrawer;
};

template <typename T>
std::shared_ptr<Op> CreateOp() {
	return std::make_shared<T>();
}