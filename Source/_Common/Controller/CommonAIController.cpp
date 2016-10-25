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
	AttachToComponent( pCharacter->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale );

	pCharacter->OnPossessAI( this );

	TArray<UJobComponentController*> jControllerArray;
	TL::Job<UJobComponentController>::GetAll( pCharacter, jControllerArray );
	for( UJobComponentController* pJobController : jControllerArray )
	{
		pJobController->Possess( pPawn );
	}
}


void ACommonAIController::UnPossess()
{
	TArray<UJobComponentController*> jControllerArray;
	TL::Job<UJobComponentController>::GetAll( GetPawn(), jControllerArray );
	for( UJobComponentController* pJobController : jControllerArray )
	{
		pJobController->UnPossess();
	}

	Super::UnPossess();
}
