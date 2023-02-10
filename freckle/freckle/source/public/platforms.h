#ifndef PLATFORMS_H
#define PLATFORMS_H

#include <string>

#ifdef _WIN64
#include <Windows.h>
#endif

namespace platforms
{
    static std::string TCHAR_to_string(TCHAR* str)
    {
#ifdef _WIN64
        int iLen = WideCharToMultiByte(CP_ACP, 0, str, -1, nullptr, 0, nullptr, nullptr);
        char* chRtn = new char[iLen * sizeof(char)];
        WideCharToMultiByte(CP_ACP, 0, str, -1, chRtn, iLen, nullptr, nullptr);

        return chRtn;
#endif
        return {};
    }

	static std::string get_root_path()
	{
#ifdef _WIN64
        TCHAR szBuf[_MAX_PATH];
        GetModuleFileName(NULL, szBuf, 256);
        std::string exePath = TCHAR_to_string(szBuf);
        auto pos = exePath.find_last_of("64", exePath.length());
        return exePath.substr(0, pos - 2) + "freckle\\";
#endif
        return {};
	}
};


#endif


