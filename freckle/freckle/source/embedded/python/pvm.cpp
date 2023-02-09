#include "pvm.h"
#include "public/platforms.h"
#include "public/python.h"

python_vm::python_vm()
{
    PyConfig config;
	
	std::string home;

    PyConfig_InitPythonConfig(&config);
    config.isolated = 1;
	//config.home = home.c_str();

    Py_InitializeFromConfig(&config);
}

python_vm::~python_vm()
{
	Py_Finalize();
}

PyObject* Func(PyObject* self, PyObject* args)
{
	int arg;

	PyArg_ParseTuple(self, "i", &arg);

	//int ret = DoFun(arg);

	return Py_BuildValue("i", 1);
}

PyMethodDef methods[] =
{
	{}
};

bool python_vm::initialize() noexcept
{

	for (const auto& text : preload)
	{
		PyRun_SimpleString(text.c_str());
	}

	return true;
}

void python_vm::tick() noexcept
{
}
