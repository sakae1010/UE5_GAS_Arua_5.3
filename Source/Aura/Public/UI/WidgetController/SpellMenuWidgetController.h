// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitValues() override ;
	virtual void BindCallBacksToDependencies() override ;
};
