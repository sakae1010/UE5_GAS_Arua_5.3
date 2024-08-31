// sakae's gas project


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AuraGameAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"


void UAuraAbilitySystemComponent::AbilityActorInfoInit()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}


void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,const FGameplayTag& Slot)
{
	if(FGameplayAbilitySpec* Spec = GetSpecFormAbilityTag(AbilityTag))
	{
		const auto PreSlot = GetInputTagFromSpec(*Spec);
		const auto Status = GetStatusTagFromSpec(*Spec);
		const FAuraGameplayTags AuraGameplayTag = FAuraGameplayTags::Get();
		const auto Status_Unlocked_Tag = AuraGameplayTag.Abilities_Status_Unlocked;
		const auto Status_Equipped_Tag = AuraGameplayTag.Abilities_Status_Equipped;
		const bool bStatusVaild = Status == Status_Equipped_Tag
		                       || Status == Status_Unlocked_Tag;
		if (!bStatusVaild) return;
		//清除任何擁有input(slot) 的 Ability
		ClearAbilityOfSlot(Slot);
		//清除目前擁有的InputTag(slot) 的Ability
		ClearSlot(Spec);
		//Assign ability to  new slot
		Spec->DynamicAbilityTags.AddTag(Slot);
		if (Status.MatchesTagExact(Status_Unlocked_Tag) )
		{
			Spec->DynamicAbilityTags.RemoveTag(Status_Unlocked_Tag);
			Spec->DynamicAbilityTags.AddTag(Status_Equipped_Tag);
		}
		MarkAbilitySpecDirty(*Spec);
		ClientEquipAbility(AbilityTag,Status_Equipped_Tag,Slot,PreSlot);
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if(!IsValid(AbilityInfo) )return;
	for (const FAuraAbilityInfo& Info : AbilityInfo->AuraAbilityInformations )
	{
		FGameplayTag AbilityTag = Info.AbilityTag;
		if(!AbilityTag.IsValid())continue;
		if(Level < Info.LevelRequirement) continue;
		if(GetSpecFormAbilityTag(AbilityTag) == nullptr)
		{
			FGameplayTag StatusTag = FAuraGameplayTags::Get().Abilities_Status_Eligible;
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec (Info.Ability , 1) ;
			AbilitySpec.DynamicAbilityTags.AddTag(StatusTag);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(AbilityTag , StatusTag , 1);
		}
	}
	
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	FGameplayAbilitySpec* AbilitySpec = GetSpecFormAbilityTag(AbilityTag);
	if (AbilitySpec == nullptr)
	{
		return;
	}
	if(GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(),-1);
	}
	FGameplayTag StatusTag = GetStatusTagFromSpec(*AbilitySpec);
	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	//可以解鎖的狀態
	if(StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Eligible))
	{
		AbilitySpec->DynamicAbilityTags.RemoveTag(AuraGameplayTags.Abilities_Status_Eligible);
		AbilitySpec->DynamicAbilityTags.AddTag(AuraGameplayTags.Abilities_Status_Unlocked);
		StatusTag = AuraGameplayTags.Abilities_Status_Unlocked;
	
	}// 已經解索 或 正在裝備中
	else if (StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Unlocked)||StatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Equipped))
	{
		AbilitySpec->Level +=1;
	}
	
	ClientUpdateAbilityStatus(AbilityTag , StatusTag , AbilitySpec->Level );
	MarkAbilitySpecDirty(*AbilitySpec);
	
	
	
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>>& InAbilities)
{
	for (const TSubclassOf<UGameplayAbility> Ability : InAbilities)
	{
		if (!Ability)continue;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		const FGameplayTag EquipTag = FAuraGameplayTags::Get().Abilities_Status_Equipped;
		if(const UAuraGameAbility* AuraAbility = Cast<UAuraGameAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(EquipTag);
			GiveAbility(AbilitySpec);
		}		
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassvieAbilities(TArray<TSubclassOf<UGameplayAbility>>& InPassvieAbilities)
{
	for (const TSubclassOf<UGameplayAbility> Ability : InPassvieAbilities)
	{
		if (!Ability)continue;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())return;
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))continue;
		AbilitySpecInputPressed(AbilitySpec);
		if(!AbilitySpec.IsActive()) continue;
		InvokeReplicatedEvent( EAbilityGenericReplicatedEvent::InputPressed ,AbilitySpec.Handle ,AbilitySpec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UAuraAbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())return;
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))continue;
		AbilitySpecInputPressed(AbilitySpec);
		if(AbilitySpec.IsActive())continue;
		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UAuraAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())return;
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent( EAbilityGenericReplicatedEvent::InputReleased ,AbilitySpec.Handle ,AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock AbilityLock(*this);
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("ForEachAbility Delegate failed [%hs]"), __FUNCTION__ );
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if(AbilitySpec.Ability)
	{
		for (const FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		
		}
	}
	UE_LOG(LogAura, Warning, TEXT("GetAbilityFromSpec failed [%hs]"), __FUNCTION__ );
	return FGameplayTag::EmptyTag;
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
		
	}
	UE_LOG(LogAura, Error, TEXT("GetAbilityFromSpec failed [%hs]"), __FUNCTION__ );
	return FGameplayTag::EmptyTag;
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if(StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	UE_LOG(LogAura, Error, TEXT("GetStatusTagFromSpec failed [%hs]"), __FUNCTION__ );
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFormAbilityTag(AbilityTag))
	{
		return GetStatusTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFormAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFormAbilityTag(const FGameplayTag& AbilityTag)
{
	//確保還在跑loop時 不會被修改資料
	FScopedAbilityListLock AbilityLock(*this);

	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for(FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if(Tag.MatchesTag(AbilityTag)) return &AbilitySpec;
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& Tag)
{
	if(!Tag.IsValid()) return;
	if(GetAvatarActor()->Implements<UPlayerInterface>())
	{
		int32 AttrPoint = IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor());
		if (AttrPoint == 0 )return;
		ServerUpgradeAttribute_Implementation(Tag);
	}
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventMagnitude = 1;
	Payload.EventTag = AttributeTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	if(GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void UAuraAbilitySystemComponent::ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquipedDelegate.Broadcast(AbilityTag , StatusTag , Slot , PreviousSlot);
}

bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString&  OutDescription,	FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFormAbilityTag(AbilityTag))
	{
		if(UAuraGameAbility* AuraGameAbility = Cast<UAuraGameAbility>( AbilitySpec->Ability ))
		{
			OutDescription = AuraGameAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraGameAbility->GetNextLevelDescription(AbilitySpec->Level + 1 );
		}
		return true;
	}
	if(!AbilityTag.IsValid() || AbilityTag == FAuraGameplayTags::Get().Abilities_Type_None)
	{
		OutDescription = FString();
	}else
	{
		const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
		OutDescription = UAuraGameAbility::GetLoackedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	}

	OutNextLevelDescription = FString();
	return false;
}

void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag SlotTag = GetInputTagFromSpec(*AbilitySpec);
	AbilitySpec->DynamicAbilityTags.RemoveTag(SlotTag);
	MarkAbilitySpecDirty(*AbilitySpec);
}

void UAuraAbilitySystemComponent::ClearAbilityOfSlot(const FGameplayTag& Slot)
{
	//確保還在跑loop時 不會被修改資料
	FScopedAbilityListLock AbilityLock(*this);
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilityHasSlot(&AbilitySpec , Slot))
		{
			ClearSlot(&AbilitySpec);
		}
	}
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& Slot)
{
	for (FGameplayTag Tag : AbilitySpec->DynamicAbilityTags )
	{
		if(Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if(bStartupAbilitiesGiven)return;
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag ,int32 AbiltyLevel)
{
	AbilityStatusChangedDelegate.Broadcast(AbilityTag,StatusTag , AbiltyLevel);
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle) const
{
	FGameplayTagContainer TagContainer;
	Spec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}

