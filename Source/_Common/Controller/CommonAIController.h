#pragma once
#include "AIController.h"
#include "CommonAIController.generated.h"

UCLASS(placeable)
class _COMMON_API ACommonAIController : public AAIController
{
	GENERATED_BODY()

protected :

private :

public:
	ACommonAIController(){};

	virtual void Possess( APawn* pPawn ) override;

	virtual void UnPossess() override;

};
