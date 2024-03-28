// sakae's gas project


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Blueprint/UserWidget.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UUserWidget* Widget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass))
	{
		// OverlayWidget = Cast<UAuraUserWidget>(Widget);
		Widget->AddToViewport();
	}
}
