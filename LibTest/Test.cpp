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
	auto pMap = wz.GetItem("Map.wz");
	((WzPackage*)pMap->GetItem("Obj"))->LoadSubItem();
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