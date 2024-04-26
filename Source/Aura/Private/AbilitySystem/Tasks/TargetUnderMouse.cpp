// sakae's gas project


#include "AbilitySystem/Tasks/TargetUnderMouse.h"

#include "Kismet/GameplayStatics.h"


UTargetUnderMouse* UTargetUnderMouse::CreateTargetUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetUnderMouse* MyObj = NewAbilityTask<UTargetUnderMouse>(OwningAbility);

	return MyObj;
}


void UTargetUnderMouse::Activate()
{

	APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	// UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	ValidData.Broadcast(CursorHit.Location);
}
