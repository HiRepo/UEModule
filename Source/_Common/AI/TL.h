#pragma once
#include "BehaviorTree/BlackboardComponent.h"
#include "Job/JobBehaviorTree.h"


namespace TL{

	template< class T >
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
