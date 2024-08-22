// sakae's gas project


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile( const FVector& TargetLocation , const FGameplayTag& SocketTag ,bool bOverridePitch  , float PitchOverrideValue)
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
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass ,
	SpawnTransform,GetOwningActorFromActorInfo() ,
	Cast<APawn>(GetOwningActorFromActorInfo()),
	ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = MakeDamageEffectParams(  );
	
	Projectile->FinishSpawning(SpawnTransform);
}