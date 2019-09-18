#pragma once
#include <map>
#include <mutex>
#include <atomic>

struct GA_Character;
struct GW_SkillRecord;
class SkillEntry;
class MobSkillEntry;
struct MCSkillEntry;
struct MCGuardianEntry;

class SkillInfo
{
	//How many root img should be processed parallelly
	int m_nRootCount = 0;
	std::atomic<int> m_nOnLoadingSkills;
	std::mutex m_mtxSkillResLock;
	std::map<int, std::map<int, SkillEntry*> *> m_mSkillByRootID;

public:
	enum ElementAttribute
	{
		e_ElemAttr_None = 0x00,
		e_ElemAttr_Ice,
		e_ElemAttr_Fire,
		e_ElemAttr_Lightning,
		e_ElemAttr_Poison,
		e_ElemAttr_Holy,
		e_ElemAttr_Dark,
		e_ElemAttr_U, //Unknown [u, U]
	};

	SkillInfo();
	~SkillInfo();

	const std::map<int, std::map<int, SkillEntry*> *>& GetSkills() const;
	const std::map<int, SkillEntry*> * GetSkillsByRootID(int nRootID) const;
	const SkillEntry* GetSkillByID(int nSkillID) const;

	static SkillInfo* GetInstance();
	static bool IsValidRootName(const std::string& sName);

	void IterateSkillInfo();
	void LoadSkillRoot(int nSkillRootID, void* pData);
	SkillEntry* LoadSkill(int nSkillRootID, int nSkillID, void* pData);
	void LoadLevelDataByLevelNode(int nSkillID, SkillEntry* pEntry, void* pData, void *pRoot);

};

