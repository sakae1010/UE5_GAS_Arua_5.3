// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;

UCLASS()
class AURA_API ACheckpoint : public APlayerStart , public ISaveInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACheckpoint(const FObjectInitializer& ObjectInitializer);
	
	/* SaveInterface */
	virtual bool ShouldLoadTransform_Implementation() override {return false;}
	virtual void LoadActor_Implementation() override;
	/* SaveInterface */
	
	UPROPERTY(BlueprintReadOnly , SaveGame)
	bool bReached = false;


	
protected:
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* MaterialInstance);

	void HandleGlowEffect();

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
