#include "pvm.h"

bool python_api::python_vm::initialize() noexcept
{
	return true;
}

void python_api::python_vm::run() noexcept
{
	std::cout << "Python virtual machine is running." << std::endl;

	while (!_should_exit)
	{

	}
	std::cout << "Python virtual machine has exited." << std::endl;
}

void python_api::python_vm::notify_to_exit() noexcept
{
	_should_exit = true;
}
