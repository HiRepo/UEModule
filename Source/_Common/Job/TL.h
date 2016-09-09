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
			return pCharacter->GetJobComposite().FindJob<T>();
		}

		static T* Get( AActor* pActor )
		{
			if( ACommonCharacter* pCommonCharacter = Cast<ACommonCharacter>( pActor ) )
				return Get( pCommonCharacter );
			
//				UJobActorComposite* pJobActorComposite = TL::Component<UJobActorComposite>::Get( pActor );
//				UJobComposite pRootComposite = pCharacter->GetJobComposite();
			return nullptr;
		}

		static bool GetAll( ACommonCharacter* pCharacter, TArray<T*>& rOutArray, bool isAppend = false )
		{
			if( nullptr == pCharacter )
			{
				if(	!isAppend )
					rOutArray.Reset();
				return false ;
			}

			return	pCharacter->GetJobComposite().FindJob<T>( rOutArray, isAppend );
//			else
//				UJobActorComposite* pJobActorComposite = TL::Component<UJobActorComposite>::Get( pActor );
//				UJobComposite pRootComposite = pCharacter->GetJobComposite();

			return false;
		}


		static bool GetAll( AActor* pActor, TArray<T*>& rOutArray, bool isAppend = false )
		{
			if( ACommonCharacter* pCommonCharacter = Cast<ACommonCharacter>( pActor ) )
				return GetAll( pCommonCharacter, rOutArray, isAppend );

//				UJobActorComposite* pJobActorComposite = TL::Component<UJobActorComposite>::Get( pActor );
//				UJobComposite pRootComposite = pCharacter->GetJobComposite();

			return false;
		}
	};

}
