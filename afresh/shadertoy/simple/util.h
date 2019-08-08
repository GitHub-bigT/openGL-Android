#pragma once

#include <windows.h>
#include <time.h>
#include <Synchapi.h>
#include <stdint.h>
#include <string>

void timer_start(FILETIME *start)
{
	GetSystemTimeAsFileTime(start);
}

int64_t timer_elapsed_msec(FILETIME *start)
{
	FILETIME now;
	GetSystemTimeAsFileTime(&now);
	int64_t elapsed = ((((int64_t)now.dwHighDateTime) << 32) + (int64_t)now.dwLowDateTime) -
		((((int64_t)start->dwHighDateTime) << 32) + (int64_t)start->dwLowDateTime);
	return elapsed / 10000;
}

int64_t timer_elapsed_usec(FILETIME *start)
{
	FILETIME now;
	GetSystemTimeAsFileTime(&now);
	int64_t elapsed = ((((int64_t)now.dwHighDateTime) << 32) + (int64_t)now.dwLowDateTime) -
		((((int64_t)start->dwHighDateTime) << 32) + (int64_t)start->dwLowDateTime);
	return elapsed / 10;
}

std::string Wchar_tToString(wchar_t *wchar)
{
	std::string szDst;
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
	char *psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
	szDst = psText;// std::string赋值
	delete[]psText;// psText的清除
	return szDst;
}

LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t) *(orig.length() - 1) + 4);
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

std::string getExeDir()
{
	char exeFullPath[MAX_PATH];
	GetModuleFileNameA(nullptr, exeFullPath, MAX_PATH);
	std::string rootDir = exeFullPath;
	rootDir = rootDir.substr(0, rootDir.find_last_of('\\', rootDir.npos));
	return rootDir;
}