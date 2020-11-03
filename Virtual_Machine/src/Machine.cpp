#include "Machine.h"
#include <fstream>


Machine::Machine() : mImage(320,240), mDrawer(mImage)
{
	// TODO
	//initialize opMap
	opMap.emplace("movi", &CreateOp<MovI>);
	opMap.emplace("exit", &CreateOp<Exit>);
	opMap.emplace("add", &CreateOp<Add>);
	opMap.emplace("mov", &CreateOp<Mov>);
	opMap.emplace("sub", &CreateOp<Sub>);
	opMap.emplace("mul", &CreateOp<Mul>);
	opMap.emplace("div", &CreateOp<Div>);
	opMap.emplace("inc", &CreateOp<Inc>);
	opMap.emplace("dec", &CreateOp<Dec>);
	opMap.emplace("jmp", &CreateOp<Jmp>);
	opMap.emplace("cmpeq", &CreateOp<Cmpeq>);
	opMap.emplace("cmplt", &CreateOp<Cmplt>);
	opMap.emplace("jt", &CreateOp<Jt>);
	opMap.emplace("jnt", &CreateOp<Jnt>);
	opMap.emplace("pop", &CreateOp<Pop>);
	opMap.emplace("push", &CreateOp<Push>);
	opMap.emplace("load", &CreateOp<Load>);
	opMap.emplace("store", &CreateOp<Store>);
	opMap.emplace("loadi", &CreateOp<Loadi>);
	opMap.emplace("storei", &CreateOp<Storei>);
	opMap.emplace("loadsc", &CreateOp<Loadsc>);
	opMap.emplace("storesc", &CreateOp<Storesc>);
	opMap.emplace("pendown", &CreateOp<Pendown>);
	opMap.emplace("penup", &CreateOp<Penup>);
	opMap.emplace("fwd", &CreateOp<Fwd>);
	opMap.emplace("back", &CreateOp<Back>);
	//initialize registers
	mRegister.emplace("r0", 0);
	mRegister.emplace("r1", 0);
	mRegister.emplace("r2", 0);
	mRegister.emplace("r3", 0);
	mRegister.emplace("r4", 0);
	mRegister.emplace("r5", 0);
	mRegister.emplace("r6", 0);
	mRegister.emplace("r7", 0);
	mRegister.emplace("pc", 0);
	mRegister.emplace("sc", 0);
	mRegister.emplace("ex", 0);
	mRegister.emplace("tx", 0);
	mRegister.emplace("ty", 0);
	mRegister.emplace("tr", 0);
	mRegister.emplace("tc", 0);

	//initialize mFlags
	mFlags.emplace("exit", false);
	mFlags.emplace("test", false);
	mFlags.emplace("pen", false);

	//initalize image and image_drawer
	mImage.set_all_channels(0,0,0);
	mDrawer.pen_width(1);
	mDrawer.pen_color(255, 255, 255);
}

void Machine::readFile(const std::string& fileName) {
	std::ifstream iFile(fileName);
	std::string line;

	while (std::getline(iFile, line)) {
		std::string opName = line.substr(0, line.find(" "));
		std::string params = (line != "exit") ? line.substr(line.find(" ") + 1) : "";

		std::shared_ptr<Op> ptr = opMap.at(opName)();
		ptr->Parse(params);

		mOps.emplace_back(ptr);
	}
}

void Machine::writeRegisterAndFlags(std::ofstream& ofile) {
	ofile << "********************\n";
	/*for (auto const& r : mRegister) {
		ofile << r.first << "=" << r.second << "\n";
	}*/

	ofile << "r0=" << mRegister["r0"] << "\n";
	ofile << "r1=" << mRegister["r1"] << "\n";
	ofile << "r2=" << mRegister["r2"] << "\n";
	ofile << "r3=" << mRegister["r3"] << "\n";
	ofile << "r4=" << mRegister["r4"] << "\n";
	ofile << "r5=" << mRegister["r5"] << "\n";
	ofile << "r6=" << mRegister["r6"] << "\n";
	ofile << "r7=" << mRegister["r7"] << "\n";
	ofile << "pc=" << mRegister["pc"] << "\n";
	ofile << "sc=" << mRegister["sc"] << "\n";
	ofile << "ex=" << mRegister["ex"] << "\n";
	ofile << "tx=" << mRegister["tx"] << "\n";
	ofile << "ty=" << mRegister["ty"] << "\n";
	ofile << "tr=" << mRegister["tr"] << "\n";
	ofile << "tc=" << mRegister["tc"] << "\n";

	//flags
	ofile << "exit" << "=" << mFlags["exit"] << "\n";
	ofile << "test" << "=" << mFlags["test"] << "\n";
	ofile << "pen" << "=" << mFlags["pen"] << "\n";
}

