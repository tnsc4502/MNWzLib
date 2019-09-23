#pragma once
#include <string>

//USE WIN32 MAPPING FILE IMPL.
#define USE_MAPPING_FILE
#define USE_FAST_AES

#ifdef USE_MAPPING_FILE
#include "WzMappedFileStream.h"
typedef WzMappedFileStream WzStreamType;
#else
#include "WzStream.h"
typedef WzStream WzStreamType;
#endif

#ifdef USE_FAST_AES
class FastAES;
typedef FastAES CipherType;
#else
class AESCipher;
typedef AESCipher CipherType;
#endif

class WzPackage;
class FastAES;
class WzNameSpace;

class WzArchive
{
	bool m_bEncrypted = true;
	unsigned long long int m_ulLength = 0;
	unsigned int m_uBeginPos = 0, m_uStreamPos = 0, m_uKey = 0;

	WzStreamType* m_pStream = nullptr;
	WzNameSpace* m_pTopNameSpace = nullptr;
	CipherType* m_pCipher = nullptr;

	WzArchive(const std::wstring& sArchivePath, const std::string& sArchiveName, CipherType* pChipher);

public:
	~WzArchive();
	static WzArchive* Mount(const std::wstring& sArchivePath, const std::string& sArchiveName, CipherType* pChipher);
	void SetPosition(unsigned int uPos);
	WzStreamType* GetStream();
	CipherType* GetCipher();
	unsigned int GetPosition() const;
	unsigned int GetBeginPos() const;
	unsigned int GetArchiveKey() const;
	unsigned long long int GetLength() const;
	void SetLength(unsigned long long int ulLength);
	void Read(char *pBuffer, unsigned int uSize);
	WzNameSpace* GetRoot();
	std::string DecodeString();
	std::string DecodePropString(unsigned int uRootPropPos);
	bool Encrypted() const;

	//ZDataFilter::_Read
	template<typename T>
	T ReadFilter()
	{
		return m_pStream->Read<T>();
	}
};

