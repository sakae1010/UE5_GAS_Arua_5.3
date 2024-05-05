// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"



class UAnimMontage;

// This class does not need to be modified.
UINTERFACE(MinimalAPI , BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetPlayerLevel();
	virtual FVector GetCombatSocketLocation();
	
	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void UpdateWarpTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	UAnimMontage* GetHitReactMontage();
};
