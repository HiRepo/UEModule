#pragma once
#include "JobComposite.h"

namespace TL
{
	template< class T>
	struct Job
	{
		static T* Get( ACommonCharacter* pCharacter )
		{
			if( nullptr == pCharacter )
				return nullptr;
			return pCharacter->GetJobActorComposite()->FindJob<T>();
		}

		static T* Get( AActor* pActor )
		{
			if( nullptr == pActor )
				return nullptr;
			if( UJobActorComposite* pActorComposite = TL::Component<UJobActorComposite>::Get( pActor ) )
				return pActorComposite->FindJob<T>();
			return nullptr;
		}

		static void GetAll( ACommonCharacter* pCharacter, TArray<T*>& rOutArray )
		{
			if( nullptr == pCharacter )
				return ;
			pCharacter->GetJobActorComposite()->FindJob<T>( rOutArray );
		}

		static void GetAll( AActor* pActor, TArray<T*>& rOutArray )
		{
			if( nullptr == pActor )
				return ;
			if( UJobActorComposite* pActorComposite = TL::Component<UJobActorComposite>::Get( pActor ) )
				pActorComposite->FindJob<T>( rOutArray );
		}
	};

}
