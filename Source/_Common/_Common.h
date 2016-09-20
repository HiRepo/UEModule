#pragma once
#include "EngineMinimal.h"
#include "EngineUtils.h"
#include "Actor/CommonCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
//	class UBehaviorTreeComponent;
 
DECLARE_LOG_CATEGORY_EXTERN(_Common, All, All)
        
typedef unsigned int		uint;

FORCEINLINE
bool operator==(const FString& sName, const FName Name)
{
	return Name == *sName;
}

FORCEINLINE
bool operator==(const FName Name, const FString& sName)
{
	return Name == *sName;
}

//	FORCEINLINE
//	bool operator!=(const FName Name, const FString& sName)
//	{
//		return Name != *sName;
//	}


namespace TL
{
#define CHK_MSG( __msg )		check( true && __msg );


struct NullFilter
{
	template<class ... Ts>
	bool operator()( Ts ... ts ){ return true; }
};


//-----------------------------------------------------------------------------------
#define type_assert(FROM,TO) \
	static_assert( TL::TypeConvertible<FROM,TO>::IsPossible, #FROM" Type must be derived  from "#TO )

	template< class T, class U>
	struct TypeConvertible
	{
		typedef char Small;
		typedef struct { char dummy[2]; } Big;
		static Small GetType(const U&);
		static Big	 GetType(...);
		static T     ObjT();
		enum
		{
			IsPossible = sizeof(Small) == sizeof(GetType(ObjT())),
			IsSameType = false,
		};
	};
	template< class T>
	struct TypeConvertible<T, T>
	{
		enum
		{
			IsPossible = true,
			IsSameType = true,
		};
	};

	template< class TSub, class TSuper >
	struct IsBaseOf
	{
		enum
		{
			Val = TypeConvertible<const TSub*, const TSuper*>::IsPossible
			&& !TypeConvertible<const TSub*, const void*>::IsSameType
		};
	};

	//-----------------------------------------------------------------------------------

#if WITH_EDITOR
#define EnumToString( EType, Val )		TL::_EnumToString<EType>( L#EType, Val )
	template< class TEnum >
	inline FString _EnumToString(const TCHAR* psEnumType, TEnum eVal)
	{
		const UEnum* pEnumObj = FindObject<UEnum>(ANY_PACKAGE, psEnumType, true);
		return pEnumObj ? pEnumObj->GetDisplayNameTextByValue((int)eVal).ToString() : FString("Invaild");
	}

	// Not DisplayName 
#define StringToEnum( EType, Str )		TL::_StringToEnum<EType>( L#EType, Str )
	template< class TEnum >
	inline TEnum _StringToEnum(const TCHAR* psEnumType, const FString& sVal)
	{
		const UEnum* pEnumObj = FindObject<UEnum>(ANY_PACKAGE, psEnumType, true);
		return pEnumObj ? (TEnum)pEnumObj->GetValueByName(FName(*sVal)) : TEnum(0);
	}

#endif // WITH_EDITOR






//-----------------------------------------------------------------------------------
	template< class T, bool = IsBaseOf<T,UInterface>::Val >
	struct Actor
	{
		FORCEINLINE
		static T* Get( const UActorComponent* pComponent )
		{
			type_assert( T, AActor ); 
			return pComponent ? Cast<T>( pComponent->GetOwner() ) : nullptr;
		}

		template <class TFilter=NullFilter>
		static T* Get( UWorld* pWorld, TFilter filter = NullFilter())
		{
			type_assert( T, AActor ); 
			if( nullptr == pWorld )
				return nullptr;
			for( TActorIterator<T> It( pWorld, T::StaticClass()); It; ++It )
			{
				T* pActor = *It;
				if( filter(pActor) && false == pActor->IsPendingKill() )
					return pActor;
			}
			return nullptr;
		}

		template <class TFilter=NullFilter>
		static T* GetByName( FString sName, UWorld* pWorld, TFilter filter = NullFilter())
		{
			return  TL::Actor<T>::Get( pWorld, [&]( T* pActor )
			{ 
				return filter(pActor) && sName == pActor->GetName();
			});
		}

		template <class TFilter=NullFilter>
		static void GetAll( UWorld* pWorld, TArray<T*>& rOutArray, TFilter filter = NullFilter())
		{
			type_assert( T, AActor ); 
			if( nullptr == pWorld )
				return ;
			for( TActorIterator<T> It( pWorld, T::StaticClass()); It; ++It )
			{
				T* pActor = *It;
				if( filter(pActor) && false == pActor->IsPendingKill() )
					rOutArray.Add( pActor );
			}
		}
	};

	template< class T >
	struct Actor<T,true>			// Only Interface
	{
		static void GetAll( UWorld* pWorld, TArray<T*>& rOutArray )
		{
			if( nullptr == pWorld )
				return ;

			for( FActorIterator It(pWorld); It; ++It )
			{
				AActor* pActor = *It;
				if( pActor && !pActor->IsPendingKill() && pActor->GetClass()->ImplementsInterface(T::StaticClass()) )
					rOutArray.Add( pActor );
			}
		}
	};



//-----------------------------------------------------------------------------------
//		enum ECreateType
//		{
//			Object,
//			Actor,
//			Component,				// RegiterComponent() Function append.
//		};
	

//	template< class T, bool = std::is_base_of<AActor,T>::value >
	template<class T, bool = IsBaseOf<T,AActor>::Val>
	struct Create
	{
		FORCEINLINE
		static T* Init( UObject* pOuter )
		{
			return Init( pOuter, T::StaticClass(), NAME_None );
		}

		template< class ... Ts >
		FORCEINLINE
		static T* Init( UObject* pOuter, FName Name, Ts ... ts )
		{
			return Init( pOuter, T::StaticClass(), Name, ts... );
		}

		template<class ... Ts > 
		FORCEINLINE
		static T* SubInit( UObject* pOuter, FName subName, Ts ... ts )
		{
			return SubInit( pOuter, T::StaticClass(), subName, ts... );
		}


		template< class ... Ts >
		static T* Init( UObject* pOuter, UClass* pClass, FName Name, Ts ... ts )
		{
			if( nullptr == pOuter )
				pOuter = (UObject*)GetTransientPackage();
			if( nullptr == pClass ) 
				pClass = T::StaticClass();
			T* pNewObj = NewObject<T>( pOuter, pClass, Name );
			pNewObj->_init(ts...);
			return pNewObj;
		}

		template<class ... Ts > 
		static T* SubInit( UObject* pOuter, UClass* pClass, FName subName, Ts ... ts )
		{
			check( pOuter )
			if( nullptr == pClass ) 
				pClass = T::StaticClass();
			T* pSubObj = (T*)pOuter->CreateDefaultSubobject(subName, pClass, pClass, /*bIsRequired =*/ true, /*bIsAbstract =*/ false, /*bTransient*/false);
			pSubObj->_init( ts... );
			return pSubObj;
		}

		static T* AttachComponent( USceneComponent* pParent, FName name )
		{
			type_assert( T, USceneComponent );
//				T* pComponent = (T*)pParent->CreateSubobject<T>( name );
			T* pComponent = NewObject<T>( pParent, T::StaticClass(), name );
			if( nullptr == pComponent )
				return nullptr;
//				pComponent->SetupAttachment( pParent );
			return pComponent;
		}
	};


	template< class T >
	struct Create< T,true>
	{
		template< class ... Ts >
		FORCEINLINE
		static T* Init( UWorld* pWorld, Ts ... ts )
		{
			return 	Init( pWorld, T::StaticClass(), ts... );
		}

		template< class ... Ts > 	
		static T* Init( UWorld* pWorld, UClass* pClass, Ts ... ts ) 
		{
			T* pOuter = pWorld->SpawnActor<T>( pClass );
			pOuter->_init( ts... );
			return pOuter;
		}
	};

	// ASpawnController* pDuplicate = TL::CloneActor<ASpawnController>( this );
	template< class T >
	inline T* Clone( AActor*  pActor )
	{
		check( pActor && "pActor  is Null" );
		FActorSpawnParameters spawnParam{};
		spawnParam.Template = pActor;
		spawnParam.Owner 	= pActor->GetOwner();
		return (T*)(pActor->GetWorld()->SpawnActor<T>(spawnParam));
	}



//-----------------------------------------------------------------------------------

	template< class T >
	struct Component
	{
		template <class TFilter=NullFilter>
		static T* Get( USceneComponent* pRootComponent, TFilter filter = NullFilter())
		{
			type_assert( T, USceneComponent ); 
			if( nullptr == pRootComponent )
				return nullptr;

			TArray<USceneComponent*> arrChildren;
			pRootComponent->GetChildrenComponents( true, arrChildren );
			arrChildren.Add( pRootComponent );
			for( USceneComponent* pChild : arrChildren )
			{
				T* pCast = Cast<T>( pChild );
				if( pCast && filter( pCast ) )
					return pCast;
			}
			return nullptr;
		}

		template <class TFilter=NullFilter>
		static void GetAll( USceneComponent* pRootComponent, TArray<T*>& rOutArray, TFilter filter = NullFilter())
		{
			type_assert( T, USceneComponent ); 
			if( nullptr == pRootComponent )
				return ;

			TArray<USceneComponent*> arrChildren;
			pRootComponent->GetChildrenComponents( true, arrChildren );
			arrChildren.Add( pRootComponent );
			for( USceneComponent* pChild : arrChildren )
			{
				T* pCast = Cast<T>( pChild );
				if( pCast && filter( pCast ) )
					rOutArray.Add( pCast );
			}
		}


		template <class TFilter=NullFilter>
		static T* Get( AActor* pActor, TFilter filter = NullFilter() )
		{
			type_assert( T, UActorComponent ); 
			if( nullptr == pActor )
				return nullptr;
			T* pComponent = pActor->FindComponentByClass<T>();
			return (pComponent && filter(pComponent)) ? pComponent : nullptr;
		}

		template <class TFilter=NullFilter>
		static void GetAll( AActor* pActor, TArray<T*>& rOutArray, TFilter filter = NullFilter() )
		{
			type_assert( T, UActorComponent );
			if( nullptr == pActor )
				return ;

			const TSet<UActorComponent*>& _rCompoenents = pActor->GetComponents();
			for( UActorComponent* pComponent : _rCompoenents )
			{
				T* pCast = Cast<T>( pComponent );
				if( pCast && filter( pCast ) )
					rOutArray.Add( pCast );
			}
		}


		template <class TFilter=NullFilter>
		static void GetActors( const TArray<T*>& rInCompArray, TArray<AActor*>& rOutArray, UClass* pActorFilter=nullptr, TFilter filter = NullFilter())
		{
			for( T* const  _pComp : rInCompArray )
			{
				if( AActor* pOwner = _pComp->GetOwner() )
				{
					if( ( nullptr == pActorFilter || pOwner->IsA(pActorFilter) ) && filter( pOwner ) )
						rOutArray.AddUnique( pOwner );
				}
			}
		}


		template <class TFilter=NullFilter>
		static T* Tag( USceneComponent* pRootComponent, FName tag, TFilter filter = NullFilter())
		{
			return Get( pRootComponent, [&]( T* pComp ){
				return pComp->ComponentHasTag(tag) && filter(pComp); });
		}

		template <class TFilter=NullFilter>
		static void TagAll( USceneComponent* pRootComponent, TArray<T*>& rOutArray, FName tag, TFilter filter = NullFilter())
		{
			return GetAll( pRootComponent, rOutArray, [&]( T* pComp ){
				return  pComp->ComponentHasTag(tag) && filter(pComp); });
		}

		template <class TFilter=NullFilter>
		static void TagAll( AActor* pActor, TArray<T*>& rOutArray, FName tag, TFilter filter = NullFilter() )
		{
			return GetAll( pActor, rOutArray, [&]( T* pComp ){
				return pComp->ComponentHasTag(tag) && filter(pComp); });
		}

	};






//-----------------------------------------------------------------------------------
	template< class T, bool = IsBaseOf<T,APlayerController>::Val >
	struct __ControllerInWorld
	{
		static T* Get( const UWorld* pWorld )
		{
			FConstControllerIterator Iterator = pWorld->GetControllerIterator();
			while( Iterator )
			{
				if( T* pConoller = Cast<T>(*Iterator) )
					return pConoller;
			}
			return nullptr;
		}

		static T* Get( const UWorld* pWorld, int index )
		{
			FConstControllerIterator Iterator = pWorld->GetControllerIterator();
			Iterator += index;
			return Cast<T>(*Iterator);
		}
	};

	template< class T >			// PlayerController
	struct __ControllerInWorld<T,true>
	{
		static T* Get( const UWorld* pWorld )
		{
			FConstPlayerControllerIterator Iterator = pWorld->GetPlayerControllerIterator();
			while( Iterator )
			{
				if( T* pConoller = Cast<T>(*Iterator) )
					return pConoller;
			}
			return nullptr;
		}

		static T* Get( const UWorld* pWorld, int index )
		{
			FConstPlayerControllerIterator Iterator = pWorld->GetPlayerControllerIterator();
			Iterator += index;
			return Cast<T>(*Iterator);
		}
	};


	template< class T, bool = IsBaseOf<T,APlayerController>::Val >
	struct Controller
	{
		static T* Get(const UBehaviorTreeComponent& rBTComponent) 
		{ 
			type_assert(T, AController);
			return Cast<T>(rBTComponent.GetOwner());
		}

		FORCEINLINE
		static T* Get(const UBehaviorTreeComponent* pBTComponent)
		{
			return pBTComponent ? TL::Controller<T>::Get( *pBTComponent ) : nullptr;
		}

		static T* Get(const APawn* pPawn)
		{
			type_assert(T, AController);
			return pPawn ? Cast<T>(pPawn->GetController()) : nullptr;
		}

		FORCEINLINE
		static T* Get( const UWorld* pWorld )
		{
			type_assert(T, AController);
			return pWorld ? __ControllerInWorld<T>::Get( pWorld ) : nullptr;
		}

		FORCEINLINE
		static T* Get( const UWorld* pWorld, int index )
		{
			type_assert(T, AController);
			return pWorld ? __ControllerInWorld<T>::Get( pWorld, index ): nullptr;
		}
	};



//-----------------------------------------------------------------------------------

	template< class T >
	struct Character
	{
		static T* Get( const UBehaviorTreeComponent& rBTComponent )
		{
			type_assert( T, ACharacter );
			AController *pController = Controller<AController>::Get(rBTComponent);
			return pController ? Cast<T>(pController->GetCharacter()) : nullptr;
		}

		static T* Get( const UBehaviorTreeComponent* pBTComponent )
		{
			return pBTComponent ? TL::Character<T>::Get( *pBTComponent ) : nullptr;
		}

		static T* Get( const USkeletalMeshComponent* pSkeletonMesh )
		{
			type_assert(T, ACharacter);
			return pSkeletonMesh ? Cast<T>(pSkeletonMesh->GetOwner()) : nullptr;
		}

		static T* Get( const AController* pController )
		{
			type_assert(T, ACharacter);
			return pController ? Cast<T>(pController->GetPawn()) : nullptr;
		}
	};


//-----------------------------------------------------------------------------------
	template <class T>
	struct GameMode
	{
		FORCEINLINE
		static T* Get( const UWorld* pWorld )
		{
			return pWorld ? Cast<T>(pWorld->GetAuthGameMode()) : nullptr;
		}
	};


	inline UWorld* GetWorld( const AActor* pActor )
	{
		return pActor ? ( !pActor->HasAnyFlags(RF_ClassDefaultObject) ? pActor->GetWorld() : GWorld ) : nullptr;
	}

	// Ref : UEngine::GetWorldFromContextObject
	inline UWorld* GetWorld( const UObject* pObject )
	{
		if( nullptr == pObject )
			return nullptr;
		UWorld* pWorld = pObject->GetWorld();
		if( pWorld )
			return pWorld;

//		if( UObject* pOuter = pObject->GetOuter() )
		for( UObject* pOuter = pObject->GetOuter(); pOuter; pOuter = pOuter->GetOuter() )
		{
			pWorld = pOuter->GetWorld();
			if( pWorld )
				return pWorld;
		}

		bool bSupported = true;
		pWorld = pObject->GetWorldChecked( bSupported );
		return bSupported ? pWorld : GWorld;
	}

	inline bool IsEditorWorld( const UWorld* pWorld )
	{
		return TL::GameMode<AGameMode>::Get( pWorld ) ? false : true;
	}



}

