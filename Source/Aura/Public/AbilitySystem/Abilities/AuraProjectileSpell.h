// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraGameAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
class UGameplayEffect;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere , BlueprintReadOnly ,Category = "Projectile")
	UClass* ProjectileClass;
	
	UFUNCTION(BlueprintCallable , Category = "Projectile")
	void SpawnProjectile(const FVector& TargetLocation,const FGameplayTag& SocketTag);// , const FRotator& Rotation);


};
