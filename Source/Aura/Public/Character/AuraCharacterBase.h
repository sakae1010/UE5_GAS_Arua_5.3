#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;
class UGameplayEffect;
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere , Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>  AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	virtual	void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere , Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere , Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributeEffect;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass , float Level) const;
	void InitializeDefaultAttributes() const;
};
