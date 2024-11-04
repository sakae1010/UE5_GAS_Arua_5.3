// sakae's gas project
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include <AuraAbilityTypes.h>
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Abilities/AuraGameAbility.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,FWidgetControllerParams& OutParams , AAuraHUD*& OutAuraHUD)
{
	if( APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD());
		if (OutAuraHUD == nullptr)
		{
			UE_LOG(LogAura, Error, TEXT("AAuraHUD is not set in the BP_AuraHUD"));
			return false;
		}
		AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
		UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
		UAttributeSet* AttributeSet = PlayerState->GetAttributeSet() ;
		// OutParams = FWidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
		OutParams.PlayerController = PlayerController;
		OutParams.PlayerState = PlayerState;
		OutParams.AbilitySystemComponent = AbilitySystemComponent;
		OutParams.AttributeSet = AttributeSet;
		return true;	
	}
	return false;
}


UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Parms ;
	AAuraHUD* AuraHUD = nullptr;
	if(MakeWidgetControllerParams(WorldContextObject , Parms , AuraHUD))
	{
		return AuraHUD-> GetOverlayWidgetController(Parms);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Parms ;
	AAuraHUD* AuraHUD = nullptr;
	if(MakeWidgetControllerParams(WorldContextObject , Parms , AuraHUD))
	{
		return AuraHUD-> GetAttributeMenuWidgetController(Parms);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Parms ;
	AAuraHUD* AuraHUD = nullptr;
	if(MakeWidgetControllerParams(WorldContextObject , Parms , AuraHUD))
	{
		return AuraHUD-> GetSpllMenuWidgetController(Parms);
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
		if(ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			int32 Level = ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor());
			FGameplayAbilitySpec StartupAbilitySpec = FGameplayAbilitySpec(StartupAbilityClass, Level);
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

void UAuraAbilitySystemLibrary::SetIsRadialDamageEffectParams(FDamageEffectParams& DamageEffectParams,	bool bInIsRadialDamage, float InnerRadius, float OuterRadius, const FVector& DamageOrigin)
{
	DamageEffectParams.bIsRadialDamage = bInIsRadialDamage;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = DamageOrigin;
}

void UAuraAbilitySystemLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams, FVector InKnockbackDirection , float Magnitude)
{
	InKnockbackDirection.Normalize();
	if (Magnitude == 0.f)
	{
		Magnitude = DamageEffectParams.KnockbackForceMagnitude;
	}
	DamageEffectParams.KnockbackForce = Magnitude * InKnockbackDirection;
}

void UAuraAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams,FVector InDeathImpulseDirection , float Magnitude)
{
	InDeathImpulseDirection.Normalize();
	if (Magnitude == 0.f)
	{
		Magnitude = DamageEffectParams.DeathImpulseMagnitude;
	}
	DamageEffectParams.DeathImpulseVector = InDeathImpulseDirection * Magnitude;
}

void UAuraAbilitySystemLibrary::SetEffectParmsTargetASC(FDamageEffectParams& DamageEffectParams,	UAbilitySystemComponent* InTargetASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InTargetASC;
}


UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(AuraGameModeBase == nullptr) return nullptr;
	return AuraGameModeBase->CharacterClassInfo;
	
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(AuraGameModeBase == nullptr) return nullptr;
	return AuraGameModeBase->AbilityInfo;
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

bool UAuraAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = AuraEffectContext->GetDamageType();
		return DamageType.IsValid() ? *DamageType : FGameplayTag();
	}
	return FGameplayTag();
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UAuraAbilitySystemLibrary::GetKnokbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetKnokbackForce();
	}
	return FVector::ZeroVector;
}

bool UAuraAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsRadialDamage();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UAuraAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
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

void UAuraAbilitySystemLibrary::SetSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,bool bInSuccessfulDebuff)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast< FAuraGameplayEffectContext*>( EffectContextHandle.Get()))
	{
		AuraEffectContext->SetSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle,float InDebuffDamage)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle,float InDebuffDuration)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle,float InDebuffFrequency)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

void UAuraAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle , const FGameplayTag& InDamageType)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedRef<FGameplayTag> DamageType = MakeShared<FGameplayTag>( InDamageType );
		AuraEffectContext->SetDamageType(DamageType);
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,const FVector& InDeathImpulse)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UAuraAbilitySystemLibrary::SetKnokbackForce(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InKnokbackForce)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetKnokbackForce(InKnokbackForce);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamage(FGameplayEffectContextHandle& EffectContextHandle, bool bInRadialDamage)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsRadialDamage(bInRadialDamage);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,float InRadialDamageInnerRadius)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageInnerRadius( InRadialDamageInnerRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,float InRadialDamageOuterRadius)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageOuterRadius(InRadialDamageOuterRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,const FVector& InRadialDamageOrigin)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageOrigin(InRadialDamageOrigin);
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
		World->OverlapMultiByObjectType(Overlaps,SphereOrigin, FQuat::Identity,FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),FCollisionShape::MakeSphere(Radius),SphereParams);
		for (FOverlapResult& OverlapResult : Overlaps)
		{
			// && OverlapResult.GetActor()->ActorHasTag(FName("Player")) 透過BP過濾
			if (OverlapResult.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(OverlapResult.GetActor() ))
			{
				OutOverLappingActors.AddUnique( ICombatInterface::Execute_GetAvatar(OverlapResult.GetActor()) );
			}
		
		}
	}
	
}

void UAuraAbilitySystemLibrary::GetClosestTargets(int32 MaxNumTargets, const TArray<AActor*>& Actors,TArray<AActor*>& OutOverlappingActors, const FVector& Origin)
{

	if (MaxNumTargets >= Actors.Num())
	{
		OutOverlappingActors = Actors;
		return;
	}
	TArray<AActor*> ActorToCheck = Actors;
	int32 NumTargets = 0;
	while(NumTargets<MaxNumTargets)
	{
		if(ActorToCheck.Num() == 0) break;	
		AActor* ClosestActor = nullptr;
		float MinDistance = TNumericLimits<double>::Max();
		for (AActor* Actor : ActorToCheck)
		{
			const double Distance = (Actor->GetActorLocation() - Origin).Length();
			if(Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestActor = Actor;
			}
		}
		if(ClosestActor)
		{
			OutOverlappingActors.AddUnique(ClosestActor);
			ActorToCheck.Remove(ClosestActor);
			NumTargets++;
		}else
		{
			break;
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

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffectParams(const FDamageEffectParams& DamageEffectParams)
{
	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	const AActor* SourceActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	FGameplayEffectContextHandle ContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(SourceActor);
	SetDeathImpulse( ContextHandle, DamageEffectParams.DeathImpulseVector );
	SetKnokbackForce( ContextHandle, DamageEffectParams.KnockbackForce );
	//範圍傷害應用
	SetRadialDamage( ContextHandle, DamageEffectParams.bIsRadialDamage );
	SetRadialDamageInnerRadius( ContextHandle, DamageEffectParams.RadialDamageInnerRadius );
	SetRadialDamageOuterRadius( ContextHandle, DamageEffectParams.RadialDamageOuterRadius );
	SetRadialDamageOrigin( ContextHandle, DamageEffectParams.RadialDamageOrigin );
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageEffectClass, DamageEffectParams.AbilityLevel, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraGameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraGameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraGameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraGameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	if (DamageEffectParams.TargetAbilitySystemComponent != nullptr)
	{
		DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	}
	//可能不需要使用
	return ContextHandle;
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis , float SpreadAngle , int32 NumRotators)
{

	TArray<FRotator> Rotators;
	const FVector LeftOfSpread =  Forward.RotateAngleAxis(-SpreadAngle*0.5f, Axis);
	if(NumRotators > 1)
	{
		const float Delta = SpreadAngle/(NumRotators-1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(Delta*i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}else
	{
		Rotators.Add(Forward.Rotation());
	}

	return Rotators;

}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlySpacedVectors(const FVector& Forward, const FVector& Axis ,const float SpreadAngle ,const int32 NumVectors)
{
	TArray<FVector> Vectors;
	
	const FVector LeftOfSpread =  Forward.RotateAngleAxis(-SpreadAngle*0.5f, Axis);
	if(NumVectors > 1)
	{
		const float Delta = SpreadAngle/(NumVectors-1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(Delta*i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}else
	{
		Vectors.Add(Forward);
	}

	return Vectors;
}



