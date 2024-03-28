// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<class UAuraUserWidget> OverlayWidget;
protected:

	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UAuraUserWidget> OverlayWidgetClass;
};
