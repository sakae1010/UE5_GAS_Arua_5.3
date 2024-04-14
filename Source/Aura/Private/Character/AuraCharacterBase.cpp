

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

void AAuraCharacterBase::InitializePrimaryAttributes() const
{
	UAbilitySystemComponent* Target = GetAbilitySystemComponent();
	check(Target)
	check(DefaultPrimaryAttributeEffect)
	const FGameplayEffectContextHandle Handle = Target->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = Target->MakeOutgoingSpec(DefaultPrimaryAttributeEffect, 1 ,Handle);
	Target->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get() , Target);
}
