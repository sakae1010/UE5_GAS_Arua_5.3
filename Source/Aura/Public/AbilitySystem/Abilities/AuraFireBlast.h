// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

class AAuraFireBall;
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

	UFUNCTION(BlueprintCallable, Category = "FireBlast")
	TArray<AAuraFireBall*> SpawnFireBalls();
protected:
	virtual FString GetDefaultDesc(const FString& Title, const int Level) const override;

	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NewFireBalls = 12;

private:
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	TSubclassOf<AAuraFireBall> FireBallClass;
};
