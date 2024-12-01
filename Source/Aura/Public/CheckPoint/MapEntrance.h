// sakae's gas project

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint.h"
#include "MapEntrance.generated.h"

UCLASS()
class AURA_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()

public:

	AMapEntrance(const FObjectInitializer& ObjectInitializer);
	/* SaveInterface */
	virtual void LoadActor_Implementation() override;
	/* SaveInterface */
	/* HightlightInterface */
	virtual void HighlightActor_Implementation() override;
	/* HightlightInterface */


	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;
	
protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
