﻿// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraGameAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere , BlueprintReadOnly ,Category = "Projectile")
	UClass* ProjectileClass;
	
	UFUNCTION(BlueprintCallable , Category = "Projectile")
	void SpawnProjectile(const FVector& TargetLocation);// , const FRotator& Rotation);

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Projectile")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

};
