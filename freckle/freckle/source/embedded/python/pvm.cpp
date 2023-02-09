#include "pvm.h"
#include "public/python.h"

python_vm::python_vm()
{
	Py_Initialize();
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

	PyObject* the_module = PyImport_AddModule("wr");
	PyModule_AddFunctions(the_module, methods);
	PyModule_AddObject(the_module, "wr", nullptr);
	PyModule_AddObject(the_module, "wr", nullptr);

	for (const auto& text : preload)
	{
		PyRun_SimpleString(text.c_str());
	}

	return true;
}

void python_vm::tick() noexcept
{
}
