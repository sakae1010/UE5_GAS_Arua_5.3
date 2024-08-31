// sakae's gas project


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

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

void UAuraBeamSpell::SetOwnerPlayerController()
{
	if(CurrentActorInfo)
	{
		OwnerPlayerController = Cast<APlayerController>(CurrentActorInfo->PlayerController.Get());
	}
	
}
