#pragma once
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Job/JobTimeDilation.h"
#include "Actor/CommonCharacter.h"
#if WITH_EDITOR
#include "Edit/TL.h"
#endif 
#include "TimeDilationState.generated.h"

UCLASS(MinimalAPI, meta = (DisplayName = "TimeDilation State"))
class UTimeDilationNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public :
	UTimeDilationNotifyState()
	{
		TimeDilation.SetLifeCycle( ELifeCycle::Infinity );
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Time Speed"))
	FTimeDilation TimeDilation;

protected :

#if WITH_EDITOR
	UJobTimeDilation* m_pJobTimeDilation = nullptr;
#endif

	virtual void NotifyBegin( USkeletalMeshComponent * pMeshComp, class UAnimSequenceBase * pAnimation, float fTotalDuration ) override
	{
		ACommonCharacter* pCharacter = TL::Character<ACommonCharacter>::Get( pMeshComp );
		if( pCharacter )
			pCharacter->GetJobTimeDilation().AppendExtern( &TimeDilation );
#if WITH_EDITOR
		else
		if( !pMeshComp->GetOwner() )
		{
			UJobActorComposite* pJobComposite = TL::GetNotifyJobActorComosite( pMeshComp );

			m_pJobTimeDilation = pJobComposite->FindJob<UJobTimeDilation>();
			if( nullptr == m_pJobTimeDilation )
				m_pJobTimeDilation = pJobComposite->AppendNew<UJobTimeDilation>( pJobComposite );

			if( m_pJobTimeDilation && false == m_pJobTimeDilation->IsContainExtern( &TimeDilation ) )
				m_pJobTimeDilation->AppendExtern( &TimeDilation );
		}
#endif
	}


	virtual void NotifyEnd( USkeletalMeshComponent* pMeshComp, class UAnimSequenceBase* pAnimation ) override
	{
		ACommonCharacter* pCharacter = TL::Character<ACommonCharacter>::Get( pMeshComp );
		if( pCharacter )
			pCharacter->GetJobTimeDilation().RemoveExtern( &TimeDilation );
#if WITH_EDITOR
		else
		if( !pMeshComp->GetOwner() )
		{
			if( m_pJobTimeDilation )
				m_pJobTimeDilation->RemoveExtern( &TimeDilation );
		}
#endif
	}


//	virtual void NotifyTick( USkeletalMeshComponent* pMeshComp, class UAnimSequenceBase* pAnimation, float fFrameDeltaTime ) override;

};
