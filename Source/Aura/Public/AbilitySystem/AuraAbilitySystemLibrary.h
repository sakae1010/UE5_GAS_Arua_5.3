// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAbilitySystemLibrary.generated.h"


struct FDamageEffectParams;
class AAuraHUD;
class USpellMenuWidgetController;
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
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject , FWidgetControllerParams& OutParams , AAuraHUD*& OutAuraHUD);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController" , meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController" , meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController" , meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClass")
	static void InitializeDefaultAttributes (const UObject* WorldContextObject, ECharacterClass CharacterClass , float Level , UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClass")
	static void GiveStartupAbilities (const UObject* WorldContextObject, UAbilitySystemComponent* ASC ,ECharacterClass CharacterClass );
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassInfo")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject); 

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject); 
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle); 

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)  ;
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle) ;
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle) ;
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle) ;

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle) ;
	
	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetBlockedHit(UPARAM( ref ) FGameplayEffectContextHandle& EffectContextHandle, bool bInBlockedHit);

	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetCriticalHit(UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, bool bInCriticalHit);


	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetSuccessfulDebuff(UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle,bool bInSuccessfulDebuff);

	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetDebuffDamage(UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);
	
	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetDebuffDuration(UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle,float InDebuffDuration);

	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetDebuffFrequency(UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency);

	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetDamageType(UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetDeathImpulse(UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject,TArray<AActor*>& OutOverLappingActors, const TArray<AActor*>& ActorsToIgnore , float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend	(const AActor* FirstOwner ,const AActor* SecondTarget);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffectParams")
	static FGameplayEffectContextHandle ApplyDamageEffectParams(const FDamageEffectParams& DamageEffectParams);
	
	static int32 GetXpRewardForCharacterClassAndLevel(const UObject* WorldContextObject,ECharacterClass CharacterClass , int CharacterLevel);
};
