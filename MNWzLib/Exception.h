#pragma once

class WvsException
{
public:
	static void FatalError(const char *sFormat, ...);
};