// sakae's gas project


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
    if( APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
	    if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
	    {
	    	AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
	    	UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	    	UAttributeSet* AttributeSet = PlayerState->GetAttributeSet() ;
	    	const FWidgetControllerParams Params(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	    	return  AuraHUD->GetOverlayWidgetController(Params);
	    }
    }
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if( APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
	    if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
	    {
	    	AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
	    	UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	    	UAttributeSet* AttributeSet = PlayerState->GetAttributeSet() ;
	    	const FWidgetControllerParams Params(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	    	return  AuraHUD->GetAttributeMenuWidgetController(Params);
	    }
	}
	return nullptr;
}
