// sakae's gas project


#include "Actor/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsValidOverlap(OtherActor))
	{
		return;
	}

	if(HasAuthority())
	{
		if(UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const auto DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulseVector = DeathImpulse;
			DamageEffectParams.TargetAbilitySystemComponent = TargetAbilitySystemComponent;
			UAuraAbilitySystemLibrary::ApplyDamageEffectParams(DamageEffectParams);
		}
		
	}
	
}
