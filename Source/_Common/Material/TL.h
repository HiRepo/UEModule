#pragma once

namespace TL{


	inline int MaterialIndex( UMeshComponent* pMeshComponent, UMaterialInterface* pMat )
	{
		TArray<class UMaterialInterface*> matArray = pMeshComponent->GetMaterials();
		int iNumMat = matArray.Num();
		for( int i = 0; i < iNumMat; ++i )
		{
			if(	matArray[i] == pMat )
				return i; 
		}
		return -1;
	}




	template<class T> 
	struct Mesh
	{
		FORCEINLINE
		static T* Get( AActor* pActor )
		{
			type_assert( T, UMeshComponent );
			return Component<T>::Get( pActor );
		}
	};
	template<> 
	struct Mesh<USkeletalMeshComponent>
	{
		FORCEINLINE
		static USkeletalMeshComponent* Get( AActor* pActor )
		{
			return Component<USkeletalMeshComponent>::Get( pActor );
		}
		
		FORCEINLINE
		static USkeletalMeshComponent* Get( ACharacter* pCharacter )
		{
			return pCharacter->GetMesh();
		}

	};



	template<class T> 
	struct Material
	{
		FORCEINLINE
		static T* Get( UMeshComponent* pMeshComponent )
		{
			type_assert( T, UMaterialInterface );
			return  pMeshComponent 	? Cast<T>(pMeshComponent->GetMaterial(0)) : nullptr;
		}

//		FORCEINLINE
		template< class TComp >
		static T* Get( AActor* pActor )
		{
			type_assert( TComp, UMeshComponent );
			return	Get( Mesh<TComp>::Get( pActor ) );
		}

		FORCEINLINE
		static T* GetBase( UMeshComponent* pMeshComponent )
		{
			UMaterialInterface* pMatInterface = TL::Material<UMaterialInterface>::Get(pMeshComponent);
			return  pMatInterface ? pMatInterface->GetBaseMaterial() : nullptr;
		}

		template< class TComp > 
		static T* GetBase( AActor* pActor )
		{
			UMaterialInterface* pMatInterface = TL::Material<UMaterialInterface>::Get<TComp>(pActor);
			return  pMatInterface ? pMatInterface->GetBaseMaterial() : nullptr;
		}

		static bool GetAll( TArray<T*>& rOutArray, UMeshComponent* pMeshComponent, FName BaseName = NAME_None, bool isAppend = false )
		{
			type_assert( T, UMaterialInterface );
			if( nullptr == pMeshComponent )
				return false;
			if( !isAppend )
				rOutArray.Reset();

			const int _arrayNum = rOutArray.Num();

			TArray<UMaterialInterface*> arrMaterialInterface = pMeshComponent->GetMaterials();
			for( UMaterialInterface* pMat : arrMaterialInterface )
			{
				pMat = Cast<T>( pMat );
				if( pMat == nullptr )
					continue;

				FString sCurBaseMatName = pMat->GetBaseMaterial()->GetName();
				if( pMat && (BaseName == NAME_None || BaseName == sCurBaseMatName))
					rOutArray.Push(pMat);
			}
			return _arrayNum != rOutArray.Num();
		}

		template< class TComp > 
		static bool GetAll( TArray<T*>& rOutArray, AActor* pActor, FName BaseName = NAME_None, FName tag = NAME_None, bool isAppend = false )
		{
			type_assert( T, UMaterialInterface );
			type_assert( TComp, UMeshComponent );
			if( nullptr == pActor )
				return false;
			if( !isAppend )
				rOutArray.Reset();

			const int _arrayNum = rOutArray.Num();

			TArray<TComp*> componentArray;
			Component<TComp>::GetAll( pActor, componentArray, tag );
			for( auto pMeshComponent : componentArray )
			{
				TArray<UMaterialInterface*> arrMaterialInterface = pMeshComponent->GetMaterials();
				for( UMaterialInterface* pMat : arrMaterialInterface )
				{
					pMat = Cast<T>( pMat );
					FString sCurBaseMatName = pMat->GetBaseMaterial()->GetName();
					if( pMat && (BaseName == NAME_None || BaseName == sCurBaseMatName) )
						rOutArray.Push( pMat );
				}
			}


			return _arrayNum != rOutArray.Num();
		}

	};
}

