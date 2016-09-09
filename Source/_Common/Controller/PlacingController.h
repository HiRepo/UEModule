#pragma once
#include "PlacingController.generated.h"


UCLASS()
class _COMMON_API APlacingController : public AActor
{
	GENERATED_BODY()

public :
	APlacingController();
	APlacingController(const FObjectInitializer& ObjectInitializer );

	UPROPERTY(EditAnywhere, BlueprintAble, Category=Classes)
	TSubclassOf<class ACommonPlayerController> CommonPlayerControllerClass;

	UPROPERTY(EditAnywhere, Category=Classes)
	int KK = 4;

	virtual void PostLoad() override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void PostDuplicate(bool bDuplicateForPIE) override;
	
	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;

	virtual void PostActorCreated() override;

	virtual void PostInitProperties() override;

#if WITH_EDITOR		
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif



	/** Called immediately before gameplay begins. */

//		virtual void PreInitializeComponents();


};

