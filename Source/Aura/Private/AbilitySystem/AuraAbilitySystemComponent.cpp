// sakae's gas project


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTag.h"

void UAuraAbilitySystemComponent::AbilityActorInfoInit()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnGameplayEffectAppliedCallback);

	 
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("AbilityActorInfoInit  %s" ),*FAuraGameplayTags::Get().Armor.ToString()));
}

void UAuraAbilitySystemComponent::OnGameplayEffectAppliedCallback(UAbilitySystemComponent* AbilitySystemComponent,
                                                                  const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	FGameplayTagContainer TagContainer;
	Spec.GetAllAssetTags(TagContainer);
	
	EffectAssetTags.Broadcast(TagContainer);
	
	// for ( const FGameplayTag& Tag : TagContainer)
	// {
	// 	//TODO 廣播 tag 給 Widget Controller 知道
	// 	const FString TagString =FString::Printf(TEXT("Tag: %s"), *Tag.ToString());
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TagString);
	// 	UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), *Tag.ToString());
	// }
}
