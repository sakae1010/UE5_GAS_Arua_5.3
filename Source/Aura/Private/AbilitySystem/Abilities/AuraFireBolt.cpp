// sakae's gas project


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraGameplayTag.h"


FString UAuraFireBolt::GetDescription(int32 Level)
{
	return GetDefaultDesc(FString("FIRE BOLT"), Level);
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	return GetDefaultDesc(FString("NEXT LEVEL :"), Level);
}

FString UAuraFireBolt::GetDefaultDesc(const FString& Title, const int Level) const
{
	const float DamageValue = GetDamageByDamageType(Level,FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>%s</> \n\n"
			//Details
			"<Small>Level : </> <Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost : </> <ManaCost>%.1f</>\n"
			//Cooldown
			"<Small>Cooldown : </> <Cooldown>%.1f</>\n\n"
			//Description
			"<Default>Lanunches a Bolt of fire ,"
			" Exploding on impact and delling : </>"
			" <Damage> %.1f </> fire damage with"
			" a chance to burn \n\n"
			),
			//Values 
			*Title,
			Level,
			ManaCost,
			Cooldown,
			DamageValue );	
	}
	
	return FString::Printf(TEXT(
		//Title
		"<Title>%s</> \n\n"
		//Details
		"<Small>Level : </> <Level>%d</>\n"
		//ManaCost
		"<Small>ManaCost : </> <ManaCost>%.1f</>\n"
		//Cooldown
		"<Small>Cooldown : </> <Cooldown>%.1f</>\n\n"
		//Description
		"<Default>Lanunches %d Bolts of fire ,"
		" Exploding on impact and delling : </>"
		" <Damage> %.1f </> fire damage with "
		"a chance to burn"),
		*Title,
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level,NewProjectiles),
		DamageValue);
}
