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
		
	const UAbilitySystemComponent* SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = TargetLocation;
	EffectContextHandle.AddHitResult(HitResult);
		
		
	const FGameplayEffectSpecHandle SpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(),EffectContextHandle);
	// const float ScaledValue = Damage.GetValueAtLevel(10);//(GetAbilityLevel());
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const float ScaledValue = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,DamageType, ScaledValue);
	Projectile->DamageEffectSpecHandle = SpecHandle;
	Projectile->FinishSpawning(SpawnTransform);
}