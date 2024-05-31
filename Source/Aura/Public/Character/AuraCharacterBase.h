#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AuraCharacterBase.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;
class UGameplayEffect;

class UMaterialInterface;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter , public IAbilitySystemInterface , public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/** CombatInterface */
	//推撥出去
	virtual void Die() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag MontageTag) override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	/**End CombatInterface */


	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere , Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	UPROPERTY(EditAnywhere , Category = "Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere , Category = "Combat")
	FName LeftHandSocketName;
	
	UPROPERTY(EditAnywhere , Category = "Combat")
	FName RightHandSocketName;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>  AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	virtual	void InitAbilityActorInfo();

	bool bDead = false;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere , Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere , Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributeEffect;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere , Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributeEffect;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass , float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities();

	/*溶解效果 Dissolve Effects vvvvv */
	void Dissolve();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* MaterialInterface);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* MaterialInterface);
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Effects")
	TObjectPtr<UMaterialInterface> DissolveMaterialInterface;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Effects")
	TObjectPtr<UMaterialInterface> WeaponDissolveMaterialInterface;
	/*溶解效果 Dissolve Effects ^^^^^ */
private:
	UPROPERTY(EditAnywhere , Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility> > StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
