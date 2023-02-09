#pragma once

#include <memory>
#include <string>
#include <iostream>

namespace kanas
{
	static void log(const std::string& msg)
	{
		std::cout << "[kanas - lua] : " << msg << std::endl;
	}

	static std::int32_t logf(const char* msg)
	{
		std::cout << "[kanas - lua - f] : " << msg << std::endl;
		return 10086;
	}
}