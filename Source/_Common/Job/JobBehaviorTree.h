#pragma once
#include "JobComponent.h"
#include "AIController.h"
#include "JobBehaviorTree.generated.h"


UCLASS(Abstract, Blueprintable, Category="Job", meta=(DisplayName="Job BlackBoard Data"))
class _COMMON_API UJobDataBB : public UObject
{
	GENERATED_BODY()
friend struct TL::Create<UJobDataBB, false>;
friend class UJobBehaviorTree;

private :
	UPROPERTY( EditAnywhere, Category="JobComponent") 
	UBlackboardData* AssetBB = nullptr;

	UPROPERTY( Transient ) 
	UBlackboardComponent* m_pCompBB = nullptr;

	UPROPERTY( Transient ) 
	AAIController* m_pAIController  = nullptr;

protected :
	void _init( APawn* pPawn )
	{
		if( nullptr == pPawn )
			return;
		UseBlackboard( TL::Controller<AAIController>::Get( pPawn ) );
	}

	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected, DisplayName = "Init"))
	void _InitBB( const UBlackboardComponent* pBlackboard, APawn* pPawn );

	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected, DisplayName = "Tick"))
	void _Tick( const UBlackboardComponent* pBlackboard, APawn* pPawn, float fDeltaTime  ) const;

	void Tick( float fDeltaTime ) const
	{
		if( m_pAIController )
			_Tick( m_pCompBB, m_pAIController->GetPawn(), fDeltaTime );
	}

public :
	UFUNCTION(BlueprintPure, Category="Job|AI|BB")
  	UBlackboardComponent* GetBlackboard(){ return m_pCompBB; }

	FORCEINLINE
	UBlackboardData* GetAssetBB(){ return AssetBB; }

	void UseBlackboard( AAIController* pController )
	{
		if( AssetBB && pController )
		{
			pController->UseBlackboard( AssetBB, m_pCompBB );
			if( m_pCompBB )
			{
				m_pAIController	= pController;
				_InitBB( m_pCompBB, pController->GetPawn() );
			}
		}
	}
};



USTRUCT()
struct FBTData
{
	GENERATED_BODY()
friend class UJobBehaviorTree;

protected :
	UPROPERTY( EditAnywhere )
	UBehaviorTree*  AssetBT = nullptr;

	UPROPERTY( EditAnywhere )
	TSubclassOf<UJobDataBB>  JobDataBBClass;

private :
	UPROPERTY( Transient, meta=(DisplayName="Init BlackBoard"))
	UJobDataBB* JobDataBB = nullptr;

public :
	UJobDataBB* GetJobDataBB( UJobBehaviorTree* pJobBT, APawn* pPawn );
};



UCLASS(Category="Job", meta=(DisplayName ="Job BehaviorTree"))
class _COMMON_API UJobBehaviorTree : public UJobComponentController
{
	GENERATED_BODY()
friend struct TL::Create<UJobBehaviorTree, false>;

private :
	UPROPERTY( EditAnywhere, Category="JobComponent",  meta=(DisplayName = "BT Datas") )
  	TArray<FBTData> BTDataArray;

	UPROPERTY( EditAnywhere, Category="JobComponent",  meta=(DisplayName = "Current BT Index") )
  	int m_iCurBT = 0;

public :
	UFUNCTION(BlueprintPure, Category="Job|AI") 
  	int GetCurrentBTIndex(){ return m_iCurBT; }
	
	UFUNCTION(BlueprintPure, Category="Job|AI") 
	UBlackboardComponent* GetCurBlackboard()
	{ 
		if( false == BTDataArray.IsValidIndex( m_iCurBT ) )
			return nullptr;						
		FBTData& rBTData = BTDataArray[m_iCurBT];
		return rBTData.JobDataBB ? rBTData.JobDataBB->GetBlackboard() : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category="Job|AI") 
	bool ChangeBT( int index );



//	void UBlackboardComponent::SetValueAsVector(const FName& KeyName, FVector VectorValue)
//	{
//		const FBlackboard::FKey KeyID = GetKeyID(KeyName);
//		SetValue<UBlackboardKeyType_Vector>(KeyID, VectorValue);
//	}
//	void UBlackboardComponent::SetValueAsRotator(const FName& KeyName, FRotator RotatorValue)
//	{
//		const FBlackboard::FKey KeyID = GetKeyID(KeyName);
//		SetValue<UBlackboardKeyType_Rotator>(KeyID, RotatorValue);
//	}
//	bool UBlackboardComponent::IsVectorValueSet(const FName& KeyName) const
//	{
//		const FBlackboard::FKey KeyID = GetKeyID(KeyName);
//		return IsVectorValueSet(KeyID);
//	}


//		UPROPERTY( EditAnywhere, Category="Time Array",  meta=(DisplayName = "Default Time List") )
//		TArray<FTimeDilation> 	m_TimeDilationDatas;

protected :

public :
	UJobBehaviorTree() 	
	{ m_EnableTick = true; }

	FORCEINLINE
 	FBTData& operator[]( int idx )
 	{ 
		check( BTDataArray.IsValidIndex( idx ) );
 		return BTDataArray[ idx ]; 
 	}


protected :
	virtual void Possess( APawn* pPawn ) override
	{
		m_pPawn = pPawn;
		if( ChangeBT( m_iCurBT ) )
			_OnPossess( pPawn );
	}

	virtual void UnPossess() override
	{
		Super::UnPossess();
	}

	virtual void Tick( float fDeltaTime ) override
	{
		Super::Tick( fDeltaTime );
		if( nullptr == m_pPawn )
			return;

		TArray<const UJobDataBB*> jobDataBBUniArray;		// Filter Redundant BlackBoard
		for( FBTData& rBTData : BTDataArray )
		{
			UJobDataBB* pJobDataBB = rBTData.JobDataBB;
			if( false == jobDataBBUniArray.Contains( pJobDataBB ) )
				jobDataBBUniArray.Add( pJobDataBB );
		}

		jobDataBBUniArray.Remove( nullptr );

		for( const UJobDataBB* pJobDataBB : jobDataBBUniArray )
			pJobDataBB->Tick( fDeltaTime );
	}

//		virtual void OnAttach() override
//		{
//			Super::OnAttach();
//		}
//	
//		virtual void OnDetach() override
//		{
//			Super::OnDetach();
//		}
};


inline UJobDataBB* FBTData::GetJobDataBB( UJobBehaviorTree* pJobBT, APawn* pPawn )
{
	if( nullptr == JobDataBB &&  nullptr != JobDataBBClass )
		JobDataBB = TL::Create<UJobDataBB>::Init( pJobBT, JobDataBBClass, JobDataBBClass->GetFName(), pPawn );
	return JobDataBB;
}

