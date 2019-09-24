
#include "..\MNWzLib\WzFileSystem.h"
#include "..\MNWzLib\WzPackage.h"
#include "..\MNWzLib\WzIterator.h"
#include "SkillEntry.h"
#include "SkillLevelData.h"
#include "SkillInfo.h"

#include <thread>
#include <unordered_map>

#define CHECK_SKILL_ATTRIBUTE(var, attribute) if(attributeSet.find(#attribute) != attributeSetEnd) (mappingTable[(&(var)) - pAttributeBase]=(std::string)skillCommonImg[#attribute]);
#define PARSE_SKILLDATA(attribute) ((int)(skillCommonImg[#attribute]));

WzFileSystem wzFileSys;
WzNameSpace* pArchive = nullptr;

auto t1 = std::chrono::high_resolution_clock::now();

SkillInfo::SkillInfo()
{
	wzFileSys.Init("./WzDir/");
	pArchive = wzFileSys.GetItem("Skill.wz"); 
	m_nOnLoadingSkills = 0;
}


SkillInfo::~SkillInfo()
{
}

SkillInfo * SkillInfo::GetInstance()
{
	static SkillInfo* pInstance = new SkillInfo;
	return pInstance;
}


bool SkillInfo::IsValidRootName(const std::string & sName)
{
	for (char c : sName)
		if (c == 0)
			break;
		else if (!isdigit(c))
			return false;
	return true;
}

/*int SkillInfo::GetLoadingSkillCount() const
{
	return m_nOnLoadingSkills;
}*/

const std::map<int, std::map<int, SkillEntry*>*>& SkillInfo::GetSkills() const
{
	return m_mSkillByRootID;
}

const std::map<int, SkillEntry*>* SkillInfo::GetSkillsByRootID(int nRootID) const
{
	auto findIter = m_mSkillByRootID.find(nRootID);
	if (findIter == m_mSkillByRootID.end())
		return nullptr;

	return findIter->second;
}

const SkillEntry * SkillInfo::GetSkillByID(int nSkillID) const
{
	int nJobID = nSkillID / 10000;
	auto rootData = GetSkillsByRootID(nJobID);
	if (!rootData)
		return nullptr;

	auto findResult = rootData->find(nSkillID);
	return (findResult != rootData->end() ? findResult->second : nullptr);
}

void SkillInfo::IterateSkillInfo()
{
	printf("[SkillInfo::IterateSkillInfo<IterateSkillInfo>]On iterating all skills....\n");
	t1 = std::chrono::high_resolution_clock::now();
	((WzPackage*)pArchive)->LoadSubItem(); //Expand all sub nodes
	//static auto& skillWz = stWzResMan->GetWz(Wz::Skill);
	bool continued = false;
	int nRootID;
	static std::vector<std::pair<int, WzIterator>> aRoot;
	WzIterator iter(pArchive);
	std::string name;
	for (auto& node : iter)
	{
		name = node.GetName();
		name[name.find('.')] = '\0';
		if (!IsValidRootName(name))
			continue;
		//printf("node name = %s\n", node.GetName().c_str());
		aRoot.push_back({ atoi(name.c_str()), node });
	}
	m_nRootCount = (int)aRoot.size();
	for (auto& node : aRoot)
	{
		nRootID = node.first;
		//LoadSkillRoot(nRootID, (void*)(&node.second));
		std::thread t(&SkillInfo::LoadSkillRoot, this, nRootID, (void*)(&node.second));
		t.detach();
	}
}

void SkillInfo::LoadSkillRoot(int nSkillRootID, void * pData)
{
	auto skillRootIter = m_mSkillByRootID.find(nSkillRootID);
	if (skillRootIter == m_mSkillByRootID.end()) 
		m_mSkillByRootID.insert({nSkillRootID, new std::map<int, SkillEntry*>() });
	//auto skillRootImg = (WzNameSpace*)pData;

	WzIterator&skillIter = *((WzIterator*)pData);
	skillIter = skillIter["skill"];

	int nSkillID = 0;
	for (auto skillImg : skillIter)
	{
		++m_nOnLoadingSkills;
		//printf("Iter skill root : %s\n", skillImg.GetName().c_str());
		nSkillID = atoi(skillImg.GetName().c_str());
		LoadSkill(nSkillRootID, nSkillID, (void*)&skillImg);
		--m_nOnLoadingSkills;
	}
	if (m_nOnLoadingSkills == 0 && m_mSkillByRootID.size() >= m_nRootCount) 
	{
		auto t2 = std::chrono::high_resolution_clock::now();
		printf("%d item loaded, time = %lld\n", 0, std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());

		printf("Damage = %d\n", GetSkillByID(5211005)->GetLevelData(1)->m_nDamage);
		printf("mpCon = %d\n", GetSkillByID(5211005)->GetLevelData(27)->m_nMpCon);
		//stWzResMan->ReleaseMemory();
		printf("[SkillInfo::IterateSkillInfo<IterateSkillInfo>]Skill information are completely loaded.\n");
		
	}
}

