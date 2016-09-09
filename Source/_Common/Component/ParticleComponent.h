#pragma once
#include "Job/JobAniTransform.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParticleComponent.generated.h"

UCLASS(ClassGroup=(Common,Component))
class _COMMON_API UParticleComponent : public UParticleSystemComponent
{
	GENERATED_BODY()
friend class UParticleEffectNotify;

private :
	FAniTransform* m_pAniTransform = nullptr;

	void __RemoveComponentInAniTransform()
	{
		if( m_pAniTransform )
		{
			m_pAniTransform->RemoveComponent( this );
			m_pAniTransform->RemoveComponent( nullptr );
		}
	}

protected :
	virtual void TickComponent( float fDeltaTime, ELevelTick eTickType, FActorComponentTickFunction* pThisTickFunction ) override
	{
		Super::TickComponent( fDeltaTime, eTickType, pThisTickFunction );

		if( nullptr == m_pAniTransform )
			return;
		if( bWasCompleted )
			m_pAniTransform->RemoveComponent( this );
	}

	virtual void OnComponentDestroyed( bool bDestroyingHierarchy ) override
	{
		Super::OnComponentDestroyed( bDestroyingHierarchy );
		__RemoveComponentInAniTransform();
	}

	virtual void OnRegister() override
	{
		Super::OnRegister();
	}
	virtual void OnUnregister()  override
	{
		Super::OnUnregister();
	}

	virtual void BeginDestroy() override
	{
		Super::BeginDestroy();
//			__RemoveComponentInAniTransform();
	}

	virtual void Deactivate() override
	{
		Super::Deactivate();
	}
//		virtual void BeginPlay() override
//		{
//			Super::BeginPlay();
//		}
//		virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override
//		{
//			Super::EndPlay( EndPlayReason );
//		}
};
