#pragma once
#include "BTTask_BP.h"
#include "BTTask_BPWait.generated.h"

// This is Blueprint Parent class

UCLASS(Abstract,Blueprintable)
class _COMMON_API UBTTask_BPWait : public UBTTask_BP
{
	GENERATED_BODY()

private :
	UBTTask_BPWait()
	{
		NodeName = "Wait BP";
		bNotifyTick = true;
	}

protected :
	UFUNCTION( BlueprintImplementableEvent, meta=(BlueprintProtected))
	bool _Tick( AController* pController , APawn* pPawn, float fDeltaTime );

public :
	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& rBTComponent, uint8* pNodeMemory ) override
	{
		if( Super::ExecuteTask( rBTComponent, pNodeMemory ) )
			return EBTNodeResult::InProgress;
		else
			return EBTNodeResult::Failed;
	}

protected:
	virtual void TickTask(UBehaviorTreeComponent& rBTComponent, uint8* pNodeMemory, float fDeltaTime) override
	{
		AController* pController =	TL::Controller<AController>::Get( rBTComponent );
		if( false ==  _Tick( pController, pController->GetPawn(), fDeltaTime ) )
			FinishLatentTask( rBTComponent, EBTNodeResult::Succeeded );
	}

public :
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override
	{
		return FName("BTEditor.Graph.BTNode.Task.Wait.Icon");
	}
#endif // WITH_EDITOR
};
