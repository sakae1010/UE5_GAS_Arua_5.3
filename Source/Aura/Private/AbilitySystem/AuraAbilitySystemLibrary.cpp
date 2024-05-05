// sakae's gas project


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystem/AuraAttributeSet.h"
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
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(AuraGameModeBase == nullptr) return;

	AActor* Actor = ASC->GetAvatarActor();
	
	UCharacterClassInfo* CharacterClassInfo = AuraGameModeBase->CharacterClassInfo;
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
