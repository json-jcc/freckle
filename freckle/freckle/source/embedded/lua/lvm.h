#pragma once

#include "lua_api.h"

struct lua_cpp_type
{
    std::string name;
    const luaL_Reg* funcs;
};

struct lua_exporter;
struct lua_lib;

class lua_vm final
{

    static std::vector<lua_exporter*> type_exporters_;

    static std::vector<lua_lib*> libs_;

public:

    static void register_type_exporter(lua_exporter* exporter);

    static void register_lib(lua_lib* lib);

private:
    
    lua_State* state_ = nullptr;
    
public:

    lua_vm();
		
    lua_vm(const lua_vm&) = delete;
    lua_vm& operator=(const lua_vm&) = delete;

    lua_vm(lua_vm&&) = delete;
	
    ~lua_vm();

private:

    void load_class(const std::string& class_name, const luaL_Reg* funcs);
	
    void load_modules();

public:

    std::vector<std::string> preload_files;
    
    bool init() noexcept;
	
    void tick() noexcept;
	
    bool do_file(const std::string& path);
    
};

struct function_registry
{
    function_registry()
    {
        functions.push_back({nullptr, nullptr});
    }

    void set(const char* name, lua_CFunction f)
    {
        const auto it = std::ranges::find_if(functions,
            [name](const luaL_Reg& v)
        {
            return v.name == name;
        });

        if(it == functions.end())
        {
            functions.push_back({name, f});
        }else
        {
            it->func = f;
        }
    }

    [[nodiscard]] const std::vector<luaL_Reg>& list() const
    {
        return functions;
    }

private:
    std::vector<luaL_Reg> functions;
};

struct lua_exporter
{
    lua_exporter(std::string name) : type_name(std::move(name))
    {
        lua_vm::register_type_exporter(this);
    }

    std::unordered_map<std::string, lua_CFunction> funcs;

    std::string type_name;
};


namespace lua_api
{
    struct cpp_type_exporter
    {
        std::string_view type_name;

        std::unordered_map<std::string_view, lua_CFunction> methods;
    };

    struct lib_exporter
    {
        
    };
    
}





struct lua_lib
{
    std::string name;
    std::vector<luaL_Reg>* lib_funcs;
    lua_CFunction open_func;

    lua_lib(const std::string& lib_name, std::vector<luaL_Reg>* funcs, lua_CFunction lib_open_func) :
        name(lib_name),
        lib_funcs(funcs),
        open_func(lib_open_func)
    {
        lua_vm::register_lib(this);
    }
};

#define LUA_LIBS_B()\
static const luaL_Reg libs[] =\
{\

#define LUA_LIBS_E()\
{nullptr}, {nullptr}\
};\


#define LUA_FUNCS_B() \
static const luaL_Reg functions[] =\
{\

#define LUA_FUNCS_E()\
{nullptr}, {nullptr}\
};\

/////////// metatable functions helper

// +
#define __ADD()\
{"__add", __add},\

// -
#define __SUB()\
{"__sub", __sub},\

// *
#define __MUL()\
{"__mul", __mul},\

// /
#define __DIV()\
{"__div", __div},\

// %
#define __MOD()\
{"__mod", __mod},\

// ^
#define __POW()\
{"__pow", __pow},\

// 
#define __UNM()\
{"__unm", __unm},\

// //
#define __IDIV()\
{"__idiv", __idiv},\

// &
#define __BAND()\
{"__band", __band},\

// |
#define __BOR()\
{"__bor", __bor},\

#define __BXOR()\
{"__bxor", __bxor},\

#define __BNOT()\
{"__bnot", __bnot},\

// <<
#define __SHL()\
{"__shl", __shl},\

// >>
#define __SHR()\
{"__shr", __shr},\

// ..
#define __CONCAT()\
{"__concat", __concat},\

// #
#define __LEN()\
{"__len", __len},\

// ==
#define __EQ()\
{"__eq", __eq},\

// <
#define __LT()\
{"__lt", __lt},\

// <=
#define __LE()\
{"__le", __le},\


#define __INDEX()\
{"__index", __index},\

#define __NEWINDEX()\
{"__newindex", __newindex},\

#define __CALL()\
{"__call", __call},\


#define __GC()\
{"__gc", __gc},\

#define __CLOSE()\
{"__close", __close},\

#define __NAME()\
{"__name", __name},\

#define __MODE()\
{"__mode", __mode},\

#define __PAIRS()\
{"__pairs", __pairs}\

#define FUNC(FUNC_PTR)\
{#FUNC_PTR, FUNC_PTR},\

#define IMPLEMENT_EXPORTER(NAME)\
struct NAME##_exporter\
{\
static NAME##_exporter instance;\
NAME##_exporter()\
{\
lua_api::lua_vm::register_class(#NAME, functions);\
}\
~NAME##_exporter() = default;\
};\
NAME##_exporter NAME##_exporter::instance;