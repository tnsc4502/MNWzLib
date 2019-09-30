#pragma once
#include <string>

class WzMappedFileStream;
typedef WzMappedFileStream WzStreamType;

class WzArchive;
class WzStreamCodec
{
	unsigned char 
		aWzKey[2][0x10000],
		aWideWzKey[2][0x10000],
		aKeyStore[2][0x10000];

	void GenKey(unsigned char* aKey, unsigned char *aResult);

public:
	WzStreamCodec();
	~WzStreamCodec();
	void Init();
	std::string DecodeString(WzStreamType *pStream);
	std::string DecodePropString(WzStreamType *pStream, unsigned int uRootPropPos);
};

