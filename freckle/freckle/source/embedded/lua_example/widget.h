#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>


class widget
{

    std::float_t w = 0.5f;

    std::shared_ptr<widget> xx;
    
public:

    float public_v = 1.0f;

    static float static_public_v;

    widget();

    ~widget();

    static std::shared_ptr<widget> new_lua()
    {
        return std::make_shared<widget>();
    }

    static void wrap_collector(std::shared_ptr<widget>& w)
    {
        w.reset();
    }

    static std::string to_string(const std::shared_ptr<widget>& wid)
    {
        return std::to_string(wid->w);
    }

    static std::int32_t calc(std::int32_t& a, const std::int32_t& b)
    {
        return a * b;
    }

    [[nodiscard]] const std::shared_ptr<widget>& get_v()
    {
        return xx;
    }

    std::float_t replace(const std::shared_ptr<widget>& other)
    {
        return w;
    }

    void set_values(const std::unordered_map<std::string, std::int32_t>& vls)
    {
        values = vls;
    }

private:

    std::unordered_map<std::string, std::int32_t> values;

};

