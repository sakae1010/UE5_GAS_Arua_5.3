// sakae's gas project


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfForTag(const FGameplayTag InAbilityTag, bool bLogNotFund) const
{
	if(AuraAbilityInformations.Num() == 0 && bLogNotFund)
	{
		UE_LOG(LogAura,Error,TEXT("AuraAbilityInformations is empty"));
		return FAuraAbilityInfo();
	}
	for (const FAuraAbilityInfo& AbilityInfo : AuraAbilityInformations)
	{
		if(AbilityInfo.AbilityTag.MatchesTag(InAbilityTag))
		{
			return AbilityInfo;
		}
	}
	if(bLogNotFund)
	{
		UE_LOG(LogAura,Error,TEXT("[%s]    AbilityTag %s not found"),*GetNameSafe(this),*InAbilityTag.ToString());
	}
	return FAuraAbilityInfo();
}
