// sakae's gas project


#include "AbilitySystem/Abilities/AuraGameAbility.h"

FString UAuraGameAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d </>"),L"Default Ability Name - Looreamipsum Looreamipsum Looreamipsum Looreamipsum Looreamipsum Looreamipsum ",Level);
}

FString UAuraGameAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level : </><Level>%d </> \n<Default>Causes much more damage : </> "),Level);
}

FString UAuraGameAbility::GetLoackedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level </><Level> %d </>"),Level);
}
