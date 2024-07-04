// sakae's gas project


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	check(AttributeInfo);
	for (const auto Pair : GetAuraAttributeSet()->TagsToAttributeMap)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	OnAttributePointsChanged.Broadcast(GetAuraPlayerState()->GetAttributePoints());
		
	// FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfo(FAuraGameplayTags::Get().AuraAttribute_Primary_Strength);
	// Info.AttributeValue = AuraAttributeSet->GetStrength();
	// AttributeInfoDelegate.Broadcast(Info);
	
}

void UAttributeMenuWidgetController::BindCallBacksToDependencies()
{
	for (const auto Pair : GetAuraAttributeSet()->TagsToAttributeMap)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value())
		.AddLambda([this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		});
	}

	GetAuraPlayerState()->OnAttributePointsChangedDelegate.AddLambda([this](const int32 AttriblePoints)
	{
		OnAttributePointsChanged.Broadcast(AttriblePoints);
	});
		
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& Tag)
{
	GetAuraAbilitySystemComponent()->UpgradeAttribute(Tag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag,
                                                            const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfo(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