SkillEntry * SkillInfo::LoadSkill(int nSkillRootID, int nSkillID, void * pData)
{
	int nElemAttr = 0;
	bool bLevelStructure = false;
	auto skillDataImg = *((WzIterator*)pData);
	std::string sElemAttr = (std::string)(skillDataImg[("elemAttr")]);
	//if (sElemAttr != "")
	//	GetElementAttribute(sElemAttr.c_str(), &nElemAttr);

	auto& skillCommonImg = (skillDataImg["level"]);
	bLevelStructure = true;

	auto& skillInfoImg = (skillDataImg["info"]);
	auto& skillListImg = (skillDataImg["skillList"]);
	auto& skillReqImg = (skillDataImg["req"]);
	SkillEntry* pResult = new SkillEntry;
	pResult->SetAttackElemAttr(nElemAttr);
	pResult->SetSkillID(nSkillID);
	pResult->SetMasterLevel(atoi(((std::string)*(skillDataImg["masterLevel"])).c_str()));
	pResult->SetMaxLevel(atoi(((std::string)*(skillDataImg["maxLevel"])).c_str()));
	pResult->SetInvisible((int)*(skillDataImg["invisible"]) == 1 ? true : false);
	if(bLevelStructure)
		LoadLevelDataByLevelNode(nSkillID, pResult, (void*)&skillCommonImg, (void*)(&skillDataImg));
	
	std::lock_guard<std::mutex> lock(m_mtxSkillResLock);
	m_mSkillByRootID[nSkillRootID]->insert({ nSkillID, pResult });
	return pResult;
}

void SkillInfo::LoadLevelDataByLevelNode(int nSkillID, SkillEntry * pEntry, void * pData, void *pRoot)
{
	auto& skillLevelImg = *((WzIterator*)pData);
	//auto& skillData = *((WZ::Node*)pRoot);
	//WZ::Node empty;
	int nMaxLevel = pEntry->GetMaxLevel();
	pEntry->AddLevelData(nullptr); //for lvl 0

	double d = 0;
	WzProperty *pProp = nullptr;
	for (auto& skillCommonImg : skillLevelImg)
	{
		d = atof(skillCommonImg.GetName().c_str());

		SkillLevelData* pLevelData = new (SkillLevelData);
		pLevelData->m_nAcc = PARSE_SKILLDATA(acc);
		pLevelData->m_nCraft = PARSE_SKILLDATA(craft);
		pLevelData->m_nAttackCount = PARSE_SKILLDATA(attackCount);
		pLevelData->m_nBulletConsume = PARSE_SKILLDATA(bulletConsume);
		pLevelData->m_nBulletCount = PARSE_SKILLDATA(bulletCount);
		pLevelData->m_nCooltime = PARSE_SKILLDATA(cooltime);
		pLevelData->m_nCooltime *= 1000;

		pLevelData->m_nDamage = PARSE_SKILLDATA(damage);
		pLevelData->m_nEva = PARSE_SKILLDATA(eva);
		pLevelData->m_nHp = PARSE_SKILLDATA(hp);
		pLevelData->m_nHpCon = PARSE_SKILLDATA(hpCon);
		pLevelData->m_nItemCon = PARSE_SKILLDATA(itemCon);
		pLevelData->m_nItemConNo = PARSE_SKILLDATA(itemConNo);
		pLevelData->m_nJump = PARSE_SKILLDATA(jump);
		pLevelData->m_nMastery = PARSE_SKILLDATA(mastery);
		pLevelData->m_nMobCount = PARSE_SKILLDATA(mobCount);
		pLevelData->m_nMorph = PARSE_SKILLDATA(morph);
		pLevelData->m_nMp = PARSE_SKILLDATA(mp);
		pLevelData->m_nMpCon = PARSE_SKILLDATA(mpCon);
		pLevelData->m_nPad = PARSE_SKILLDATA(pad);
		pLevelData->m_nPdd = PARSE_SKILLDATA(pdd);
		pLevelData->m_nMad = PARSE_SKILLDATA(mad);
		pLevelData->m_nMdd = PARSE_SKILLDATA(mdd);
		pLevelData->m_nProp = PARSE_SKILLDATA(prop);
		pLevelData->m_nRange = PARSE_SKILLDATA(range);
		pLevelData->m_nSpeed = PARSE_SKILLDATA(speed);
		pLevelData->m_nTime = PARSE_SKILLDATA(time);
		pLevelData->m_nTime *= 1000;

		pLevelData->m_nX = PARSE_SKILLDATA(x);
		pLevelData->m_nY = PARSE_SKILLDATA(y);
		pLevelData->m_nZ = PARSE_SKILLDATA(z);
		pLevelData->m_nHpR = PARSE_SKILLDATA(hpR);
		pLevelData->m_nMpR = PARSE_SKILLDATA(mpR);
		pLevelData->m_nMoneyCon = PARSE_SKILLDATA(moneyCon);
		pLevelData->m_nLt = PARSE_SKILLDATA(lt);
		pLevelData->m_nRb = PARSE_SKILLDATA(rb);
		pLevelData->m_nFixDamage = PARSE_SKILLDATA(fixdamage);
		pLevelData->m_nCriticalDamage = PARSE_SKILLDATA(criticalDamage);
		auto&lt = skillCommonImg["lt"];
		if (lt != lt.end())
		{
			//printf("lt x = %d, y = %d\n", (int)lt["x"], (int)lt["y"])
		}
		//if (IsSummonSkill(nSkillID))
		//	pLevelData->m_nMobCount = skillData["summon"]["attack1"]["info"]["mobCount"];

		pEntry->AddLevelData(pLevelData);
	}
	pEntry->SetMaxLevel((int)pEntry->GetAllLevelData().size() - 1);
}
