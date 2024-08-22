// sakae's gas project


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	return GetDefaultDesc(FString("FIRE BOLT"), Level);
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	return GetDefaultDesc(FString("NEXT LEVEL :"), Level);
}

void UAuraFireBolt::SawnProjectiles(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch,float PitchOverrideValue, AActor* HomingTarget)
{
	const bool bIServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIServer)return;
	FVector SpawnLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag
		);
		
	FRotator Rotation = ( TargetLocation - SpawnLocation).Rotation();
	if(bOverridePitch)
	{
		Rotation.Pitch = PitchOverrideValue;
	}
	const int32 NumProjectiles = FMath::Min(MaxNumProjectiles,GetAbilityLevel());
	const FVector Forward = Rotation.Vector();
	// const FVector LeftOfSpread =  Forward.RotateAngleAxis(-ProjectileSpread*0.5f, FVector::UpVector);
	// const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread*0.5f, FVector::UpVector);
	
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward,FVector::UpVector,ProjectileSpread,NumProjectiles);


	for(auto Rot : Rotations)
	{
		FTransform SpawnTransform ;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass ,
			SpawnTransform,GetOwningActorFromActorInfo() ,
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Projectile->DamageEffectParams = MakeDamageEffectParams(  );
	
		Projectile->FinishSpawning(SpawnTransform);
	}
	/*
	 * 
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass ,
	SpawnTransform,GetOwningActorFromActorInfo() ,
	Cast<APawn>(GetOwningActorFromActorInfo()),
	ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = MakeDamageEffectParams(  );
	
	Projectile->FinishSpawning(SpawnTransform);
	*/
}

FString UAuraFireBolt::GetDefaultDesc(const FString& Title, const int Level) const
{
	const float ScaledDamageValue = Damage.GetValueAtLevel(Level);
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
			ScaledDamageValue );	
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
		ScaledDamageValue);
}
