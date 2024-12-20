#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AuraCharacterBase.generated.h"



class UPassiveNiagaraComponent;
class UDebuffNiagaraComponent;
class UNiagaraSystem;
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
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/** CombatInterface */
	//推撥出去
	virtual void Die(const FVector& Impulse) override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag MontageTag) override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& Tag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() const override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() const override;
	virtual bool IsBeingShocked_Implementation() const override;
	virtual void SetIsBeingShocked_Implementation(bool bInShockLoop) override;
	virtual FOnDamageSignature& GetOnDamageSignature() override;
	
	/**End CombatInterface */
	FOnASCRegistered OnAscRegisteredDelegate;
	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& Impulse);

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;



	UPROPERTY(ReplicatedUsing = OnRep_Stunned, BlueprintReadOnly, Category = "Combat")
	bool bIsStunned = false;
	UPROPERTY(ReplicatedUsing = OnRep_Burned, BlueprintReadOnly, Category = "Combat")
	bool bIsBurned = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bIsBeingShocked = false;
	
	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();

	void SetChracterClass(ECharacterClass InCharacterClass) { CharacterClass = InCharacterClass; }
	
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere , BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;
		
	UPROPERTY(EditAnywhere,BlueprintReadOnly , Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	UPROPERTY(EditAnywhere , Category = "Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere , Category = "Combat")
	FName LeftHandSocketName;
	
	UPROPERTY(EditAnywhere , Category = "Combat")
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere , Category = "Combat")
	FName TileSocketName;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>  AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	virtual	void InitAbilityActorInfo();
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	//開放出去是為了讓 死亡時動畫不再撥放
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
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

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Effects")
	UNiagaraSystem* BloodEffect;
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Sound")
	USoundBase* DeadSound;

	/*  Minions   */

	int32 MinionCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default ")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffNiagaraComponent;


	
private:
	UPROPERTY(EditAnywhere , Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility> > StartupAbilities;

	UPROPERTY(EditAnywhere , Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility> > StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiponNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiponNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;

};
