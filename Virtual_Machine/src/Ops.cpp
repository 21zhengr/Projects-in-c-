#include "Ops.h"
#include "Machine.h"

void Exit::Execute(Machine& machine)
{
	std::string exit = "exit";
	bool value = true;
	machine.setFlag(exit, value);
}

void MovI::Execute(Machine& machine)
{
	std::string regName = std::get<0>(mParameters);
	int value = std::get<1>(mParameters);
	machine.setRegister(regName, value);
}

void Add::Execute(Machine& machine)
{
	int reg2 = machine.getRegister(std::get<1>(mParameters)),
		reg3 = machine.getRegister(std::get<2>(mParameters));

	int sum = reg2 + reg3;
	machine.setRegister(std::get<0>(mParameters), sum);


	int64_t test2 = static_cast<int64_t>(reg2),
		test3 = static_cast<int64_t>(reg3);
	int64_t testResult = test2 + test3;
	if (testResult > INT32_MAX || testResult < INT32_MIN) {
		//operation will overflow
		int code = (testResult > INT32_MAX) ? 12 : 13;
		throw NonFatalException(code);
	}
}

void Mov::Execute(Machine& machine)
{
	std::string reg1 = std::get<0>(mParameters),
		reg2 = std::get<1>(mParameters);

	int reg2Value = machine.getRegister(reg2);

	machine.setRegister(reg1, reg2Value);
}

void Sub::Execute(Machine& machine)
{
	int reg2 = machine.getRegister(std::get<1>(mParameters)),
		reg3 = machine.getRegister(std::get<2>(mParameters));
	int diff = reg2 - reg3;
	machine.setRegister(std::get<0>(mParameters), diff);

	int64_t test2 = static_cast<int64_t>(reg2),
		test3 = static_cast<int64_t>(reg3);
	int64_t testResult = test2 - test3;
	if (testResult > INT32_MAX || testResult < INT32_MIN) {
		//operation will overflow
		int code = (testResult > INT32_MAX) ? 12 : 13;
		throw NonFatalException(code);
	}
}

void Mul::Execute(Machine& machine)
{
	int reg2 = machine.getRegister(std::get<1>(mParameters)),
		reg3 = machine.getRegister(std::get<2>(mParameters));
	int product = reg2 * reg3;
	machine.setRegister(std::get<0>(mParameters), product);

	int64_t test2 = static_cast<int64_t>(reg2),
		test3 = static_cast<int64_t>(reg3);
	int64_t testResult = test2 * test3;
	if (testResult > INT32_MAX || testResult < INT32_MIN) {
		//operation will overflow
		int code = (testResult > INT32_MAX) ? 12 : 13;
		throw NonFatalException(code);
	}
}

void Div::Execute(Machine& machine)
{
	int reg2 = machine.getRegister(std::get<1>(mParameters)),
		reg3 = machine.getRegister(std::get<2>(mParameters));
	if (reg3 == 0) {
		throw FatalException(102);
	}
	int result = reg2 / reg3;
	machine.setRegister(std::get<0>(mParameters), result);

	int64_t test2 = static_cast<int64_t>(reg2),
		test3 = static_cast<int64_t>(reg3);
	int64_t testResult = test2 / test3;
	if (testResult > INT32_MAX || testResult < INT32_MIN) {
		//operation will overflow
		int code = (testResult > INT32_MAX) ? 12 : 13;
		throw NonFatalException(code);
	}
}

void Inc::Execute(Machine& machine)
{
	std::string reg1 = std::get<0>(mParameters);

	int reg1Value = machine.getRegister(reg1);
	int64_t test = static_cast<int64_t>(reg1Value);
	reg1Value += 1;

	machine.setRegister(reg1, reg1Value);


	test += 1;
	if (test > INT32_MAX ) {
		//operation will overflow
		throw NonFatalException(12);
	}
}

void Dec::Execute(Machine& machine)
{
	std::string reg1 = std::get<0>(mParameters);

	int reg1Value = machine.getRegister(reg1);
	int64_t test = static_cast<int64_t>(reg1Value);
	reg1Value -= 1;

	machine.setRegister(reg1, reg1Value);
	
	test -= 1;
	if (test < INT32_MIN) {
		//operation will overflow
		throw NonFatalException(13);
	}
}

