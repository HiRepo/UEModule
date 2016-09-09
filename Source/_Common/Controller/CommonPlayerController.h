#pragma once
#include "GameFramework/PlayerController.h"
#include "CommonPlayerController.generated.h"

class ABaseCameraActor;

UCLASS(placeable)
class _COMMON_API ACommonPlayerController : public APlayerController
{
	GENERATED_BODY()

	ABaseCameraActor* m_pCamera 	= nullptr;

protected :

private :
	void OnXAxis(float AxisValue);
	void OnYAxis(float AxisValue);

	void OnMouseLeftPressed();
	void OnMouseLeftReleased();

	void _PossessCamera();

public:
	ACommonPlayerController();

	FORCEINLINE
	ABaseCameraActor* GetCamera(){ return m_pCamera; }

	void SetCamera(ABaseCameraActor* pCamera );

	virtual void BeginPlay() override;

	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

	virtual void Tick( float fTimeDelta ) override;

	virtual void SetupInputComponent() override;
	
	virtual void Possess( APawn* pPawn ) override
	{
		Super::Possess( pPawn );
//			UE_LOG( _Common, Error, TEXT("ACommonPlayerController::Possess ") );

		pPawn = GetPawn();
		if( nullptr == pPawn )
			return ;
		
		if( ACommonCharacter* pCharacter = Cast<ACommonCharacter>(pPawn) )
			AttachToComponent( pCharacter->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale );

		if( m_pCamera )
			_PossessCamera();
	}

	virtual void UnPossess() override;
//		virtual void SetViewTargetWithBlend(class AActor* NewViewTarget, float BlendTime = 0, enum EViewTargetBlendFunction BlendFunc = VTBlend_Linear, float BlendExp = 0, bool bLockOutgoing = false);
//		virtual void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams());
};
