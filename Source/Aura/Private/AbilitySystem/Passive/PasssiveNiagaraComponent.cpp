// sakae's gas project


#include "AbilitySystem/Passive/PasssiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPasssiveNiagaraComponent::UPasssiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPasssiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	// ;
	if ( UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent> (UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( GetOwner() ) ) ; ASC != nullptr )
	{
		ASC->ActivePassiveEffectDelegate.AddUObject( this, &UPasssiveNiagaraComponent::OnPassiveActiveate );
	}
	else if(ICombatInterface* CombatInterface = Cast<ICombatInterface>( GetOwner() ); CombatInterface != nullptr )
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda( [this](UAbilitySystemComponent* InASC)
		{
			if ( UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(InASC) ; ASC != nullptr )
			{
				ASC->ActivePassiveEffectDelegate.AddUObject( this, &UPasssiveNiagaraComponent::OnPassiveActiveate );
			}
		});
	}
	{
		
	}
}

void UPasssiveNiagaraComponent::OnPassiveActiveate(const FGameplayTag& GameplayTag, bool bActiveate)
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
