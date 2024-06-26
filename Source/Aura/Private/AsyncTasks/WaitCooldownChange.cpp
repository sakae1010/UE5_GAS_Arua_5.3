// sakae's gas project


#include "AsyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "ActiveGameplayEffectHandle.h"

UWaitCooldownChange* UWaitCooldownChange::WaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                             const FGameplayTag InCooldownTags)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTags;

	if (!IsValid( AbilitySystemComponent) || !InCooldownTags.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}
	// To Know when a cooldown has Ended(cooldown tag has been Remove)
	//知道冷卻已結束（冷卻標籤已被移除）
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTags,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
			WaitCooldownChange,
			&UWaitCooldownChange::CooldownTagChange	);
	
	// To Know when a cooldown effect has been applied
	//知道冷卻效果已被應用
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange,&UWaitCooldownChange::OnActiveEffectAdded);
	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC))return;
	ASC->RegisterGameplayTagEvent(CooldownTag,EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	//標記垃圾回收
	MarkAsGarbage();
}

void UWaitCooldownChange::CooldownTagChange(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if(NewCount <=0)
	{
		OnCooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetAsc,const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);
	if(AssetTags.HasTag(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(Query);
		if(TimesRemaining.Num()>0)
		{
			float TimeRemaining = TimesRemaining[0];
			for(int32 i = 1; i<TimesRemaining.Num();i++)
			{
				if(TimesRemaining[i]>TimeRemaining)
				{
					TimeRemaining = TimesRemaining[i];
				}
			}
			
			OnCooldownStart.Broadcast(TimeRemaining);
		}

	}
}
