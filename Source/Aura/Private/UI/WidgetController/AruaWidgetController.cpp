// sakae's gas project


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InParams)
{
	PlayerController = InParams.PlayerController;
	PlayerState = InParams.PlayerState;
	AbilitySystemComponent = InParams.AbilitySystemComponent;
	AttributeSet = InParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitValues()
{
}

void UAuraWidgetController::BindCallBacksToDependencies()
{
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if(!GetAuraAbilitySystemComponent())return;
	if(!GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven) return;

	FForEachAbility Delegate;
	Delegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)->void
	{
		FAuraAbilityInfo AbilityInfo = AbilityWidgetDataTable->FindAbilityInfForTag(GetAuraAbilitySystemComponent()->GetAbilityFromSpec(AbilitySpec));
		AbilityInfo.InputTag = GetAuraAbilitySystemComponent()->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(AbilityInfo);
	});
	GetAuraAbilitySystemComponent()->ForEachAbility(Delegate);
}

AAuraPlayerController* UAuraWidgetController::GetAuraPlayerController() 
{
	if(AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
	
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraAbilitySystemComponent() 
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAttributeSet() 
{
	if( AuraAttributeSet == nullptr )
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPlayerState() 
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}
