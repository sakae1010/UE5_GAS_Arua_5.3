// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAbilitySystemLibrary.generated.h"


class ULoadScreenSaveGame;
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

	/*
	 *  UI元件使用
	 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Ability")
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject , FWidgetControllerParams& OutParams , AAuraHUD*& OutAuraHUD);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController" , meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController" , meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController" , meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	/*
	 *  能力系統 ability system class default
	 */
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClass")
	static void InitializeDefaultAttributes (const UObject* WorldContextObject, ECharacterClass CharacterClass , float Level , UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClass")
	static void InitializeDefaultAttributesFormSaveData (const UObject* WorldContextObject, UAbilitySystemComponent* ASC ,ULoadScreenSaveGame* LoadData);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClass")
	static void GiveStartupAbilities (const UObject* WorldContextObject, UAbilitySystemComponent* ASC ,ECharacterClass CharacterClass );
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassInfo")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject); 

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject); 


	/*
	 *  Effect Context  Getter
	 */
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
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static FVector GetKnokbackForce(const FGameplayEffectContextHandle& EffectContextHandle) ;

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|EffectContext")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);
	/*
	 * Effect Context Setter
	 */
	
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
	
	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetKnokbackForce(UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnokbackForce);

	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetRadialDamage(UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, bool bInRadialDamage);

	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetRadialDamageInnerRadius(UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageInnerRadius);

	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetRadialDamageOuterRadius(UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageOuterRadius);

	UFUNCTION( BlueprintCallable , Category = "AuraAbilitySystemLibrary|EffectContext" )
	static void SetRadialDamageOrigin(UPARAM( ref )FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRadialDamageOrigin);

	
	/*
	 * Gameplay Mechanics 遊戲機制
	 */
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject,TArray<AActor*>& OutOverLappingActors, const TArray<AActor*>& ActorsToIgnore , float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetClosestTargets(int32 MaxNumTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutOverlappingActors , const FVector& Origin);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend	(const AActor* FirstOwner ,const AActor* SecondTarget);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffectParams")
	static FGameplayEffectContextHandle ApplyDamageEffectParams(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis ,float SpreadAngle , int32 NumRotators);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> EvenlySpacedVectors(const FVector& StartVector, const FVector& Axis ,float SpreadAngle ,int32 NumVectors);
	
	static int32 GetXpRewardForCharacterClassAndLevel(const UObject* WorldContextObject,ECharacterClass CharacterClass , int CharacterLevel);

	/**
	 *  Damage Effect Params
	 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffectParams")
	static void SetIsRadialDamageEffectParams(UPARAM( ref )FDamageEffectParams& DamageEffectParams, bool bInIsRadialDamage ,float InnerRadius,float OuterRadius,const FVector& DamageOrigin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffectParams")
	static void SetKnockbackDirection(UPARAM( ref )FDamageEffectParams& DamageEffectParams, FVector InKnockbackDirection , float Magnitude = 0.f);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffectParams")
	static void SetDeathImpulseDirection(UPARAM( ref )FDamageEffectParams& DamageEffectParams, FVector InDeathImpulseDirection , float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffectParams")
	static void SetEffectParmsTargetASC(UPARAM( ref )FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InTargetASC);
};


