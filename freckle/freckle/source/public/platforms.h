#ifndef PLATFORMS_H
#define PLATFORMS_H

#ifdef _WIN32
    #ifdef _WIN64
    #include <Windows.h>
    #else
    
    #endif
#elif __APPLE__

#elif __ANDROID__

#elif __linux__

#elif __unix__

#else
#error "Unkown Platform"
#endif

#include <string>

namespace platforms
{

    static std::string wstring_to_string(const std::wstring& input)
    {
#ifdef _WIN64
        int iLen = WideCharToMultiByte(CP_ACP, 0, input.c_str(), -1, nullptr, 0, nullptr, nullptr);
        char* chRtn = new char[iLen * sizeof(char)];
        WideCharToMultiByte(CP_ACP, 0, input.c_str(), -1, chRtn, iLen, nullptr, nullptr);
        return chRtn;
#endif
        return {};
    }

    static std::wstring string_to_wstring(const std::string& input)
    {
#ifdef _WIN64
        int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(input.c_str()), input.size(), 0, 0);
        if (nSize <= 0)
        {
            return {};
        }

        WCHAR* pwszDst = new WCHAR[nSize + 1];
        if (pwszDst == NULL)
        {
            return {};
        }

        MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(input.c_str()), input.size(), pwszDst, nSize);
        pwszDst[nSize] = 0;
        if (pwszDst[0] == 0xFEFF)
        {
            for (int i = 0; i < nSize; ++i)
            {
                pwszDst[i] = pwszDst[i + 1];
            }
        }

        std::wstring wcResult(pwszDst);
        delete pwszDst;
        return wcResult;

#endif
        return {};
    }

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