void Jmp::Execute(Machine& machine)
{
	std::string reg1 = std::get<0>(mParameters);

	int reg1Value = machine.getRegister(reg1);

	machine.setRegister(std::string("pc"), reg1Value);
}

void Cmpeq::Execute(Machine& machine)
{
	/*std::string reg1 = std::get<0>(mParameters),
				reg2 = std::get<1>(mParameters);*/

	int reg1Value = machine.getRegister(std::get<0>(mParameters)),
		reg2Value = machine.getRegister(std::get<1>(mParameters));
	bool result = (reg1Value == reg2Value);
	machine.setFlag(std::string("test"), result);
}

void Cmplt::Execute(Machine& machine)
{
	int reg1Value = machine.getRegister(std::get<0>(mParameters)),
		reg2Value = machine.getRegister(std::get<1>(mParameters));
	bool result = (reg1Value < reg2Value);
	machine.setFlag(std::string("test"), result);
}
void Jt::Execute(Machine& machine)
{	
	bool testValue = machine.getFlag(std::string("test"));

	if (testValue) {
		int reg1Value = machine.getRegister(std::get<0>(mParameters));
		
		machine.setRegister(std::string("pc"), reg1Value);
	}
	
}

void Jnt::Execute(Machine& machine)
{
	bool testValue = machine.getFlag(std::string("test"));

	if (!testValue) {
		int reg1Value = machine.getRegister(std::get<0>(mParameters));

		machine.setRegister(std::string("pc"), reg1Value);
	}
}

void Push::Execute(Machine& machine)
{
	//int reg1Value = machine.getRegister(std::string("r1"));
	machine.pushStack(machine.getRegister(std::get<0>(mParameters))); //push onto stack
	
	//int scValue = machine.getRegister(std::string("sc"));
	//scValue += 1;
	//machine.setRegister(std::string("sc"), scValue);
}

void Pop::Execute(Machine& machine)
{
	int stackValue = machine.popStack();
	machine.setRegister(std::get<0>(mParameters), stackValue);

	//int scValue = machine.getRegister(std::string("sc"));
	//scValue -= 1;
	//machine.setRegister(std::string("sc"), scValue);
}

void Load::Execute(Machine& machine)
{
	int reg2Value = machine.getRegister(std::get<1>(mParameters));
	machine.setRegister(std::get<0>(mParameters), machine.stackIndex(reg2Value));
}

void Store::Execute(Machine& machine)
{
	int reg1Value = machine.getRegister(std::get<0>(mParameters));
	machine.stackIndex(reg1Value) = machine.getRegister(std::get<1>(mParameters));
}

void Loadi::Execute(Machine& machine)
{
	int value = std::get<1>(mParameters);
	machine.setRegister(std::get<0>(mParameters), machine.stackIndex(value));
}

void Storei::Execute(Machine& machine)
{
	int value = std::get<0>(mParameters);
	machine.stackIndex(value) = machine.getRegister(std::get<1>(mParameters));
}

void Loadsc::Execute(Machine& machine)
{
	int reg2Value = machine.getRegister(std::get<1>(mParameters)),
		scValue = machine.getRegister(std::string("sc"));

	machine.setRegister(std::get<0>(mParameters), machine.stackIndex(scValue - reg2Value -1));
}

void Storesc::Execute(Machine& machine)
{
	int reg1Value = machine.getRegister(std::get<0>(mParameters)),
		scValue = machine.getRegister(std::string("sc"));

	machine.stackIndex(scValue - reg1Value - 1) = machine.getRegister(std::get<1>(mParameters));
}

void Penup::Execute(Machine& machine)
{	
	bool value = false;
	machine.setFlag(std::string("pen"), value);
}

void Pendown::Execute(Machine& machine)
{
	bool value = true;
	machine.setFlag(std::string("pen"), value);
}

void Fwd::Execute(Machine& machine)
{
	int regValue = machine.getRegister(std::get<0>(mParameters));
	machine.forward(regValue);
}

void Back::Execute(Machine& machine)
{
	int regValue = machine.getRegister(std::get<0>(mParameters));
	machine.forward(regValue * -1);
}