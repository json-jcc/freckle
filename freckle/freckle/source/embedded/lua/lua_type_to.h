#ifndef LUA_TYPE_TO_H
#define LUA_TYPE_TO_H

#include "lua_type_is.h"

#include <optional>
#include <unordered_map>

namespace lua_api
{
    template<lua_boolean_acceptable _Ty>
	static _Ty to_impl(lua_State* l, const std::int32_t idx)
	{
		return static_cast<_Ty>(lua_toboolean(l, idx));
	}

	template<lua_integer_number_acceptable _Ty>
	static _Ty to_impl(lua_State* l, const std::int32_t idx)
	{
		return static_cast<_Ty>(lua_tointeger(l, idx));
	}

	template<lua_real_number_acceptable _Ty>
	static _Ty to_impl(lua_State* l, const std::int32_t idx)
	{
		return static_cast<_Ty>(lua_tonumber(l, idx));
	}

	template<lua_string_acceptable _Ty>
	static _Ty to_impl(lua_State* l, const std::int32_t idx)
	{
		return lua_tostring(l, idx);
	}

	template<lua_userdata_acceptable _Ty>
	static _Ty to_impl(lua_State* l, const std::int32_t idx)
	{
		_Ty* userdata = static_cast<_Ty*>(lua_touserdata(l, idx));
		return *userdata;
	}

	template<lua_t_acceptable _Ty>
	static std::optional<_Ty> to(lua_State* l, const std::int32_t idx)
	{
		if(!is<_Ty>(l, idx)) {
			return std::nullopt;
		}

		return to_impl<_Ty>(l, idx);
		
		if constexpr (lua_table_acceptable_std_vector<_Ty>) {
			const std::size_t len = lua_rawlen(l, idx);
			if (len < 1)
			{
				return std::nullopt;
			}
		
			// table, ...
			lua_pushnil(l);
			// key, table, ...
		
			_Ty c;
			c.reserve(len);
		
			while (lua_next(l, idx))
			{
				// value, key, table, ...
				auto v = to<typename _Ty::value_type>(l, -1);
				if (!v)
				{
					lua_pop(l, 2);
					// table, ...
					return std::nullopt;
				}
		
				c.push_back(v.value());
				lua_pop(l, 1);
				// key, table, ...
			}
			// table, ...
			return c;
		}
		else if constexpr (lua_table_acceptable_std_unordered_map<_Ty>)
		{
			_Ty c;
		
			// table, ...
			lua_pushnil(l);
			// nil, table, ...
			while (lua_next(l, idx))
			{
				// value, key, table, ...
				auto _optional_k = to<typename _Ty::key_type>(l, -2);
				auto _optional_v = to<typename _Ty::mapped_type>(l, -1);
				if (!_optional_k || !_optional_v)
				{
					lua_pop(l, 2);
					// table, ...
					return std::nullopt;
				}
				c.emplace(_optional_k.value(), _optional_v.value());
				lua_pop(l, 1);
				// key, table, ...
			}
			// table, ...
			return c;
		}
		else {
			return std::nullopt;
		}

		
	}

	template<lua_t_acceptable _Ty, std::size_t idx>
	static std::optional<_Ty> static_to(lua_State* l)
	{
		return to<_Ty>(l, idx);
	}
	

	// template<typename t, size_t size> 
	// static bool to(std::int32_t index, std::array<t, size>& c) noexcept
	// {
	// 	// if (!basic::is_type(index, lua_t::table))
	// 	// {
	// 	// 	return false;
	// 	// }
	//
	// 	const auto _table_len = basic::raw_len(index);
	// 	if (_table_len != size)
	// 	{
	// 		return false;
	// 	}
	//
	// 	// table, ...
	// 	basic::push_nil();
	// 	// nil, table, ...
	// 	size_t _array_index = 0;
	// 	while (basic::table_next(index))
	// 	{
	// 		// value, key, table, ....
	// 		auto _optional_v = to<t>(l, -1);
	// 		if (!_optional_v.has_value())
	// 		{
	// 			basic::pop(2);
	// 			// table, ....
	// 			return false;
	// 		}
	// 		c[_array_index++] = _optional_v.value();
	// 		basic::pop(1);
	// 		// key, table, ...
	// 	}
	// 	// table, ...
	// 	return true;
	// }
	//

