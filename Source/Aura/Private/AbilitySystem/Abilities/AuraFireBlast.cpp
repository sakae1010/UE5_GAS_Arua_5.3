// sakae's gas project


#include "AbilitySystem/Abilities/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	return GetDefaultDesc( FString("FIRE BLAST"), Level );
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	return GetDefaultDesc( FString("NEXT LEVEL"), Level );
}

FString UAuraFireBlast::GetDefaultDesc(const FString& Title, const int Level) const
{
	const float ScaledDamageValue = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
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
			//NumFireBalls
			"<Default>Lanunches %d,</>"
			"<Default>fire balls in all directions , each coming back and </>"
			"<Default>exlpoding upon retrun , causing</>"
			
			" <Damage> %.1f </> radial fire damage with"
			" a chance to burn \n\n"
			),
			//Values 
			*Title,
			Level,
			ManaCost,
			Cooldown,
			NewFireBalls,
			ScaledDamageValue );	
}
