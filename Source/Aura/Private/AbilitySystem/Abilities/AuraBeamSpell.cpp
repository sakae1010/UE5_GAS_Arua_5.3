// sakae's gas project


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "GameFramework/Character.h"

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
