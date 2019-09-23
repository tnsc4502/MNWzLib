#include "..\MNWzLib\WzFileSystem.h"
#include "..\MNWzLib\WzArchive.h"
#include "..\MNWzLib\WzNameSpace.h"
#include "..\MNWzLib\WzPackage.h"
#include "..\MNWzLib\WzIterator.h"
#include "SkillInfo.h"

#include <iostream>

#include <thread>

int main()
{
	WzFileSystem wz;
	wz.Init(L"./WzDir/");
	//WzIterator reward(wz.GetItem("Reward.img"));
	/*for (auto& mobNode : reward)
	{
		for (auto& rewardNode : mobNode)
		{
			std::cout << "Node = " << rewardNode.GetName() << std::endl;
			for (auto& node : rewardNode)
			{
				if (node.GetName() == "prob") {
					std::string strProb = node;
					auto dProb = atof(strProb.substr(4, strProb.size() - 4).c_str()) * 1000000000.0;
					std::cout << "\tProb = " << dProb << std::endl;
					//pInfo->m_unWeight = (unsigned int)dProb;
				}
				else if (node.GetName() == "money")
				{
					std::cout << "\tMoney = " << (std::string)node << std::endl; //test int to str conversion.
					//pInfo->m_nMoney = (int)node;
					//pInfo->m_nType = 0;
				}
				else if (node.GetName() == "item")
				{
					std::cout << "\tItem = " << (std::string)node << std::endl;  //test int to str conversion.
					//pInfo->m_nType = 1;
					//pInfo->m_nItemID = (int)node;
					//int nQuestID = QuestMan::GetInstance()->GetQuestByItem(pInfo->m_nItemID);

				}
				else if (node.GetName() == "min")
					std::cout << "\tmin = " << (int)node << std::endl;
				//pInfo->m_nMin = (int)node;
				else if (node.GetName() == "max")
					std::cout << "\tmax = " << (int)node << std::endl;
				//pInfo->m_nMax = (int)node;
				else if (node.GetName() == "period")
					std::cout << "\tperiod = " << (int)node << std::endl;
				//pInfo->m_nPeriod = (int)node;
				else if (node.GetName() == "premium")
					std::cout << "\tpremium = " << (int)node << std::endl;
				//pInfo->m_bPremiumMap = ((int)node == 1);
			}
		}
	}*/
	auto pEtc = wz.GetItem("Etc.wz");
	WzIterator etc(pEtc);
	for (auto& node : etc)
		std::cout << node.GetName() << std::endl;
	auto pSkill = wz.GetItem("Skill.wz");
	WzIterator iSkill(pSkill);
	for (auto& top : iSkill)
	{
		auto& skill = top["skill"];
		for (auto& skills : skill)
		{
			std::cout << skills.GetName() << std::endl;
		}
		std::cout << top.GetName() << std::endl;

	}
	//WzFileSystem wz;
	//wz.Init(L"./WzDir/");
	//auto pMap = wz.GetItem("Map.wz");
	//((WzPackage*)pMap->GetItem("Obj"))->LoadSubItem();
	/*WzFileSystem wz;
	wz.Init(L"./WzDir/");
	auto t1 = std::chrono::high_resolution_clock::now();
	auto pSkill = wz.GetItem("Skill");
	WzIterator iSkill(pSkill);
	for (auto& top : iSkill)
	{
		auto& skill = top["skill"];
		for (auto& skills : skill)
		{
			std::cout << skills.GetName() << std::endl;
		}
		std::cout << top.GetName() << std::endl;
		
	}*/

	/*WzIterator mapTopIter(pMapArchive);
	for (auto& node : mapTopIter)
		std::cout << node.GetName() << std::endl;
	auto pArchvie = wz.GetItem("Skill");
	if (!pArchvie)
		std::cout << "File doest not exist." << std::endl;
	//auto pMapDir = pArchvie->GetItem("Map");
	//auto pMapSubDir = pMapDir->GetItem("Map1");
	auto pProp = (pArchvie->GetItem("000.img"))->GetProperty();
	auto pSkillProp = pProp->GetItem("skill");
	auto pPr = pSkillProp->GetItem("0001005")->GetItem("level")->GetItem("1");


	WzIterator iter((pArchvie->GetItem("000.img")));
	auto iter2 = iter["skill"]["0001005"]["effect"]["10"];
	for (auto& i : iter2)
	{
		std::cout << i.GetName() << std::endl;
	}
	std::cout << (int)iter["skill"]["0001005"]["level"]["1"]["cooltime"] << std::endl;
	for (auto& i : iter["skill"])
	{
		std::cout << i.GetName() << std::endl;
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	printf("%d item loaded, time = %lld\n", 0, std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
	*/
	//auto t1 = std::chrono::high_resolution_clock::now();
	SkillInfo::GetInstance()->IterateSkillInfo();
	//auto t2 = std::chrono::high_resolution_clock::now();
	//printf("%d item loaded, time = %lld\n", 0, std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
	int n;
	std::cin >> n;
}