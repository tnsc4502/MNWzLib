#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include "WzNameSpace.h"
#include "WzDelayedVariant.h"

class WzArchive;

class WzProperty : public WzNameSpace
{
	//friend class WzIterator;
public:
	//typedef std::map<std::string, WzProperty*> PropContainerType;

private:
	//unsigned int m_uBeginPos = 0;
	//PropContainerType m_mChild;

	//std::string m_sName;
	WzDelayedVariant m_wzVariant;
	//WzArchive* m_pArchive = nullptr;

	//Variant property
	WzProperty(const std::string& sPropName);
public:
	WzProperty(WzArchive *pArchive, const std::string& sPropName, unsigned int uBeginPos, unsigned int uRootPropPos);
	~WzProperty();

	void OnGetItem();
	//const std::string& GetName();
	//WzProperty* GetItem(const std::string& sName);
	WzDelayedVariant& GetVariant();
	WzProperty* GetProperty();
};

