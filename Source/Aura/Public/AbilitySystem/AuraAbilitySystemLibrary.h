// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"


class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClass")
	static void InitializeDefaultAttributes (const UObject* WorldContextObject, ECharacterClass CharacterClass , float Level , UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClass")
	static void GiveStartupAbilities (const UObject* WorldContextObject, UAbilitySystemComponent* ASC ,ECharacterClass CharacterClass );
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassInfo")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject); 

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle); 

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|EffectContext")
	static void SetBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle , bool bInBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|EffectContext")
	static void SetCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle , bool bInCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject,TArray<AActor*>& OutOverLappingActors, const TArray<AActor*>& ActorsToIgnore , float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend	(AActor* FirstOwner , AActor* SecondTarget);
};
