#pragma once
#include "BehaviorTree/BlackboardComponent.h"
#include "Job/JobBehaviorTree.h"


inline void AIStart( ACommonCharacter* pCharacter )
{
	pCharacter->GetJobBehaviorTree().Start();
}

inline void AIStop( ACommonCharacter* pCharacter )
{
	pCharacter->GetJobBehaviorTree().Stop();
}


namespace TL{

	template<class T_TASK, class TFilter=NullFilter>
	inline void AITask( UWorld* pWorld, T_TASK t_task, TFilter filter =NullFilter())
	{
		TArray<ACommonCharacter*> arrCharacter;
		TL::Actor<ACommonCharacter>::GetAll( pWorld, arrCharacter, filter );
		for( ACommonCharacter* pCharacter : arrCharacter )
		{
			t_task( pCharacter );
		}
	}


	template<class T>
	struct AI
	{
		static T* GetBlackboard( AActor* pActor )
		{
			type_assert( T, UBlackboardComponent );
			if( nullptr == pActor )
				return nullptr;

			T* pBlackboard = nullptr;

			APawn* pPawn = Cast<APawn>( pActor );
			if( pPawn && pPawn->GetController() )
				pBlackboard = pPawn->GetController()->FindComponentByClass<T>();

			if( nullptr == pBlackboard )
				pBlackboard = pActor->FindComponentByClass<T>();

			return pBlackboard;
		}

		static T* GetBlackboard( APawn* pPawn )
		{
			type_assert( T, UBlackboardComponent );
			if( nullptr == pPawn )
				return nullptr;

			T* pBlackboard = nullptr;
			if( pPawn->GetController() )
				pBlackboard = pPawn->GetController()->FindComponentByClass<T>();

			return pBlackboard;
		}

		static T* GetBlackboard( UJobBehaviorTree* pJobBT )
		{
			if( nullptr == pJobBT )
				return nullptr;
			return Cast<T>( pJobBT->GetCurBlackboard() );
		}

		static T* GetBlackboard( UBehaviorTreeComponent* pBTComp )
		{
			if( nullptr == pBTComp )
				return nullptr;
			return Cast<T>( pBTComp->GetBlackboardComponent() );
		}

	};

}
