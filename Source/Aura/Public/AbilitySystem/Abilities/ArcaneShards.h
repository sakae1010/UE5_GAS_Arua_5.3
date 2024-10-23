// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
	UPROPERTY(EditDefaultsOnly ,BlueprintReadOnly, Category = "ArcaneShards")
	int32 MaxNumShards = 11;
protected:
	virtual FString GetDefaultDesc(const FString& Title, const int Level) const override;
};
