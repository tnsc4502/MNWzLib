#pragma once
#include <string>
#include <map>
#include "StandardFileSystem.h"

class WzArchive;
class FastAES;
class WzNameSpace;

class WzFileSystem
{
	filesystem::path m_sFileSysPath;
	std::map<std::wstring, WzArchive*> m_mArchive;
	bool m_bInitialized = false;

	//Get the absolute path of "sPath".
	const filesystem::path GetAbsPath(const filesystem::path& fPath) const;

	//Mount (or say, read) a file named sArchiveName(.wz)
	WzNameSpace* TryMount(const filesystem::path& sArchiveName);

	FastAES* m_pChiper = nullptr;

public:
	WzFileSystem();
	~WzFileSystem();

	//Initialize WzFileSystem, sPath = sub directory that contains Wz files.
	void Init(const filesystem::path& sPath);

	WzNameSpace* GetItem(const filesystem::path& sArchiveName);
};

