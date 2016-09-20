#pragma once
#include "CommonCharacter.generated.h"

class UJobComponent;
class UJobComposite;
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

	UPROPERTY(EditAnywhere, Category="Job", meta=(DisplayName="Composite"))
// UPROPERTY(VisibleAnywhere, Transient, Category="Job", meta=(DisplayName="Composite") )
	UJobComposite* m_JobComposite		= nullptr;

	UPROPERTY(EditAnywhere, Category="Job", meta=(DisplayName="Material"))
	UJobMaterial* m_JobMaterial			= nullptr;

	UPROPERTY(EditAnywhere, Category="Job", meta=(DisplayName="Time Speed"))
    UJobTimeDilation* m_JobTimeDilation = nullptr;

	UPROPERTY(EditAnywhere, Category="Job", meta=(DisplayName="Ani Transform"))
    UJobAniTransform* m_JobAniTransform = nullptr;

	UPROPERTY(EditAnywhere, Category="Job", meta=(DisplayName="BehaviorTree"))
    UJobBehaviorTree* m_JobBehaviorTree = nullptr;

public :

	FORCEINLINE
	UJobComposite& GetJobComposite(){ return *m_JobComposite; }

	FORCEINLINE
    UJobMaterial& GetJobMaterial(){	return  *m_JobMaterial; }

	FORCEINLINE
    UJobTimeDilation& GetJobTimeDilation(){	return  *m_JobTimeDilation; }

	FORCEINLINE
    UJobAniTransform& GetJobAniTransform(){	return  *m_JobAniTransform; }

	FORCEINLINE
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
//		virtual void PostInitProperties() override;
//		virtual void PostDuplicate(bool bDuplicateForPIE) override;
//		virtual void PostLoad() override;
//		virtual void PostInitializeComponents() override;
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
