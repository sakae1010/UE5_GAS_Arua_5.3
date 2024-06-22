

#include "Character/AuraCharacterBase.h"

#include "AuraGameplayTag.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag MontageTag)
{
	//數量太多可以用TMap<GameplayTag ,string WeaponTipSocketName>
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	if (MontageTag.MatchesTag(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);	
	}
	if (MontageTag.MatchesTag(GameplayTags.CombatSocket_LeftHand) )
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);	
	}
	if (MontageTag.MatchesTag(GameplayTags.CombatSocket_RightHand) )
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);	
	}
	if (MontageTag.MatchesTag(GameplayTags.CombatSocket_Tail) )
	{
		return GetMesh()->GetSocketLocation(TileSocketName);	
	}
	return FVector();
}


UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& Tag)
{
	for (const FTaggedMontage& Montage : AttackMontages)
	{
		if (Montage.MontageTag.MatchesTag(Tag))
		{
			return Montage;
		}
	}
	return FTaggedMontage();
}

int32 AAuraCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAuraCharacterBase::Die()
{
	//讓手上武器脫離
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	if (DeadSound!=nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this,DeadSound,GetActorLocation() , GetActorRotation());
	}
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Dissolve();
	bDead = true;

}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> EffectClass ,const float Level) const
{
	UAbilitySystemComponent* Target = GetAbilitySystemComponent();
	check(Target)
	check(EffectClass)
	FGameplayEffectContextHandle Handle = Target->MakeEffectContext();
	Handle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = Target->MakeOutgoingSpec(EffectClass, Level ,Handle);
	Target->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get() , Target);
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributeEffect , 1.f );
	ApplyEffectToSelf(DefaultSecondaryAttributeEffect , 1.f );
	ApplyEffectToSelf(DefaultVitalAttributeEffect , 1.f );
	
}

void AAuraCharacterBase::AddCharacterAbilities() 
{
	//檢查是不是server啟動
	if (!HasAuthority())return;
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(
		AbilitySystemComponent);
	AuraAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
}

void AAuraCharacterBase::Dissolve()
{
	if(IsValid(DissolveMaterialInterface))
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInterface,this);
		GetMesh()->SetMaterial(0,DynamicMaterialInstance);
		StartDissolveTimeline(DynamicMaterialInstance);
	}
	if(IsValid(WeaponDissolveMaterialInterface))
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInterface,this);
		Weapon->SetMaterial(0,DynamicMaterialInstance);
		StartWeaponDissolveTimeline(DynamicMaterialInstance);
	}
}
