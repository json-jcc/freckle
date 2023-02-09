#ifndef LUA_TYPE_IS_H
#define LUA_TYPE_IS_H

#include "lua_type_concepts.h"

namespace lua_api
{
    template<lua_boolean_acceptable T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_isboolean(l, idx);
    }

    template<lua_integer_number_acceptable T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_isinteger(l, idx);
    }

    template<lua_real_number_acceptable T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_isnumber(l, idx);
    }

    template<lua_string_acceptable T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_isstring(l, idx) && !lua_isnumber(l, idx);
    }

    template<lua_userdata_acceptable T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_isuserdata(l, idx);
    }

    template<lua_table_acceptable_std_array T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_istable(l, idx);
    }

    template<lua_table_acceptable_std_vector T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_istable(l, idx);
    }

    template<lua_table_acceptable_std_deque T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_istable(l, idx);
    }

    template<lua_table_acceptable_std_list T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_istable(l, idx);
    }

    template<lua_table_acceptable_std_forward_list T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_istable(l, idx);
    }

    template<lua_table_acceptable_std_set T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_istable(l, idx);
    }

    template<lua_table_acceptable_std_unordered_set T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_istable(l, idx);
    }

    template<lua_table_acceptable_std_unordered_multiset T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_istable(l, idx);
    }

    template<lua_table_acceptable_std_map T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_istable(l, idx);
    }

    template<lua_table_acceptable_std_unordered_map T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_istable(l, idx);
    }

    template<lua_table_acceptable_std_multimap T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_istable(l, idx);
    }

    template<lua_table_acceptable_std_unordered_multimap T>
    static bool is_impl(lua_State* l, const std::int32_t idx)
    {
        return lua_istable(l, idx);
    }
    

    template<lua_t_acceptable T>
    static bool is(lua_State* l, const std::int32_t idx) noexcept
    {
        return is_impl<T>(l, idx);
    }
    
}


#endif