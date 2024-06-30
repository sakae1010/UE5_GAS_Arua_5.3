// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, int32 /* STatValue */);


/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditDefaultsOnly, Category = "LevelUpInfo")
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
	FOnPlayerStateChanged OnXPChangedDelegate;
	FOnPlayerStateChanged OnLevelChangedDelegate;
	FOnPlayerStateChanged OnAttributePointsChangedDelegate;
	FOnPlayerStateChanged OnSpellPointsChangedDelegate;
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	
	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);
	void SetAttriblePoints(int32 InAttriblePoints);
	void SetSpellPoints(int32 InSpellPoints);
	
	void AddXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InAttributePoints);
	void AddToSpellPoints(int32 InSpellPoints);
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
private:
	UPROPERTY(VisibleAnywhere , ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere , ReplicatedUsing = OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere , ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere , ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 0;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttriblePoints);

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
	
};
