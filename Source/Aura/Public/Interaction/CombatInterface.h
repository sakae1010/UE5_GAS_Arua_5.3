// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
struct FGameplayTag;
class UAnimMontage;


//對應 Tag  與 動畫檔案
USTRUCT(Blueprintable)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};


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
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetPlayerLevel();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag MontageTag);
	
	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void UpdateWarpTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	UAnimMontage* GetHitReactMontage();


	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	AActor* GetAvatar() ;

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();
	
	virtual void Die() = 0;

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& Tag);

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	int32 GetMinionCount();
	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	void IncrementMinionCount(int32 Amount);

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	ECharacterClass GetCharacterClass() const;
};
