#ifndef LVM_H
#define LVM_H

#include <format>

#include "lua_type_push.h"
#include "lua_type_is.h"
#include "lua_type_to.h"

#include <functional>
#include <iostream>

namespace lua_api
{
	// function template name lookup (which may involve argument-dependent lookup)
	// template argument deduction
	// template argument substitution (which may invole SFINAE)
	// overload resolution

	// DeOtherSide dos  

	namespace debug
	{
		static void trace(lua_State* l)
		{
			lua_Debug entry;
			int depth = 0; 

			while (lua_getstack(l, depth, &entry))
			{
				lua_getinfo(l, "Slntu", &entry);
				std::cout<< "" <<std::endl;
				
				printf("%s(%d): %s\n", entry.short_src, entry.currentline, entry.name ? entry.name : "?");
				++depth;
			}
		}
	}
	
	
	template<typename Ret, typename... Args, std::size_t... ArgNs>
	static Ret call_cpp_func_impl(
		const std::function<Ret(Args...)>& func,
		std::tuple<std::remove_cvref_t<Args>...>& args,
		std::index_sequence<ArgNs...>
		)
	{
		if constexpr (std::is_void_v<Ret>)
		{
			func(std::get<ArgNs>(args)...);
		} else {
			return func(std::get<ArgNs>(args)...);
		}
	}
	
	template<typename Ret, typename... Args>
	static std::int32_t call_cpp_func(lua_State* l, const std::function<Ret(Args...)>& func)
	{
#if _DEBUG
		debug::trace(l);
#endif
		// std::optional<std::tuple<Args...>>
		auto opt_vars = to_tuple<std::remove_cvref_t<Args>...>(l);

		if(!opt_vars)
		{
			std::clog << "[lua -> cpp] parameters are not valid." << std::endl;
			lua_pushnil(l);
			return 1; 
		}
		
		if constexpr (std::is_void_v<Ret>)
		{
			// std::tuple<Args...>
			call_cpp_func_impl(func, opt_vars.value(), std::index_sequence_for<Args...>());
			return 0;
		} else {
			// if the func has return result, push it into the stack
			lua_api::push(l, call_cpp_func_impl(func, opt_vars.value(), std::index_sequence_for<Args...>()));
			return 1;
		}
	}

	template<typename Ret, typename... Args>
	struct cpp_function_delegate
	{
		cpp_function_delegate(const std::string& name, Ret(* f)(Args...))
		{
			delegate_name = name;
			callable = f;
		}
    
		std::string delegate_name;
		std::function<Ret(Args...)> callable;
	};

	template<typename Ret, typename This, typename... Args, std::size_t... ArgNs>
	static Ret call_cpp_member_func_impl(
		const std::function<Ret(This*, Args...)>& func,
		std::conditional_t<sizeof...(Args) == 0,
			const std::tuple<std::shared_ptr<This>>,
			const std::tuple<std::shared_ptr<This>, std::remove_cvref_t<Args>...>>& vars,
		//const std::tuple<std::shared_ptr<This>, std::remove_cvref_t<Args>... > &vars,
		std::index_sequence<ArgNs...>
		)
	{
		if constexpr (std::is_void_v<Ret>)
		{
			func(std::get<0>(vars).get(), std::get<ArgNs + 1>(vars)...);
		} else {
			return func(std::get<0>(vars).get(), std::get<ArgNs + 1>(vars)...);
		}
	}
	
	template<typename Ret, typename This, typename... Args>
	static std::int32_t call_cpp_member_func(lua_State* l, std::function<Ret(This*, Args...)> func)
	{
#if _DEBUG
		debug::trace(l);
#endif
		auto opt_vars = to_tuple<std::shared_ptr<This>, std::remove_cvref_t<Args>...>(l);
		
		if(!opt_vars)
		{
			std::clog << "[lua -> cpp] parameters are invalid." << std::endl;
			lua_pushnil(l);
			return 1;
		}

		if constexpr (std::is_void_v<Ret>)
		{
			// std::tuple<...>
			call_cpp_member_func_impl(func, opt_vars.value(), std::index_sequence_for<Args...>());
			return 0;
		} else {
			push(l, call_cpp_member_func_impl(func, opt_vars.value(), std::index_sequence_for<Args...>()));
			return 1;
		}
	}
	
	template<typename Ret, typename This, typename... Args>
	struct cpp_member_function_delegate
	{
		cpp_member_function_delegate(const std::string& name, Ret(This::* f)(Args...))
		{
			delegate_name = name;
			callable = f;
		}

		std::string delegate_name;
		std::function<Ret(This*, Args...)> callable;
	};

	static void api_test()
	{
		debug::trace(nullptr);

		std::function<void(int, float)> f;
		std::tuple<int, float> vars;
		call_cpp_func_impl(f, vars, std::index_sequence_for<int, float>());

		call_cpp_func(nullptr, f);
		
		std::function<int(int*, float, int)> m_f;
		std::tuple<std::shared_ptr<int>, float, int> m_vars;
		call_cpp_member_func_impl(m_f, m_vars, std::index_sequence_for<float, int>());
	}
	
}

#endif