#pragma once
#include <string>

class WzArchive;

class FastAES
{
	unsigned char 
		aWzKey[2][0x10000],
		aWideWzKey[2][0x10000],
		aKeyStore[2][0x10000];

	void GenKey(unsigned char* aKey, unsigned char *aResult);

public:
	FastAES();
	~FastAES();
	void Init();
	std::string DecodeString(WzArchive *pArchive);
};

