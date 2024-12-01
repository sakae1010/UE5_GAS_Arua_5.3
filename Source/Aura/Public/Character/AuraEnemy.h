// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HightlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewHealth);

class UWidgetComponent;
class AAuraAIController;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase , public IEnemyInterface ,public IHightlightInterface 
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;

	/* IHightlightInterface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void MoveToLocation_Implementation(FVector& OutLocation) override;
	/* End IHightlightInterface */
	
	/* IEnemyInterface */
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() override;
	/* End IEnemyInterface */

	/** CombatInterface */
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& Impulse) override;
	/** End CombatInterface */

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	void HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting;
	


	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatActor;

	void SetLevel(int32 InLevel) { Level = InLevel; }
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes()  const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default ")
	int32 Level = 1;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere , Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

};
