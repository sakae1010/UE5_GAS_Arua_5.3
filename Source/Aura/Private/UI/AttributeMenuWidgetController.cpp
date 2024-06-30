// sakae's gas project


#include "UI/AttributeMenuWidgetController.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo)
	for (const auto Pair : AuraAttributeSet->TagsToAttributeMap)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(this->PlayerState);
	OnAttributePointsChanged.Broadcast(AuraPlayerState->GetAttributePoints());
		
	// FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfo(FAuraGameplayTags::Get().AuraAttribute_Primary_Strength);
	// Info.AttributeValue = AuraAttributeSet->GetStrength();
	// AttributeInfoDelegate.Broadcast(Info);
	
}

void UAttributeMenuWidgetController::BindCallBacksToDependencies()
{
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo)
	for (const auto Pair : AuraAttributeSet->TagsToAttributeMap)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value())
		.AddLambda([this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		});
	}


	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(this->PlayerState);
	
	AuraPlayerState->OnAttributePointsChangedDelegate.AddLambda([this](const int32 AttriblePoints)
	{
		OnAttributePointsChanged.Broadcast(AttriblePoints);
	});
		
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfo(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

