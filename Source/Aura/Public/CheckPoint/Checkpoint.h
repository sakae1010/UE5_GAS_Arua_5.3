// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "Aura/Aura.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HightlightInterface.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;

UCLASS()
class AURA_API ACheckpoint : public APlayerStart , public ISaveInterface  , public IHightlightInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACheckpoint(const FObjectInitializer& ObjectInitializer);
	
	/* SaveInterface */
	virtual bool ShouldLoadTransform_Implementation() override {return false;}
	virtual void LoadActor_Implementation() override;
	
	/* SaveInterface */
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;

	UPROPERTY(EditAnywhere)
	bool bBindOverlapCallBack = false;
	
protected:
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	/* HightlightInterface */

	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void MoveToLocation_Implementation(FVector&  OutLocation) override;
	/* HightlightInterface */

	UPROPERTY(EditDefaultsOnly)
	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* MaterialInstance);

	UFUNCTION(BlueprintCallable)
	void HandleGlowEffect();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
