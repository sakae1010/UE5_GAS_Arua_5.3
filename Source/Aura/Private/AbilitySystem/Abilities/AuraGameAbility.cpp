// sakae's gas project


#include "AbilitySystem/Abilities/AuraGameAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameAbility::GetDescription(int32 Level)
{
	return GetDefaultDesc("Default Description", Level);
}

FString UAuraGameAbility::GetNextLevelDescription(int32 Level)
{
	return GetDefaultDesc("Default Description", Level);
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

FString UAuraGameAbility::GetDefaultDesc(const FString& Title, const int Level) const
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		// Title
		"<Title>%s</>\n\n"
		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		// Default
		"<Default>Default Description</>"),
		// Values
		*Title,
		Level,
		ManaCost,
		Cooldown);
}
