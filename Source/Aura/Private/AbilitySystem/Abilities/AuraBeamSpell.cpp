// sakae's gas project


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"


FString UAuraBeamSpell::GetDescription(int32 Level)
{
	return GetDefaultDesc(FString("ELECTROCUTE"), Level);
}

FString UAuraBeamSpell::GetNextLevelDescription(int32 Level)
{
	return GetDefaultDesc(FString("ELECTROCUTE"), Level);
}

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if ( HitResult.bBlockingHit )
	{
		MouseHitActor = HitResult.GetActor();
		MouseHitLocation = HitResult.ImpactPoint;
	}else
	{
		CancelAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true );
	}
}

void UAuraBeamSpell::StoreOwnerVariables()
{
	if(!CurrentActorInfo)return;
	OwnerPlayerController = Cast<APlayerController>(CurrentActorInfo->PlayerController.Get());
	OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check( OwnerCharacter );
	FHitResult HitResult = FHitResult();

	if(OwnerCharacter->Implements<UCombatInterface>())
	{
		 if( USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon( OwnerCharacter ) )
		 {
		 	const FVector SocketLocation = Weapon->GetSocketLocation( FName( "TipSocket" ) );
		    TArray<AActor*> ActorsToIgnore = TArray<AActor*>();
		 	ActorsToIgnore.Add( OwnerCharacter );
		 	
		 	UKismetSystemLibrary::SphereTraceSingle(
		 		OwnerCharacter,
		 		SocketLocation,
		 		BeamTargetLocation,
		 		5.f,
		 		ETraceTypeQuery::TraceTypeQuery1,
		 		false,
		 		ActorsToIgnore,
		 		EDrawDebugTrace::None,
		 		HitResult,
		 		true
		 	 );

		 	if( HitResult.bBlockingHit )
		 	{
		 		MouseHitLocation = HitResult.ImpactPoint;
		 		MouseHitActor = HitResult.GetActor();
		 	}
		 }
	}

	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>( MouseHitActor ))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound( this, &UAuraBeamSpell::PrimaryTargetDied ))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic( this, &UAuraBeamSpell::PrimaryTargetDied );
		}
	}

}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add( GetAvatarActorFromActorInfo() );
	ActorsToIgnore.Add( MouseHitActor.Get() );
	TArray<AActor*> OverlappingActors;
	UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius( GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		850.f,
		MouseHitActor->GetActorLocation() );

	const int32 NumAdditionalTargets = FMath::Min( GetAbilityLevel() -1, MaxNumShockTargets );
	const int32 NumOverlappingActors = FMath::Min( OverlappingActors.Num(), NumAdditionalTargets );
	UAuraAbilitySystemLibrary::GetClosestTargets(
		NumOverlappingActors,
		OverlappingActors,
		OutAdditionalTargets,
		MouseHitActor->GetActorLocation()
		);

	for(AActor* AdditionalTarget : OutAdditionalTargets)
	{
		if(ICombatInterface* CombatInterface = Cast<ICombatInterface>( AdditionalTarget ))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound( this, &UAuraBeamSpell::AdditionalTargetDied ))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic( this, &UAuraBeamSpell::AdditionalTargetDied );
			}
		}
	}
}

FString UAuraBeamSpell::GetDefaultDesc(const FString& Title, const int Level) const
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>%s</> \n\n"
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
