// sakae's gas project


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	return GetDefaultDesc("ELECTROCUTE", Level);
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	return GetDefaultDesc("NEXT LEVEL:", Level);
}

FString UElectrocute::GetDefaultDesc(const FString& Title, const int Level) const
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>%s</>\n\n"
			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Emits a beam of lightning, "
			"connecting with the target, repeatedly causing </>"
			// Damage
			"<Damage>%d</><Default> lightning damage with"
			" a chance to stun</>"),
			// Values
			*Title,
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	return FString::Printf(TEXT(
		// Title
		"<Title>%s</>\n\n"
		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		// Addition Number of Shock Targets
		"<Default>Emits a beam of lightning, "
		"propagating to %d additional targets nearby, causing </>"
		// Damage
		"<Damage>%d</><Default> lightning damage with"
		" a chance to stun</>"),
		// Values
		*Title,
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumShockTargets - 1),
		ScaledDamage);
}