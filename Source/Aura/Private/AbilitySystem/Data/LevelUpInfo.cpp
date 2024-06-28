// sakae's gas project
#include "AbilitySystem/Data/LevelUpInfo.h"


int32 ULevelUpInfo::FindLevelForXP(int32 InXP) const
{
	int32 Level = 1;
	bool bSearching = false;
	while (bSearching)
	{
		if (LevelUpInfos.Num()-1 <= Level)
		{
			break;
		}

		if (InXP >= LevelUpInfos[Level].Exp)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}		
	


	return Level;
}


