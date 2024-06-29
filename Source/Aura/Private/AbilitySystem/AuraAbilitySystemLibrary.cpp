﻿// sakae's gas project


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include <AuraAbilityTypes.h>

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Abilities/AuraGameAbility.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
    if( APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
	    if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
	    {
	    	AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
	    	UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	    	UAttributeSet* AttributeSet = PlayerState->GetAttributeSet() ;
	    	const FWidgetControllerParams Params(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	    	return AuraHUD->GetOverlayWidgetController(Params);
	    }
    }
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if( APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
	    if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
	    {
	    	AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
	    	UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	    	UAttributeSet* AttributeSet = PlayerState->GetAttributeSet() ;
	    	const FWidgetControllerParams Params(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	    	return AuraHUD->GetAttributeMenuWidgetController(Params);
	    }
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject ,ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;
	AActor* Actor = ASC->GetAvatarActor();

	FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassInfo->GetCharacterClassInfo(CharacterClass);


	FGameplayEffectContextHandle PrimaryAttributeHandleHandle = ASC->MakeEffectContext();
	PrimaryAttributeHandleHandle.AddSourceObject(Actor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassDefaultInfo.PrimaryAttribute, Level, PrimaryAttributeHandleHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle SeconderAttributeHandleHandle = ASC->MakeEffectContext();
	SeconderAttributeHandleHandle.AddSourceObject(Actor);
	const FGameplayEffectSpecHandle SeconderAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SeconderAttributes, Level, SeconderAttributeHandleHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SeconderAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitaAttributeHandleHandle = ASC->MakeEffectContext();
	VitaAttributeHandleHandle.AddSourceObject(Actor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitaAttributeHandleHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC , ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;
	for(const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		if (!AbilityClass)continue;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetCharacterClassInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> StartupAbilityClass : DefaultInfo.StartupAbilities)
	{
		if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec StartupAbilitySpec = FGameplayAbilitySpec(StartupAbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(StartupAbilitySpec);
		}
	}
}

int32 UAuraAbilitySystemLibrary::GetXpRewardForCharacterClassAndLevel(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return 0;
	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetCharacterClassInfo(CharacterClass);
	const float XPReward = Info.Xp_Reward.GetValueAtLevel(CharacterLevel);
	return static_cast<int32>(XPReward);
}


UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(AuraGameModeBase == nullptr) return nullptr;
	return AuraGameModeBase->CharacterClassInfo;
	
}


bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInBlockedHit)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetBlockedHit(bInBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInCriticalHit)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetCriticalHit(bInCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(
	const UObject* WorldContextObject,
	TArray<AActor*>& OutOverLappingActors,
	const TArray<AActor*>& ActorsToIgnore,
	float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& OverlapResult : Overlaps)
		{
			// && OverlapResult.GetActor()->ActorHasTag(FName("Player")) 透過BP過濾
			if (OverlapResult.GetActor()->Implements<UCombatInterface>()&& !ICombatInterface::Execute_IsDead(OverlapResult.GetActor() ))
			{
					OutOverLappingActors.AddUnique( ICombatInterface::Execute_GetAvatar(OverlapResult.GetActor()) );
			}
		
		}
	}
	
}

bool UAuraAbilitySystemLibrary::IsNotFriend(const AActor* FirstOwner,const AActor* SecondTarget)
{
	const FName PlayerTag = FName("Player");
	const bool bBothArePlayer = FirstOwner->ActorHasTag(PlayerTag)  &&  SecondTarget->ActorHasTag(PlayerTag);
	const FName EnemyTag = FName("Enemy");
	const bool bBothAreEnemy = FirstOwner->ActorHasTag(EnemyTag)  &&  SecondTarget->ActorHasTag(EnemyTag);
	const bool bIsFriend = bBothArePlayer || bBothAreEnemy;
	return !bIsFriend;
}


