// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "SpllMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API USpllMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitValues() override ;
	virtual void BindCallBacksToDependencies() override ;
};