	// template <typename t> 
	// static bool to(std::int32_t index, std::deque<t>& c) noexcept
	// {
	// 	// if (!basic::is_type(index, lua_t::table))
	// 	// {
	// 	// 	return false;
	// 	// }
	//
	// 	const auto table_len = basic::raw_len(index);
	// 	if (table_len < 1)
	// 	{
	// 		return true;
	// 	}
	//
	// 	// table, ...
	// 	basic::push_nil();
	// 	// key, table, ...
	// 	while (basic::table_next(index))
	// 	{
	// 		// value, key, table, ...
	// 		auto _optional_v = to<t>(l, -1);
	// 		if (!_optional_v.has_value())
	// 		{
	// 			basic::pop(2);
	// 			// table, ...
	// 			return false;
	// 		}
	// 		c.push_back(_optional_v.value());
	// 		basic::pop(1);
	// 		// key, table, ...
	// 	}
	// 	// table, ...
	// 	return true;
	// }
	//
	// template<typename t> 
	// static bool to(std::int32_t index, std::list<t>& c) noexcept
	// {
	// 	// if (!basic::is_type(index, lua_t::table))
	// 	// {
	// 	// 	return false;
	// 	// }
	//
	// 	auto _table_len = basic::raw_len(index);
	// 	if (_table_len < 1)
	// 	{
	// 		return true;
	// 	}
	//
	// 	// table, ...
	// 	basic::push_nil();
	// 	// nil, table, ...
	// 	while (basic::table_next(index))
	// 	{
	// 		// value, key, table, ...
	// 		auto _optional_v = to<t>(l, -1);
	// 		if (!_optional_v.has_value())
	// 		{
	// 			basic::pop(2);
	// 			return false;
	// 		}
	// 		c.push_back(_optional_v.value());
	// 		basic::pop(1);
	// 		// key, table, ...
	// 	}
	// 	// table, ...
	// 	return true;
	// }
	//
	// template<typename t> 
	// static bool to(std::int32_t index, std::forward_list<t>& c) noexcept
	// {
	// 	// if (!basic::is_type(index, lua_t::table))
	// 	// {
	// 	// 	return false;
	// 	// }
	//
	// 	auto _table_len = basic::raw_len(index);
	// 	if (_table_len < 1)
	// 	{
	// 		return true;
	// 	}
	//
	// 	// table, ...
	// 	basic::push_nil();
	// 	// nil, table, ...
	// 	while (basic::table_next(index))
	// 	{
	// 		// value, key, table, ...
	// 		auto _optional_v = to<t>(l, -1);
	// 		if (!_optional_v.has_value())
	// 		{
	// 			basic::pop(2);
	// 			// table, ...
	// 			return false;
	// 		}
	// 		c.push_front(_optional_v.value());
	// 		basic::pop(1);
	// 		// key, value, ...
	// 	}
	// 	// table, ...
	// 	return true;
	// }
	//
	// template<typename t> 
	// static bool to(std::int32_t index, std::set<t>& c) noexcept
	// {
	// 	// if (!basic::is_type(index, lua_t::table))
	// 	// {
	// 	// 	return false;
	// 	// }
	//
	// 	size_t _table_len = basic::raw_len(index);
	// 	if (_table_len < 1)
	// 	{
	// 		return true;
	// 	}
	//
	// 	// table, ...
	// 	basic::push_nil();
	// 	// nil, table, ...
	// 	while (basic::table_next(index))
	// 	{
	// 		// value, key, table, ....
	// 		auto _optional_v = to<t>(l, -1);
	// 		if (!_optional_v.has_value())
	// 		{
	// 			basic::pop(2);
	// 			// table, ...
	// 			return false;
	// 		}
	// 		c.insert(_optional_v.value());
	// 		basic::pop(1);
	// 		// key, table, ...
	// 	}
	// 	// table, ...
	// 	return true;
	// }
	//
	// template<typename t> 
	// static bool to(std::int32_t index, std::multiset<t>& c) noexcept
	// {
	// 	// if (!basic::is_type(index, lua_t::table))
	// 	// {
	// 	// 	return false;
	// 	// }
	//
	// 	auto _table_len = basic::raw_len(index);
	// 	if (_table_len < 1)
	// 	{
	// 		return true;
	// 	}
	//
	// 	// table, ...
	// 	basic::push_nil();
	// 	// nil, table, ...
	// 	while (basic::table_next(index))
	// 	{
	// 		// value, key, table, ...
	// 		auto _optional_v = to<t>(l, -1);
	// 		if (!_optional_v.has_value())
	// 		{
	// 			basic::pop(2);
	// 			// table, ...
	// 			return false;
	// 		}
	// 		c.insert(_optional_v.value());
	// 		basic::pop(1);
	// 		// key, table, ...
	// 	}
	// 	// table, ...
	// 	return true; 
	// }
	//
	// template<typename t> 
	// static bool to(lua_State* l, std::int32_t index, std::unordered_set<t>& c)  noexcept
	// {
	// 	// if (!basic::is_type(index, lua_t::table))
	// 	// {
	// 	// 	return false;
	// 	// }
	//
	// 	auto _table_len = basic::raw_len(index);
	// 	if (_table_len < 1)
	// 	{
	// 		return true;
	// 	}
	// 	// table, ...
	// 	basic::push_nil();
	// 	// nil, table, ...
	// 	while (basic::table_next(index))
	// 	{
	// 		// value, key, table, ...
	// 		auto _optional_v = to<t>(l, -1);
	// 		if (!_optional_v.has_value())
	// 		{
	// 			basic::pop(2);
	// 			// table, ...
	// 			return false;
	// 		}
	// 		c.insert(_optional_v.value());
	// 		basic::pop(1);
	// 		// key, value, ...
	// 	}
	// 	// table, ...
	// 	return true;
	// }
	//
	// template<typename t> 
	// static bool to(std::int32_t index, std::unordered_multiset<t>& c) noexcept
	// {
	// 	// if (!basic::is_type(index, lua_t::table))
	// 	// {
	// 	// 	return false;
	// 	// }
	//
	// 	auto _table_len = basic::raw_len(index);
	// 	if (_table_len < 1)
	// 	{
	// 		return true;
	// 	}
	//
	// 	// table, ...
	// 	basic::push_nil();
	// 	// nil, table, ...
	// 	while (basic::table_next(index))
	// 	{
	// 		// value, key, table, ...
	// 		auto _optional_v = to<t>(-1);
	// 		if (!_optional_v.has_value())
	// 		{
	// 			basic::pop(2);
	// 			// table, ...
	// 			return false;
	// 		}
	// 		c.insert(_optional_v.value());
	// 		basic::pop(1);
	// 		// key, table, ...
	// 	}
	// 	// table, ...
	// 	return true;
	// }
	//
	// template<
	// 	typename key_t, typename value_t,
	// 	typename predicate = std::less<key_t>,
	// 	typename allocator = std::allocator<std::pair<const key_t, value_t>>
	// > 
	// static bool to(std::int32_t index, std::map<key_t, value_t, predicate, allocator>& c) noexcept
	// {
	// 	// if (!basic::is_type(index, lua_t::table))
	// 	// {
	// 	// 	return false;
	// 	// }
	//
	// 	auto _table_len = basic::raw_len(index);
	// 	if (_table_len < 1)
	// 	{
	// 		return true;
	// 	}
	//
	// 	// table, ...
	// 	basic::push_nil();
	// 	// nil, table, ...
	// 	while (basic::table_next(index))
	// 	{
	// 		// value, key, table, ...
	// 		auto _optional_k = to<key_t>(l, -1);
	// 		auto _optional_v = to<value_t>(l, -1);
	// 		if (!_optional_k.has_value() || !_optional_v.has_value())
	// 		{
	// 			basic::pop(2);
	// 			// table, ...
	// 			return false;
	// 		}
	// 		c.emplace(_optional_k.value(), _optional_v.value());
	// 		basic::pop(1);
	// 		// key, table, ...
	// 	}
	// 	// table, ...
	// 	return true;
	// }
	//
	 template<
	 	typename key_t, typename value_t,
	 	typename hasher = std::hash<key_t>,
	 	typename predicate = std::equal_to<key_t>,
	 	typename allocator = std::allocator<std::pair<const key_t, value_t>>
	 > 
	 static bool to(std::int32_t index, std::unordered_map<key_t, value_t, hasher, predicate, allocator>& c) noexcept
	 {
	 	// if (!basic::is_type(index, lua_t::table))
	 	// {
	 	// 	return false;
	 	// }
	
	 	
	 }

	template<typename... Args, std::size_t... N, bool bottom = true, std::size_t offset = 0>
	static std::optional<std::tuple<Args...>> to_tuple_impl(lua_State* l, std::index_sequence<N...>)
	{
		if (lua_gettop(l) != sizeof...(Args))
		{
			return std::nullopt;
		}

		std::tuple<std::optional<Args>...> opt_parameters =
		{
			// initializer_list for tuple<...>
			static_to<Args, (N + 1 + offset) * (bottom ? 1 : -1)>(l)...
		};

		bool is_ok = true;

		( ( is_ok = ( is_ok ? std::get<N>(opt_parameters).has_value() : false ) ), ... );

		if( !is_ok )
		{
			return std::nullopt;
		}
		
		return 
		{ // std::optinal<...> 
			{ // initializer_list for std::tuple<...> 
				(std::get<N>(opt_parameters).value())... 
			} 
		};
	}

	template<typename ...Args>
	static std::optional<std::tuple<Args...>> to_tuple(lua_State* l)
	{
		return to_tuple_impl<Args...>(l, std::index_sequence_for<Args...>());
	}
}


#endif