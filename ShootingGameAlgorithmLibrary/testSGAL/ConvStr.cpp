#include "ConvStr.h"
#include <windows.h>
#pragma warning(disable : 4996) // use sprintf

void ConvStr::WcharToChar(const wchar_t *unicode, char *ansi)
{
	int n;
	n = WideCharToMultiByte(CP_ACP, 0, unicode, -1, ansi, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, unicode, -1, ansi, n, 0, 0);
}

void ConvStr::CharToWchar(wchar_t *unicode, const char *ansi)
{
	int n;
	n = MultiByteToWideChar(CP_ACP, 0, ansi, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, ansi, -1, unicode, n);
} 

std::wstring ConvStr::GetWstr( int number )
{
	wchar_t data[MAX_PATH];
	wsprintf(data, L"%d", number);
	return std::wstring(data);
}

std::wstring ConvStr::GetWstr( unsigned int number )
{
	wchar_t data[MAX_PATH];
	wsprintf(data, L"%d", number);
	return std::wstring(data);
}

std::wstring ConvStr::GetWstr( long long number )
{
	wchar_t data[MAX_PATH];
	wsprintf(data, L"%ld", number);
	return std::wstring(data);
}

std::wstring ConvStr::GetWstr( float number )
{
	wchar_t data[MAX_PATH];
	wsprintf(data, L"%f", number);
	return std::wstring(data);
}

std::wstring ConvStr::GetWstr( double number )
{
	wchar_t data[MAX_PATH];
	wsprintf(data, L"%lf", number);
	return std::wstring(data);
}

std::string ConvStr::GetStr( int number )
{
	char data[MAX_PATH];
	sprintf(data, "%d", number);
	return std::string(data);
}

std::string ConvStr::GetStr( unsigned int number )
{
	char data[MAX_PATH];
	sprintf(data, "%d", number);
	return std::string(data);
}

std::string ConvStr::GetStr( long long number )
{
	char data[MAX_PATH];
	sprintf(data, "%ld", number);
	return std::string(data);
}

std::string ConvStr::GetStr( float number )
{
	char data[MAX_PATH];
	sprintf(data, "%f", number);
	return std::string(data);
}

std::string ConvStr::GetStr( double number )
{
	char data[MAX_PATH];
	sprintf(data, "%lf", number);
	return std::string(data);
}
std::wstring ConvStr::GetWstr( std::wstring unicode )
{
	return std::wstring(unicode);
}

std::wstring ConvStr::GetWstr( std::string ansi )
{
	std::vector<wchar_t> unicode;
	unicode.resize(ansi.length()+1);
	CharToWchar(&unicode[0], &ansi[0]);
	return std::wstring(&unicode[0]);
}

std::string ConvStr::GetStr( std::wstring unicode )
{
	std::vector<char> ansi;
	ansi.resize(unicode.length()*2+1);
	WcharToChar(&unicode[0], &ansi[0]);
	return std::string(&ansi[0]);
}

std::string ConvStr::GetStr( std::string ansi )
{
	return std::string(ansi);
}

