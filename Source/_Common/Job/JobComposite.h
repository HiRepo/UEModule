#pragma once
#include "JobComponent.h"
#include "JobComposite.generated.h"

class UJobActorComposite;

UCLASS()
class _COMMON_API UJobComposite : public UJobComponent
{
	GENERATED_BODY()
private :
	friend struct TL::Create<UJobComposite,false>;

private :
	UPROPERTY(EditAnywhere, Category="JobComponent", meta=(DisplayName=""))
	TArray<TSubclassOf<UJobComponent>> m_pJobClassArray;

//	UPROPERTY( VisibleAnywhere, Transient, Category="JobComponent" )
	UPROPERTY(EditAnywhere, Category="JobComponent" )
	TArray<UJobComponent*>  m_Components;

	UPROPERTY(Transient)
	UJobActorComposite* m_pJobActorComposite = nullptr;

public :
	UJobActorComposite* GetJobActorComposite(){ return  m_pJobActorComposite; }


protected :
	void _init( AActor* pActor, UJobActorComposite* pJobActorComposite = nullptr ) 
	{
//			check( pActor );
		_SetOwner( pActor );

		m_pJobActorComposite = pJobActorComposite;
	}

	virtual void PostInitProperties() override					// Property값이 설정이 끝난후 처리해야할 작업
	{
		Super::PostInitProperties();
	}


	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override
	{
		m_Components.Remove( nullptr );
		m_pJobClassArray.Remove( nullptr );
		for( UClass* pClass : m_pJobClassArray )
		{
			if( nullptr == FindJob( pClass ) )
				Append( pClass );
		}
	}


	virtual void _SetOwner( AActor* pOwner ) override
	{
		Super::_SetOwner( pOwner );
		for (UJobComponent* pComponent : m_Components)
			pComponent->_SetOwner( pOwner );
	}

	virtual void OnAttach() override
	{
		Super::OnAttach();
		for (UJobComponent* pComponent : m_Components)
			pComponent->OnAttach();
	}

	virtual void OnDetach() override
	{
		Super::OnDetach();
		for (UJobComponent* pComponent : m_Components)
			pComponent->OnDetach();
	}




public :
	UJobComposite()
	{ 
		m_EnableTick = true; 

	}
//		UJobComposite( AActor* pActor ){ _init( pActor ); }




 	UJobComponent* operator[]( int idx ) 
 	{ 
		check( m_Components.IsValidIndex( idx ) );
 		return m_Components[idx]; 
 	}



	virtual void operator()() override
	{
		Super::operator()();
		for (UJobComponent* pComponent : m_Components)
			(*pComponent)();
	}

	virtual void Tick( float fDeltaTime ) override
	{
		Super::Tick( fDeltaTime );
		for (UJobComponent* pComponent : m_Components)
		{
			if( pComponent->m_EnableTick )
				pComponent->Tick( fDeltaTime );
		}
	}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		for (UJobComponent* pComponent : m_Components)
			pComponent->BeginPlay();
	}

	void BeginPlay( AActor* pOwner )
	{
		m_pOwner = pOwner;			// Renew Properties
		for (UJobComponent* pComponent : m_Components)
			pComponent->_SetComposite( this );			
		BeginPlay();
	}


	virtual void EndPlay() override
	{
		Super::EndPlay();
		for (UJobComponent* pComponent : m_Components)
			pComponent->EndPlay();
	}

	UFUNCTION( BlueprintCallable, Category = "Job" )
	void Append( TSubclassOf<UJobComponent> pJobClass );

	void Append( UJobComponent* pJobComponent )
	{
		m_Components.Push( pJobComponent );
		pJobComponent->_SetComposite( this );
		pJobComponent->OnAttach();
	}

	void Remove( UJobComponent* pJobComponent )
	{
		pJobComponent->OnDetach();
		pJobComponent->_SetComposite( nullptr );
		m_Components.Remove( pJobComponent );
	}


	UFUNCTION( BlueprintCallable, Category = "Job" )
	UJobComponent* FindJob( UClass* pClass )
	{
		if( pClass == nullptr )
			return nullptr;	
		for( UJobComponent* pJob : m_Components )
		{
			if( pJob->GetClass() == pClass )
				return pJob;
		}
		return nullptr;
	}


	template<class T>	
	T*	AppendNew( UObject* pOuter )
	{
		T* pJobComp =  TL::Create<T>::Init( pOuter );
		Append( pJobComp );
		return pJobComp;
	}

	template<class T>
	T*	FindJob()
	{
		type_assert(T, UJobComponent);
		return (T*)FindJob(T::StaticClass());
	}

//		UFUNCTION( BlueprintCallable, Category = "Job" )
//		bool FindJob( UClass* pClass, TArray<JobComponent*>& oJobArray )
//		{
//			if( pClass == nullptr )
//				return nullptr;	
//	
//			oJobArray.Num();
//			for( UJobComponent* pJob : m_Components )
//			{
//				if( pJob->GetClass().IsA( pClass ) )
//					pJob;
//			}
//			return nullptr;
//		}

	template< class T>
	bool FindJob( TArray<T*>& rOutArray )
	{
		type_assert(T, UJobComponent);

		const int _arrayNum = rOutArray.Num();
		for (UJobComponent* pComponent : m_Components)
		{
			if (T* pCast = Cast<T>(pComponent))
				rOutArray.Add(pCast);

		}
		return _arrayNum != rOutArray.Num();
	}
};
