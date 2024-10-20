﻿// sakae's gas project


#include "AbilitySystem/Data/AttributeInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfo(const FGameplayTag InAttributeTag, bool bLogNotFund)
{
	for(FAuraAttributeInfo& AttributeInfo : AttributeInfos)
	{
		if(AttributeInfo.AttributeTag.MatchesTagExact(InAttributeTag))
		{
			return AttributeInfo;
		}
	}
	if(bLogNotFund)
	{
		UE_LOG(LogAura, Warning, TEXT("AttributeInfo not found for tag [%s] from [%s] "), *InAttributeTag.ToString() , *GetNameSafe(this));
	}
	return FAuraAttributeInfo();
}
