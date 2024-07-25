// sakae's gas project
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>( GetOwner() );
	if ( UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( GetOwner() ) ; ASC != nullptr )
	{
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject( this, &UDebuffNiagaraComponent::DebuffTagChanged );
	}else if ( CombatInterface )
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda( this, [this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject( this, &UDebuffNiagaraComponent::DebuffTagChanged );
		});
	}
	if ( CombatInterface )
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic( this, &UDebuffNiagaraComponent::OnOwnerDeath );
	}

}

void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	NewCount>0 ? Activate() : Deactivate();
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
