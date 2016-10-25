#pragma once
#include "JobComposite.h"
#include "JobActorComposite.generated.h"

UCLASS(ClassGroup=Job, DefaultToInstanced, Blueprintable, meta=(BlueprintSpawnableComponent))
class _COMMON_API UJobActorComposite : public USceneComponent
{
	GENERATED_BODY()
private :
	friend struct TL::Create<UJobActorComposite, false>;

private :
	UPROPERTY(EditAnywhere, Instanced, Category="Job Actor Component", meta=(DisplayName="Composite"))
	UJobComposite* m_JobComposite		= nullptr;

public :
	void operator()()
	{
		(*m_JobComposite)();
	}

	UJobComposite* GetJobComposite(){ return  m_JobComposite; }

protected :
//		void _init( AActor* pActor ) 
//		{
//			check( pActor );
//	//			_SetOwner( pActor );
//		}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		AActor* pActor = GetOwner();
		m_JobComposite->BeginPlay( pActor );	// Renew Property
	}
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override
	{
		Super::EndPlay( EndPlayReason );
		AActor* pActor = GetOwner();
		m_JobComposite->EndPlay();
	}

	virtual void TickComponent( float fDeltaTime, enum ELevelTick eTickType, FActorComponentTickFunction *ThisTickFunction ) override
	{
		Super::TickComponent( fDeltaTime, eTickType, ThisTickFunction );
		AActor* pActor = GetOwner();
		m_JobComposite->Tick( fDeltaTime );
	}

public :
	UJobActorComposite() 
	{
		PrimaryComponentTick.bCanEverTick = true;		// Enable Tick

#if WITH_EDITOR
//			SetComponentTickEnabled( true );
//			bAutoActivate = true;
		bTickInEditor = true;
		PrimaryComponentTick.TickGroup = TG_DuringPhysics;
#endif


		AActor* pActor = GetOwner();
//		if( pActor )
		{
			m_JobComposite = TL::Create<UJobComposite>::SubInit( this, "JobActorComposite", pActor, this );
//				m_JobComposite->AddToRoot();
		}
	}

	void Append( UJobComponent* pJobComponent )
	{
		m_JobComposite->Append( pJobComponent );
	}

	void Remove( UJobComponent* pJobComponent )
	{
		m_JobComposite->Remove( pJobComponent );
	}

	template<class T>	
	T*	AppendNew( UObject* pOuter )
	{
		return m_JobComposite->AppendNew<T>( pOuter );
	}

	template<class T>
	inline T* FindJob(){ return m_JobComposite->FindJob<T>(); }

	template< class T>
	inline bool FindJob(TArray<T*>& rOutArray )
	{
		return m_JobComposite->FindJob<T>( rOutArray );
	}
};