void Machine::writePcAndStack(std::ofstream& ofile) {
	ofile << "pc: " << mRegister["pc"] << " Stack: ";
	for (auto const& v : mStack) {
		ofile << v << " ";
	}
	ofile << "\n";
}

void Machine::Execute() {
	/*1. Get the current value of the “pc” register
	2. Get the pointer to the op in the mOps index specified by #1
	3. Increment the “pc” register
	4. Execute the op from #2
	5. Repeat as long as the “exit” flag is false*/
	std::ofstream logFile("log.txt"), 
				stackFile("stack.txt");

	writeRegisterAndFlags(logFile);
	writePcAndStack(stackFile);
	while (!mFlags["exit"]) {
		int currValue = mRegister["pc"];
		std::shared_ptr<Op> op = mOps[currValue];
		logFile << "Executing: " << op->GetName() << "\n"; //log to file
		mRegister["pc"] += 1;
		try {
			op->Execute(*this);
		}
		catch (NonFatalException e) {
			mRegister["ex"] = e.code;
		}
		catch (FatalException e) {
			mRegister["ex"] = e.code;
			mFlags["exit"] = true;
		}
		
		writeRegisterAndFlags(logFile);
		writePcAndStack(stackFile);
	}
	mImage.save_image("output.bmp");
}

int& Machine::getRegister(std::string registerName) {
	//maybe implement try catch here

	return mRegister.at(registerName);
}

bool& Machine::getFlag(std::string flagName) {
	//maybe implement try catch here

	return mFlags.at(flagName);
}

void Machine::setRegister(std::string key, int& value) {
	if (key == "sc" || key == "r0") {
		int code = (key == "sc") ? 10 : 11;
		throw NonFatalException(code);
	}
	if (key == "tc" && (value < 0 || value > 7)) {
		throw NonFatalException(14);
	}
	mRegister.at(key) = value;
}
void Machine::setFlag(std::string key, bool& value) {
	//try catch maybe
	mFlags.at(key) = value;
}

void Machine::pushStack(int& value) {
	if (mStack.size() >= 256) {
		throw FatalException(101);
	}
	mStack.push_back(value);
	mRegister["sc"] += 1;
}
int Machine::popStack() {
	if (mStack.empty()) {
		throw FatalException(103);
	}
	int value = mStack.back();
	mStack.pop_back();
	mRegister["sc"] -= 1;
	return value;
}
int& Machine::stackIndex(int index) {
	if (index < 0 || index >= mRegister["sc"]) {
		throw FatalException(100);
	}
	return mStack[index];
}

void Machine::forward(int reg1Value) {
	int tx = mRegister["tx"],
		ty = mRegister["ty"],
		tr = mRegister["tr"];

	float angle = tr * 0.0174533f;

	int endx = tx + static_cast<int>(std::cos(angle) * reg1Value),
		endy = ty + static_cast<int>(std::sin(angle) * reg1Value);
	if (mFlags["pen"]) { //if pen is true then draw a line
		switch (mRegister["tc"])
		{
		case 0:
			mDrawer.pen_color(255, 255, 255);
			break;
		case 1:
			mDrawer.pen_color(255, 255, 0);
			break;
		case 2:
			mDrawer.pen_color(255, 0, 255);
			break;
		case 3:
			mDrawer.pen_color(255, 0, 0);
			break;
		case 4:
			mDrawer.pen_color(0, 255, 255);
			break;
		case 5:
			mDrawer.pen_color(0, 255, 0);
			break;
		case 6:
			mDrawer.pen_color(0, 0, 255);
			break;
		case 7:
			mDrawer.pen_color(0, 0, 0);
			break;
		default:
			break;
		}
		mDrawer.line_segment(tx, ty, endx, endy);
	}
	//update values
	mRegister["tx"] = endx;
	mRegister["ty"] = endy;
}
