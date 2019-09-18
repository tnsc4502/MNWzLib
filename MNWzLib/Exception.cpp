#include "Exception.h"
#include "StringUtility.h"
#include <memory>
#include <Windows.h>

void WvsException::FatalError(const char *sFormat, ...)
{
	std::unique_ptr<char[]> sFormatted;
	STRUTILITY_PARSE_VAR_LIST(sFormat, sFormatted);
	MessageBoxA(nullptr, (char*)sFormatted.get(), "Fatal Error", 0);
	exit(-1);
}