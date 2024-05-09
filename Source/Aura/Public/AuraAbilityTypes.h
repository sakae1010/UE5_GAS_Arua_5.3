// sakae's gas project
#pragma once

#include "AuraAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetCriticalHit(bool bInIsCriticalHit ) { bIsCriticalHit = bInIsCriticalHit; }
	
	void SetBlockedHit(bool bInIsBlockedHit ) { bIsBlockedHit = bInIsBlockedHit; }
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayEffectContext::StaticStruct();
	}
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
	
protected:
	UPROPERTY()
	bool bIsCriticalHit = false;
	UPROPERTY()
	bool bIsBlockedHit = false;
};
