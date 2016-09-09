#pragma once
#include "_Common.h"
#include "WorldCollision.h"

namespace TL{
namespace Physics{
	// TL::Collision<AHeroBase>::Test( boxComponent )

// ============		Component		=================================================================

	inline bool Collision( UPrimitiveComponent* pPrimitive, const AActor* pOtherActor )
	{
		check( pPrimitive );
		return pPrimitive->IsOverlappingActor( pOtherActor );
	}

	inline bool Collision( UPrimitiveComponent* pPrimitive, UPrimitiveComponent const* pOtherComp )
	{
		check( pPrimitive );
		return pPrimitive->IsOverlappingComponent( pOtherComp );
	}

	inline bool Collision( UPrimitiveComponent* pPrimitive, UClass* pClassFilter = nullptr )
	{
		check( pPrimitive );
		TArray<UPrimitiveComponent*> inOverlappingComponents;
		pPrimitive->GetOverlappingComponents( inOverlappingComponents );
		const uint _nOverlap = inOverlappingComponents.Num();
		if( nullptr == pClassFilter )
			return !!_nOverlap;

		for( UPrimitiveComponent* pComp : inOverlappingComponents )
		{
			if( pComp->GetOwner()->IsA( pClassFilter ) )
				return true;
		}
		return false;
	}

	inline bool Collision( UPrimitiveComponent* pPrimitive, TArray<AActor*>& rOutActor, UClass* pClassFilter = nullptr )
	{
		check( pPrimitive );

		const int _numOri = rOutActor.Num();

		pPrimitive->GetOverlappingActors( rOutActor, pClassFilter );

		return _numOri < rOutActor.Num();
	}

// ============		World		=================================================================

//	Ref : UKismetSystemLibrary::SphereOverlapComponents_NEW
	inline bool Collision( UWorld* pWorld, const FCollisionShape& rCollisionShape, const FVector vPos, TArray<UPrimitiveComponent*>& rOutComp, const TArray<AActor*>& rIgnoreActors, UClass* const pCompFilter=nullptr )
	{
		check( pWorld );
		check( ( nullptr == pCompFilter || pCompFilter->IsChildOf(UPrimitiveComponent::StaticClass()) ) );
		const int _numOri = rOutComp.Num();

		static FName QueryName( TEXT("TL::Physics::Collision") );
		FCollisionQueryParams Params( QueryName, false );
		Params.bTraceAsyncScene = true;
		Params.AddIgnoredActors(rIgnoreActors);

		FCollisionObjectQueryParams objParams;
//			TArray<TEnumAsByte<EObjectTypeQuery>>  ObjectTypes;		// ?????? 
//			for( auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter )
//			{
//				const ECollisionChannel& rChannel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);		// Link Error
//				objParams.AddObjectTypesToQuery( rChannel );
//			}

		TArray<FOverlapResult> inOverlapResults;
		pWorld->OverlapMultiByObjectType( inOverlapResults, vPos, FQuat::Identity, objParams, rCollisionShape, Params );
//		pWorld->OverlapMultiByObjectType( inOverlapResults, vPos, FQuat::Identity, objParams, FCollisionShape::MakeSphere(SphereRadius), Params );
//		pWorld->OverlapMultiByObjectType( inOverlapResults, vPos, FQuat::Identity, objParams, FCollisionShape::MakeBox(BoxExtent), Params);
//		pWorld->OverlapMultiByObjectType( inOverlapResults, vPos, FQuat::Identity, objParams, FCollisionShape::MakeCapsule(Radius, HalfHeight), Params);

		for( FOverlapResult& rOverlapResult : inOverlapResults )
		{
			if( false == rOverlapResult.Component.IsValid() )
				continue;

			UPrimitiveComponent* pComp = rOverlapResult.Component.Get();
			if( nullptr == pCompFilter || pComp->IsA( pCompFilter ) )
				rOutComp.Add( pComp );
		}

		return _numOri < rOutComp.Num();
	}

	inline bool Collision( UWorld* pWorld, const FCollisionShape& rCollisionShape, const FVector vPos, TArray<UPrimitiveComponent*>& rOutComp, AActor* pIgnoreActor=nullptr, UClass* pCompFilter=nullptr)
	{
		TArray<AActor*> ignoreActors;
		if( pIgnoreActor )
			ignoreActors.Add( pIgnoreActor );
		return Collision( pWorld, rCollisionShape, vPos, rOutComp, ignoreActors, pCompFilter );
	}


	inline bool Collision( UWorld* pWorld, const FCollisionShape& rCollisionShape, const FVector vPos, TArray<AActor*>& rOutActor, const TArray<AActor*>& rIgnoreActors, UClass* pActorFilter=nullptr, UClass* pCompFilter=nullptr )
	{
		check( ( nullptr == pActorFilter || pActorFilter->IsChildOf(AActor::StaticClass()) ) );

		TArray<UPrimitiveComponent*> inOverlapComponents;
		bool bResult = Collision( pWorld, rCollisionShape, vPos, inOverlapComponents, rIgnoreActors, pCompFilter );
		if( false == bResult )
			return false;

		return TL::Component<UPrimitiveComponent>::GetActors( inOverlapComponents, rOutActor, pActorFilter );
	}

