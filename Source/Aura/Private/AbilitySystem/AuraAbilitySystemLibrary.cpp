// sakae's gas project


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Abilities/AuraGameAbility.h"
#include "Game/AuraGameModeBase.h"
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

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;
	for(const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		if (!AbilityClass)continue;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(AuraGameModeBase == nullptr) return nullptr;
	return AuraGameModeBase->CharacterClassInfo;
	
}
