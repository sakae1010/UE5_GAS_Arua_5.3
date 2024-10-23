// sakae's gas project


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	return GetDefaultDesc("ARCANE SHARDS", Level);
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
{
	return GetDefaultDesc("Next Level:", Level);
}

FString UArcaneShards::GetDefaultDesc(const FString& Title, const int Level) const
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
			
			"<Default>Summon a shard of arcane energy, "
			"causing radial arcane damage of  </>"
			// Damage
			"<Damage>%d</><Default> at the shard origin.</>"),
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
		"<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"
		// Damage
		"<Damage>%d</><Default> at the shard origins.</>"),
		// Values
		*Title,
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumShards),
		ScaledDamage);
}

