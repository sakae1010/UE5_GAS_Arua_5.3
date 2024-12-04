// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"


class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply,
};
UENUM(BlueprintType)
enum class EEffectRemovePolicy
{
	RemoveOnEndOverlap,
	DontRemove,
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "Pickup Movement")
	FVector CalculatedLocation ;
	
	UPROPERTY(BlueprintReadWrite, Category = "Pickup Movement")
	FRotator CalculatedRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Movement")
	bool bRotators = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Movement")
	float RotationRate = 45.f;

	//正琁
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Movement")
	bool bSinusoidalMovement = false;
	
	UFUNCTION(BlueprintCallable, Category = "Pickup Movement")
	void StartSinusoidalMovement();
	
	UFUNCTION(BlueprintCallable, Category = "Pickup Movement")
	void StartRotation();
	//振幅
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Movement")
	float SineAmplitude = 1.f;
	//周期
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Movement")
	float SinePeriodConstant = 1.f;//  2pi  3.14159 *2

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Movement")
	FVector InitialLocation;

	
	UFUNCTION(BlueprintCallable, Category = "Applied Effect")
	void ApplyEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable, Category = "Applied Effect")
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = "Applied Effect")
	void OnEndOverlap(AActor* TargetActor);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	bool bDestroyOnEffectApplication = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	bool bApplyEffectsToEnemy = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Applied Effect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Applied Effect")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Applied Effect")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Applied Effect")
	EEffectRemovePolicy InfiniteGameplayEffectRemovePolicy = EEffectRemovePolicy::RemoveOnEndOverlap;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effect")
	float ActorLevel = 1.f;
private:

	 float RunningTime = 0.f;

	void ItemMovement(float DeltaTime);
};
