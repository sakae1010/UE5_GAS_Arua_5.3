// sakae's gas project


#include "AbilitySystem/Abilities/AuraPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility( Handle , ActorInfo , ActivationInfo , TriggerEventData );

	if ( UAuraAbilitySystemComponent* const AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( GetAvatarActorFromActorInfo() )) )
	{
		AuraASC->DeactivePassiveAbilityDelegate.AddUObject(this, &UAuraPassiveAbility::ReceiveDeactive);
	}
}

void UAuraPassiveAbility::ReceiveDeactive(const FGameplayTag& AbilityTag)
{
	if ( AbilityTags.HasTagExact( AbilityTag ) )
	{
		EndAbility( CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , true , true );
	}
}
