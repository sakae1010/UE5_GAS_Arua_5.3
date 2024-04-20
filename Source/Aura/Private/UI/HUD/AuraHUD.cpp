// sakae's gas project


#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/AttributeMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallBacksToDependencies();
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlayWidgetController(APlayerController* InPlayerController, APlayerState* InPlayerState,
	UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
{
	checkf(OverlayWidgetClass , TEXT("OverlayWidgetClass is not set , please set it in the BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass , TEXT("OverlayWidgetControllerClass is not set , please set it in the BP_AuraHUD"));
	
	if (UUserWidget* Widget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass))
	{
		OverlayWidget = Cast<UAuraUserWidget>(Widget);
		const FWidgetControllerParams Params(InPlayerController, InPlayerState, InAbilitySystemComponent, InAttributeSet);
		
		UOverlayWidgetController* WidgetController = GetOverlayWidgetController(Params);
		
		OverlayWidget->SetWidgetController(WidgetController);
		WidgetController->BroadcastInitValues();
		
		OverlayWidget->AddToViewport();
		
	}
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& Params)
{
	if(AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this,AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(Params);
		AttributeMenuWidgetController->BindCallBacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

