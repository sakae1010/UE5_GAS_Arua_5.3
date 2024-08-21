// sakae's gas project


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "Kismet/KismetSystemLibrary.h"
// #include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

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
	FTransform SpawnTransform ;
	FVector SpawnLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag
		);
		
	FRotator Rotation = ( TargetLocation - SpawnLocation).Rotation();
	if(bOverridePitch)
	{
		Rotation.Pitch = PitchOverrideValue;
	}
	const int32 NumProjectiles = MaxNumProjectiles;// FMath::Min(MaxNumProjectiles,GetAbilityLevel());
	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread =  Forward.RotateAngleAxis(-ProjectileSpread*0.5f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread*0.5f, FVector::UpVector);

	if(NumProjectiles>1)
	{
		const float Delta = ProjectileSpread/(NumProjectiles-1);
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(Delta*i, FVector::UpVector);
			const FVector Start = SpawnLocation+FVector(0,0,10);
			
			UKismetSystemLibrary::DrawDebugLine(GetAvatarActorFromActorInfo(),Start,Start+Direction*75.f,FLinearColor::Red,5.f,1.f);

			// SpawnTransform.SetLocation(SpawnLocation);
			// SpawnTransform.SetRotation(NewRotation.Quaternion());
			//
			// AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass ,
			// SpawnTransform,GetOwningActorFromActorInfo() ,
			// Cast<APawn>(GetOwningActorFromActorInfo()),
			// ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			//
			// Projectile->DamageEffectParams = MakeDamageEffectParams(  );
			// Projectile->HomingTarget = HomingTarget;
			// Projectile->FinishSpawning(SpawnTransform);
		}
	}
	else
	{
		const FVector Start = SpawnLocation+FVector(0,0,10);
		UKismetSystemLibrary::DrawDebugLine(GetAvatarActorFromActorInfo(),Start,Start+Forward*75.f,FLinearColor::Red,5.f,1.f);

	}

	UKismetSystemLibrary::DrawDebugLine(GetAvatarActorFromActorInfo(),SpawnLocation,SpawnLocation+Forward*100.f,FLinearColor::Green,5.f,1.f);
	UKismetSystemLibrary::DrawDebugLine(GetAvatarActorFromActorInfo(),SpawnLocation,SpawnLocation+LeftOfSpread*100.f,FLinearColor::Green,5.f,1.f);
	UKismetSystemLibrary::DrawDebugLine(GetAvatarActorFromActorInfo(),SpawnLocation,SpawnLocation+RightOfSpread*100.f,FLinearColor::Green,5.f,1.f);

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
