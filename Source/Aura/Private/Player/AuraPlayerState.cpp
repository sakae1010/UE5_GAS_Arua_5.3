// sakae's gas project


#include "..\..\Public\Player\AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"


AAuraPlayerState::AAuraPlayerState()
{
	//通常是 100
	NetUpdateFrequency = 100.0f;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	/*
	 * ReplicationMode  UseCase Description  
	 * Full UseCase : SinglePlayer                     ReplicationMode: Replicate all GameplayEffects to all players.
	 * Mixed UseCase : Multiplayer , Player-Controlled ReplicationMode: Replicate all GameplayEffects to the owning player and only replicate non-local GameplayEffects to other players.
	 * Minimal UseCase : Multiplayer , AI-Controlled   ReplicationMode: Replicate only non-local GameplayEffects to other players.
	 */
	
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuraPlayerState, Level);
	
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	
}
