#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BP.generated.h"

// this is for Blueprint's parent

UCLASS(Abstract,Blueprintable)
class _COMMON_API UBTTask_BP : public UBTTaskNode
{
	GENERATED_BODY()

public :
	UPROPERTY(editinstanceonly, Category=Description)
	bool IsShowPropertyDetails = true;


protected :
	UFUNCTION( BlueprintImplementableEvent, meta=(BlueprintProtected, DisplayName="Execute Task"))
	bool _ExecuteTask( AController* pController, APawn* pPawn );

	UFUNCTION( BlueprintNativeEvent, meta=(BlueprintProtected, DisplayName="Node Description"))
	FString _NodeDescription( FString& sParentNodeDesc ) const;
	FString _NodeDescription_Implementation( FString& sParentNodeDesc ){ return sParentNodeDesc; }


protected :
	UBTTask_BP() 
	{
		NodeName = "Task";
	}

public :
	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& rBTComponent, uint8* pNodeMemory ) override
	{
		AController* pController =	TL::Controller<AController>::Get( rBTComponent );
		if( nullptr == pController )
  	   		return EBTNodeResult::Failed;

		return _ExecuteTask( pController, pController->GetPawn() ) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& rBTComponent, uint8* pNodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;

	virtual FString GetStaticDescription() const override;
};
