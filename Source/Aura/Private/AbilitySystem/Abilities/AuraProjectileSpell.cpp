// sakae's gas project


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIServer = HasAuthority(&ActivationInfo);

	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		FTransform SpawnTransform ;
		const FVector SpawnLocation = CombatInterface->GetCombatSocketLocation();
		SpawnTransform.SetLocation(SpawnLocation);
		//TODO 設定 Projectile 的旋轉方向


		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass ,
		SpawnTransform,GetOwningActorFromActorInfo() ,
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		//TODO 建立 Gameplay Effect Spec 作為傷害標記
		Projectile->FinishSpawning(SpawnTransform);
	}

}
