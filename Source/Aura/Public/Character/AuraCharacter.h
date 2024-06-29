// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase , public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	/** Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	/** End Combat Interface */
	
	/** Player Interface */
	virtual void AddToXP_Implementation(int32 XP) override;
	virtual void LevelUp_Implementation() override;
	/** End Player Interface */
	
protected:
	virtual  void InitAbilityActorInfo() override;
};
