

#include "Character/AuraCharacterBase.h"
#include "AuraGameplayTag.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "AbilitySystem/Passive/PasssiveNiagaraComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BurnDebuffNiagaraComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffNiagaraComponent");
	BurnDebuffNiagaraComponent->SetupAttachment( GetRootComponent() );
	BurnDebuffNiagaraComponent->DebuffTag = FAuraGameplayTags::Get().Debuff_Burn;

	StunDebuffNiagaraComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("StunDebuffNiagaraComponent");
	StunDebuffNiagaraComponent->SetupAttachment( GetRootComponent() );
	StunDebuffNiagaraComponent->DebuffTag = FAuraGameplayTags::Get().Debuff_Stun;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachPoint");
	EffectAttachComponent->SetupAttachment(GetRootComponent());

	
	HaloOfProtectionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("HaloOfProtectionComponent");
	HaloOfProtectionNiagaraComponent->SetupAttachment(EffectAttachComponent);
	HaloOfProtectionNiagaraComponent->PassiveSpellTag = FAuraGameplayTags::Get().Abilities_Passive_HaloOfProtection;

	LifeSiponNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiponComponent");
	LifeSiponNiagaraComponent->SetupAttachment(EffectAttachComponent);
	LifeSiponNiagaraComponent->PassiveSpellTag = FAuraGameplayTags::Get().Abilities_Passive_LifeSiphon;

	ManaSiponNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaSiponComponent");
	ManaSiponNiagaraComponent->SetupAttachment(EffectAttachComponent);
	ManaSiponNiagaraComponent->PassiveSpellTag = FAuraGameplayTags::Get().Abilities_Passive_ManaSiphon;
	
	
}

void AAuraCharacterBase::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
	EffectAttachComponent->SetWorldRotation( FRotator::ZeroRotator );
}

void AAuraCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );
	DOREPLIFETIME(AAuraCharacterBase,bIsStunned);
	DOREPLIFETIME(AAuraCharacterBase,bIsBurned);
	DOREPLIFETIME(AAuraCharacterBase,bIsBeingShocked);
}

float AAuraCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage( DamageAmount , DamageEvent , EventInstigator , DamageCauser );
	OnDamageDelegate.Broadcast(DamageTaken);
	return DamageTaken;
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

void AAuraCharacterBase::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation() const
{
	return CharacterClass;
}

FOnASCRegistered& AAuraCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnAscRegisteredDelegate;
}

FOnDeathSignature& AAuraCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

FOnDamageSignature& AAuraCharacterBase::GetOnDamageSignature()
{
	return OnDamageDelegate;
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeapon_Implementation() const
{
	return Weapon;
}

bool AAuraCharacterBase::IsBeingShocked_Implementation() const
{
	return  bIsBeingShocked;
}

void AAuraCharacterBase::SetIsBeingShocked_Implementation(bool bInShockLoop)
{
	if (bInShockLoop==bIsBeingShocked)return;
	bIsBeingShocked = bInShockLoop;
}


void AAuraCharacterBase::Die(const FVector& Impulse)
{
	//讓手上武器脫離
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));

	MulticastHandleDeath(Impulse);
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation(const FVector& Impulse)
{
	if (DeadSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this,DeadSound,GetActorLocation() , GetActorRotation());
	}
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Weapon->AddImpulse( Impulse * 0.1f , NAME_None , true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	GetMesh()->AddImpulse( Impulse , NAME_None , true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Dissolve();
	bDead = true;
	BurnDebuffNiagaraComponent->Deactivate();
	StunDebuffNiagaraComponent->Deactivate();
	OnDeathDelegate.Broadcast(this);
	
}

void AAuraCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f: BaseWalkSpeed;
}

void AAuraCharacterBase::OnRep_Stunned()
{
}

void AAuraCharacterBase::OnRep_Burned()
{
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
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
	AuraAbilitySystemComponent->AddCharacterPassvieAbilities(StartupPassiveAbilities);
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
