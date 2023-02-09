#ifndef RUNTIME_CORE_H
#define RUNTIME_CORE_H

#include <vector>

enum class access_category
{
    s_public,
    s_protected,
    s_private
};

enum class type_category
{
    s_built_in,
    s_struct,
    s_class,
    s_enum
};

enum class member_category
{
    variable,
    function
};

struct field
{
    type_category type_category_;
    
};

struct function
{
    
};


struct type
{
    type_category type_category_;

    std::vector<field*> fields_;

    
};




#endif