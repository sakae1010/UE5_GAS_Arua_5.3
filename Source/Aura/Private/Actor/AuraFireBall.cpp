// sakae's gas project


#include "Actor/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTag.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsValidOverlap(OtherActor))
	{
		return;
	}

	if(HasAuthority())
	{
		if(UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const auto DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulseVector = DeathImpulse;
			DamageEffectParams.TargetAbilitySystemComponent = TargetAbilitySystemComponent;
			UAuraAbilitySystemLibrary::ApplyDamageEffectParams(DamageEffectParams);
		}
		
	}
	
}

void AAuraFireBall::OnHit()
{
	/** 
	 *  Convenience methods for invoking non-replicated gameplay cue events. 
	 * 
	 *	We want to avoid exposing designers the choice of "is this gameplay cue replicated or non-replicated?".
	 *	We want to make the decision for them in most cases:
	 *	- Abilities will always use replicated GameplayCue events because they are not executed on simulated proxies.
	 *	- Animations always use non-replicated GameplayCue events because they are always executed on simulated proxies.
	 *	
	 *  Sometimes it will be useful to give designers both options: in actor classes where there are many possible use cases.
	 *  Still, we should keep the choice confined to the actor class, and not globally.  E.g., Don't add both choices to the function library
	 *  since they would appear everywhere. Add the choices to the actor class so they only appear there.
	 */
	//翻譯：為了方便調用非複製的遊戲玩法提示事件。
	//我們希望避免設計師暴露“這個遊戲玩法提示是複製的還是非複製的？”的選擇。
	//我們希望在大多數情況下為他們做出決定：
	//-能力始終使用複製的GameplayCue事件，因為它們不在模擬代理上執行。
	//-動畫始終使用非複製的GameplayCue事件，因為它們始終在模擬代理上執行。
	//有時在actor類中給設計師提供這兩種選擇會很有用。
	//但是，我們應該將選擇限制在actor類中，而不是全局。例如，不要將這兩種選擇添加到函數庫中
	//因為它們將出現在所有地方。將選擇添加到actor類中，這樣它們只會出現在那裡。

	if (GetOwner())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParams);
	}
	
	if(LoppingSoundComponent)
	{
		LoppingSoundComponent->Stop();
		LoppingSoundComponent->DestroyComponent();
	}
	bHit = true;
}
