// sakae's gas project


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Aura/Public/AuraGameplayTag.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile( const FVector& TargetLocation )
{
	const bool bIServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIServer)return;
	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		
		FTransform SpawnTransform ;
		FVector SpawnLocation = CombatInterface->GetCombatSocketLocation();
		
		FRotator Rotation = ( TargetLocation - SpawnLocation).Rotation();
		Rotation.Pitch = 0.f;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass ,
		SpawnTransform,GetOwningActorFromActorInfo() ,
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		const UAbilitySystemComponent* SourceAbilitySystemComponent =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		
		const FGameplayEffectSpecHandle SpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec(
			DamageEffectClass, GetAbilityLevel(), SourceAbilitySystemComponent->MakeEffectContext());
		FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
		// 5.3 要 GetTagName()
		UAbilitySystemBlueprintLibrary::AssignSetByCallerMagnitude(SpecHandle , AuraGameplayTags.Damage.GetTagName() , 50.f);
        Projectile->DamageEffectSpecHandle = SpecHandle;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
