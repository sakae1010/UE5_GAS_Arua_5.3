// sakae's gas project


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

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

	const int32 NumAdditionalTargets =  5;//FMath::Min( GetAbilityLevel() -1, MaxNumShockTargets );
	const int32 NumOverlappingActors = FMath::Min( OverlappingActors.Num(), NumAdditionalTargets );
	UAuraAbilitySystemLibrary::GetClosestTargets(
		NumOverlappingActors,
		OverlappingActors,
		OutAdditionalTargets,
		MouseHitActor->GetActorLocation()
		);
}
