// sakae's gas project


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"

// Sets default values
AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	//server 執行移動邏輯 Client 產生複製體
	bReplicates = true;
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement( true );

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	SetLifeSpan(LifeSpan);
	if(LoopingSound)
	{
		LoppingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, RootComponent);
		
	}
}

void AAuraProjectile::OnHit()
{
	if(ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	}
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(),FRotator::ZeroRotator);
		if(LoppingSoundComponent)
		{
			LoppingSoundComponent->Stop();
			LoppingSoundComponent->DestroyComponent();
		}
	}
	bHit = true;
}

void AAuraProjectile::Destroyed()
{
	if(LoppingSoundComponent)
	{
		LoppingSoundComponent->Stop();
		LoppingSoundComponent->DestroyComponent();
	}
	if(!bHit && !HasAuthority())
	{
		OnHit();
	}
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsValid(DamageEffectParams.SourceAbilitySystemComponent)) return; // 如果沒有設定SourceAbilitySystemComponent就不做任何事
	const AActor* SourceActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if(SourceActor == OtherActor)
	{
		return;
	}

	if(!UAuraAbilitySystemLibrary::IsNotFriend(SourceActor ,OtherActor))
	{
		return;
	}

	
	if (!bHit)
	{
		OnHit();
	}
	if(HasAuthority())
	{
		if(UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const auto DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulseVector = DeathImpulse;

			const bool bIsKnockback = FMath::FRandRange(1.f, 100.f) < DamageEffectParams.KnockbackChance;
			if(bIsKnockback)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;

				const FVector KnokbackDirection = Rotation.Vector();
				const FVector KnokbackForce = KnokbackDirection * DamageEffectParams.KnockbackForceMagnitude;
				DamageEffectParams.KnockbackForce = KnokbackForce;
			}

			DamageEffectParams.TargetAbilitySystemComponent = TargetAbilitySystemComponent;
			UAuraAbilitySystemLibrary::ApplyDamageEffectParams(DamageEffectParams);
		}
		Destroy();
	}else
	{
		bHit = true;
	}

	
	// UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
}


