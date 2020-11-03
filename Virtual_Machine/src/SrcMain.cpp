#include "SrcMain.h"
#include <iostream>
#include "Machine.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	//Next, in SrcMain.cpp, create an instance of Machine 
	//and call your member function, passing in the argv[1] string.
	Machine m = Machine();
	m.readFile(argv[1]);
	m.Execute();
}
