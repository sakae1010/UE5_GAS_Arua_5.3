

#include "Character/AuraCharacterBase.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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

