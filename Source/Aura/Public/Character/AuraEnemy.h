// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewHealth);

class UWidgetComponent;
class AAuraAIController;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase , public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
	// IEnemyInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() override;
	// End IEnemyInterface

	/** CombatInterface */
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	/** End CombatInterface */

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	void HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting;
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatActor;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes()  const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default ")
	int32 Level = 1;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere , Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

};
