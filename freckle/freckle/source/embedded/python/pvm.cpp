#include "pvm.h"
#include "public/python.h"

bool python_vm::initialize() noexcept
{
	Py_Initialize();
	PyRun_SimpleString("print(\"helloworld\")");
	Py_Finalize();
	
	return true;
}

void python_vm::run() noexcept
{
	std::cout << "Python virtual machine is running." << std::endl;

	while (!_should_exit)
	{

	}
	std::cout << "Python virtual machine has exited." << std::endl;
}

void python_vm::notify_to_exit() noexcept
{
	_should_exit = true;
}
