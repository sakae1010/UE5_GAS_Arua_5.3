// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName  PlayerStartTag = FName ();
	
	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;
};
