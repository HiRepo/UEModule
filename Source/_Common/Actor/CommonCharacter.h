#pragma once
#include "CommonCharacter.generated.h"

class UJobComposite;
class UJobActorComposite;
class UJobMaterial;
class UJobTimeDilation;
class UJobAniTransform;
class UJobBehaviorTree;
class ACommonAIController;

UCLASS(ClassGroup=(Common,Actor))
class _COMMON_API ACommonCharacter : public ACharacter
{
	GENERATED_BODY()
friend class ACommonAIController;

protected :
//		UPROPERTY(EditAnywhere, Category="Job", meta=(DisplayName="Job Class Array"))
//		TArray<TSubclassOf<UJobComponent>>	m_pJobClassArray;

 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Job", meta=(DisplayName="ActorComposite"))
	UJobActorComposite* m_JobActorComposite = nullptr; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Job", meta=(DisplayName="Material"))
	UJobMaterial* m_JobMaterial			= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Job", meta=(DisplayName="Time Speed"))
    UJobTimeDilation* m_JobTimeDilation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Job", meta=(DisplayName="Ani Transform"))
    UJobAniTransform* m_JobAniTransform = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Job", meta=(DisplayName="BehaviorTree"))
    UJobBehaviorTree* m_JobBehaviorTree = nullptr;

public :
	UJobActorComposite* GetJobActorComposite(){ return m_JobActorComposite; }

    UJobMaterial& GetJobMaterial(){	return  *m_JobMaterial; }

    UJobTimeDilation& GetJobTimeDilation(){	return  *m_JobTimeDilation; }

    UJobAniTransform& GetJobAniTransform(){	return  *m_JobAniTransform; }

    UJobBehaviorTree& GetJobBehaviorTree(){	return  *m_JobBehaviorTree; }


protected :
	UPROPERTY(VisibleAnywhere, Transient )
    ACommonAIController* m_AIController = nullptr;

	virtual void OnPossessAI( ACommonAIController* pController )
	{
		m_AIController = pController;
	}

public :
	ACommonAIController* GetAIController(){ return m_AIController; }

	void PossessedFromAI( ACommonAIController* pAIController = nullptr );

protected :
//		virtual void PostDuplicate(bool bDuplicateForPIE) override;
//		virtual void PostLoad() override;
//		virtual void PostActorCreated() override;
//		virtual void PostInitProperties() override;
	virtual void PostInitializeComponents() override;
	virtual void PostLoadSubobjects( FObjectInstancingGraph* pOuterInstanceGraph ) override;


#if WITH_EDITOR
	virtual void PostEditChangeChainProperty( struct FPropertyChangedChainEvent& rPropertyChangedEvent ) override;
#endif


public:
	ACommonCharacter();

	virtual void Tick(float fDeltaTime) override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
