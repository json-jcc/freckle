#include "lvm.h"

std::vector<lua_exporter*> lua_vm::type_exporters_ = {};
std::vector<lua_lib*> lua_vm::libs_ = {};

lua_vm::lua_vm()
{
	state_ = luaL_newstate();
}

lua_vm::~lua_vm()
{
	if(!state_)
	{
		lua_close(state_);
	}
}

bool lua_vm::init() noexcept
{
	if (!state_)
	{
		return false;
	}
	
	load_modules();
	
	for(const auto& preload_file : preload_files)
	{
		do_file(preload_file);
	}
	
	return true;
}

void lua_vm::tick() noexcept
{
	
}

bool lua_vm::do_file(const std::string& path)
{
	const int ret = luaL_dofile(state_, path.c_str());
	
	switch (ret)
	{
	case LUA_OK:
		{
			return true;
		}
	case LUA_ERRRUN:
		{
			std::cerr << "a runtime error." << std::endl;
		}
		break;
	case LUA_ERRMEM:
		{
			std::cerr << "memory allocation error.For such errors, Lua does not call the message handler." << std::endl;
		}
		break;
	case LUA_ERRERR:
		{
			std::cerr << "error while running the message handler." << std::endl;
		}
		break;
	case LUA_ERRSYNTAX: std::cerr << "syntax error during precompilation." << std::endl; break;
	case LUA_YIELD: std::cerr << "the thread(coroutine) yields." << std::endl; break;
	case LUA_ERRFILE: std::cerr << "a file - related error; e.g., it cannot open or read the file." << std::endl; break;

	default:break;
	}

	lua_Debug debug;
	lua_getstack(state_, 1, &debug);
	
	if(const auto err_msg = lua_api::to<std::string_view>(state_, -1))
	{
		std::cerr << err_msg.value() <<std::endl;
	}

	lua_pop(state_, 1);
	
	return false;
}

void lua_vm::load_class(const std::string& class_name, const luaL_Reg* funcs)
{
	luaL_getmetatable(state_, class_name.c_str());
	if (lua_isnil(state_, -1))
	{
		luaL_newmetatable(state_, class_name.c_str());
		luaL_setfuncs(state_, funcs, 0);
	}
	lua_setglobal(state_, class_name.c_str());
}

static int index_event(lua_State* l)
{
	if (!lua_isuserdata(l, 1))
	{
		lua_error(l);
		return 0;
	}

	if (lua_getmetatable(l, 1))
	{
		lua_pushvalue(l, 2);
		lua_rawget(l, -2);
		if (lua_isfunction(l, -1))
		{
			return 1;
		}
	}

	return 0;
}

static int new_index_event(lua_State* l)
{
	return 0;
}

void lua_vm::load_modules()
{
	luaL_openlibs(state_);

	for(const auto lib : libs_)
	{
		if(!lib)
		{
			continue;
		}
		
		lib->lib_funcs->emplace_back(nullptr, nullptr);

		luaL_requiref(state_, lib->name.c_str(), lib->open_func, 0);

		lua_pop(state_, 1);
	}

	std::vector<luaL_Reg> raw_method_regs;
	
	for (const auto exporter : type_exporters_)
	{
		const auto& name_to_raw_method = exporter->funcs;

		// check required custom methods
		if (!name_to_raw_method.contains("__gc"))
		{
			std::cerr << "type : " << exporter->type_name << " has no __gc func" << std::endl;
			continue;
		}

		raw_method_regs.clear();
		// collect custom methods
		for (const auto& [name, raw_method] : name_to_raw_method)
		{
			raw_method_regs.emplace_back(name.c_str(), raw_method);
		}
		// universal methods
		raw_method_regs.emplace_back("__index", index_event);
		raw_method_regs.emplace_back("__newindex", new_index_event);
		raw_method_regs.emplace_back("__call", nullptr);
		raw_method_regs.emplace_back("__gc", nullptr);
		raw_method_regs.emplace_back("__close", nullptr);
		raw_method_regs.emplace_back("__name", nullptr);
		raw_method_regs.emplace_back("__mode", nullptr);
		raw_method_regs.emplace_back("__pairs", nullptr);
		// note: from lua, reg end tag
		raw_method_regs.emplace_back(nullptr, nullptr);
		
		load_class(exporter->type_name, raw_method_regs.data());
	}
}

void lua_vm::register_type_exporter(lua_exporter* exporter)
{
	type_exporters_.push_back(exporter);
}

void lua_vm::register_lib(lua_lib* lib)
{
	libs_.push_back(lib);
}
