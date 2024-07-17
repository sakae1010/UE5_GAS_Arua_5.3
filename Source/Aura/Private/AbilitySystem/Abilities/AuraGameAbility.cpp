// sakae's gas project


#include "AbilitySystem/Abilities/AuraGameAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

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

float UAuraGameAbility::GetManaCost(const float InLevel) const
{
	float OutManaCost = 0;
	if (UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if(Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				//取得原始量級(Magnitude) 未縮放的版本 主要用於 ScalableFloat 
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel ,OutManaCost );
				return OutManaCost;
			}
		}
	}
	return OutManaCost;
}

float UAuraGameAbility::GetCooldown(float InLevel) const
{
	float OutCooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		if(CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel,OutCooldown))
		{
			return OutCooldown;
		}
	}
	return OutCooldown;
}
