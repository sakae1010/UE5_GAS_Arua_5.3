// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PasssiveNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UPassiveNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;

protected:
	virtual void BeginPlay() override;
	void OnPassiveActiveate(const FGameplayTag& GameplayTag,bool bActiveate);
	// virtual void Activate(bool bReset) override;
	// virtual void Deactivate() override;
};
