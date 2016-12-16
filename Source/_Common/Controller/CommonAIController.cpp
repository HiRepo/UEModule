#include "_Common.h"
#include "Job/JobBehaviorTree.h"
#include "Job/TL.h"
#include "CommonAIController.h"

void ACommonAIController::Possess( APawn* pPawn )
{
	if( nullptr == pPawn || pPawn->IsPendingKill() )
		return;

	Super::Possess( pPawn );
//	UE_LOG( _Common, Error, TEXT("ACommonAIController::Possess ") );
		
	ACommonCharacter* pCharacter = Cast<ACommonCharacter>( pPawn );
	if( nullptr == pCharacter )
		return;

	pCharacter->OnPossessAI( this );

	TArray<UJobComponentController*> jControllerArray;
	TL::Job<UJobComponentController>::GetAll( pCharacter, jControllerArray );
	for( UJobComponentController* pJobController : jControllerArray )
	{
		pJobController->Possess( pPawn );
	}

	if( GetAttachParentActor() != pCharacter )
		AttachToActor( pCharacter, FAttachmentTransformRules::SnapToTargetNotIncludingScale );
}


void ACommonAIController::UnPossess()
{
	ACommonCharacter* pCharacter = Cast<ACommonCharacter>(	GetPawn() );

	TArray<UJobComponentController*> jControllerArray;
	TL::Job<UJobComponentController>::GetAll( pCharacter, jControllerArray );
	for( UJobComponentController* pJobController : jControllerArray )
	{
		pJobController->UnPossess();
	}

	Super::UnPossess();

	if( nullptr == pCharacter )
		return;
}
