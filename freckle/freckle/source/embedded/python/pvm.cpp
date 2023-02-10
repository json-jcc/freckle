#include "pvm.h"
#include "public/platforms.h"
#include "public/python.h"

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

python_vm::python_vm()
{
	PyConfig config;
	PyConfig_InitPythonConfig(&config);
	
	//config.isolated = 1;
	
	std::wstring home = platforms::string_to_wstring(platforms::get_root_path())
		+ L"dependencies\\py\\";

	config.home = home.data();

	std::wstring path = platforms::string_to_wstring(platforms::get_root_path());
	
	config.module_search_paths;
	config.module_search_paths_set;

    Py_InitializeFromConfig(&config);
}

python_vm::~python_vm()
{
	Py_Finalize();
}

bool python_vm::initialize() noexcept
{
	for (const auto& text : preload)
	{
		PyRun_SimpleString(text.c_str());
	}

	const std::string main_path = platforms::get_root_path() + "";

	return true;
}

void python_vm::tick() noexcept
{
}
