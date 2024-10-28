// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
protected:
	virtual FString GetDefaultDesc(const FString& Title, const int Level) const override;

	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NewFireBalls = 12;
};