	inline bool Collision( UWorld* pWorld, const FCollisionShape& rCollisionShape, const FVector vPos, TArray<AActor*>& rOutActor, AActor* pIgnoreActor= nullptr, UClass* pActorFilter=nullptr, UClass* pCompFilter=nullptr )
	{
		TArray<AActor*> ignoreActors;
		if( pIgnoreActor )
			ignoreActors.Add( pIgnoreActor );
		return Collision( pWorld, rCollisionShape, vPos, rOutActor, ignoreActors, pActorFilter, pCompFilter );
	}




//		template<class TFuncFilter>
//		inline bool CollisionFilter( UWorld* pWorld, const FCollisionShape& rCollisionShape, const FVector vPos, TArray<AActor*>& rOutActor, const TArray<AActor*>& rIgnoreActors, UClass* pActorFilter, UClass* pCompFilter, TFuncFilter Filter )
//		{
//			check( ( nullptr == pActorFilter || pActorFilter->IsChildOf(AActor::StaticClass()) ) );
//	
//			TArray<UPrimitiveComponent*> inOverlapComponents;
//			bool bResult = Collision( pWorld, rCollisionShape, vPos, inOverlapComponents, rIgnoreActors, pCompFilter );
//			if( false == bResult )
//				return false;
//	
//			return TL::Component<UPrimitiveComponent>::GetActors( inOverlapComponents, rOutActor, pActorFilter, Filter );
//		}
//	
//		template<class TFuncFilter>
//		inline bool CollisionFilter( UWorld* pWorld, const FCollisionShape& rCollisionShape, const FVector vPos, TArray<AActor*>& rOutActor, AActor* pIgnoreActor, UClass* pActorFilter, UClass* pCompFilter, TFuncFilter Filter )
//		{
//			
//			TArray<AActor*> ignoreActors;
//			if( pIgnoreActor )
//				ignoreActors.Add( pIgnoreActor );
//			return Collision( pWorld, rCollisionShape, vPos, rOutActor, ignoreActors, pActorFilter, pCompFilter, Filter );
//		}





//		template <typename Predicate>
//		int32 IndexOfByPredicate(Predicate Pred) const
//		{
//			const ElementType* RESTRICT Start = GetData();
//			for (const ElementType* RESTRICT Data = Start, *RESTRICT DataEnd = Start + ArrayNum; Data != DataEnd; ++Data)
//			{
//				if (Pred(*Data))
//				{
//					return static_cast<int32>(Data - Start);
//				}
//			}
//			return INDEX_NONE;
//		}


} /* NameSpace End : Physics */



// ============		Template Library		=================================================================

	template<class T>
	struct Collision
	{
		FORCEINLINE
		static bool Component( UPrimitiveComponent* pPrimitive )
		{
			return Physics::Collision( pPrimitive, T::StaticClass() );
		}

		static bool Component( UPrimitiveComponent* pPrimitive, TArray<T*>& rOut )
		{
			TArray<AActor*> outActorArray; 
			bool bResult = Physics::Collision( pPrimitive, outActorArray, T::StaticClass() );
			for( AActor* pActor : outActorArray )
				rOut.Add( static_cast<T*>( pActor ) );
			return bResult;
		}


		static bool World( UWorld* pWorld, const FCollisionShape& rCollisionShape, const FVector vPos, TArray<T*>& rOut, AActor* pIgnoreActor= nullptr )
		{
			TArray<AActor*> outActorArray; 
			bool bResult = Physics::Collision( pWorld, rCollisionShape, vPos, outActorArray, pIgnoreActor, T::StaticClass() );
			for( AActor* pActor : outActorArray )
				rOut.Add( static_cast<T*>( pActor ) );
			return bResult;
		}

		static bool World( UWorld* pWorld, const FCollisionShape& rCollisionShape, const FVector vPos, TArray<T*>& rOut, const TArray<AActor*>& rIgnoreActors )
		{
			TArray<AActor*> outActorArray; 
			bool bResult = Physics::Collision( pWorld, rCollisionShape, vPos, outActorArray, rIgnoreActors, T::StaticClass() );
			for( AActor* pActor : outActorArray )
				rOut.Add( static_cast<T*>( pActor ) );
			return bResult;
		}


		static T* GetClose( UWorld* pWorld, const FCollisionShape& rCollisionShape, const FVector vPos, const TArray<AActor*>& rIgnoreActors )
		{
			TArray<T*> outArray; 
			AActor* pResultActor = nullptr;
			if( TL::Collision<T>::World( pWorld, rCollisionShape, vPos, outArray, rIgnoreActors ) )
			{
				pResultActor = outArray[0];
				float minLength = FVector::Dist( vPos , pResultActor->GetActorLocation() );
				for( int i = 1; i < outArray.Num(); ++i )
				{
					AActor* pActor = outArray[i];
					float fLength = FVector::Dist( vPos , pActor->GetActorLocation() );
					if( fLength < minLength )
					{
						pResultActor = pActor;
						minLength = fLength;
					}
				}
			}
			return static_cast<T*>(pResultActor);
		}

		static T* GetClose( UWorld* pWorld, const FCollisionShape& rCollisionShape, const FVector vPos, AActor* pIgnoreActor = nullptr )
		{
			TArray<AActor*> ignoreActors;
			if( pIgnoreActor )
				ignoreActors.Add( pIgnoreActor );
			return GetClose( pWorld, rCollisionShape, vPos,  ignoreActors );
		}

	};
}

