// sakae's gas project


#include "AbilitySystem/Passive/PasssiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	// ;
	if ( UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent> (UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( GetOwner() ) ) ; ASC != nullptr )
	{
		ASC->ActivePassiveEffectDelegate.AddUObject( this, &UPassiveNiagaraComponent::OnPassiveActiveate );
	}
	else if(ICombatInterface* CombatInterface = Cast<ICombatInterface>( GetOwner() ); CombatInterface != nullptr )
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda( [this](UAbilitySystemComponent* InASC)
		{
			if ( UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(InASC) ; ASC != nullptr )
			{
				ASC->ActivePassiveEffectDelegate.AddUObject( this, &UPassiveNiagaraComponent::OnPassiveActiveate );
			}
		});
	}
	{
		
	}
}

void UPassiveNiagaraComponent::OnPassiveActiveate(const FGameplayTag& GameplayTag, bool bActiveate)
{
	if ( GameplayTag.MatchesTagExact( PassiveSpellTag ) )
	{
		if ( bActiveate && !IsActive() )
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}
