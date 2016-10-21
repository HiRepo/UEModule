#include "_COMMON.h"
#include "Job/JobComposite.h"
#include "Job/JobMaterial.h"
#include "Job/JobTimeDilation.h"
#include "Job/JobAniTransform.h"
#include "Job/JobBehaviorTree.h"
#include "Controller/CommonAIController.h"
#include "CommonCharacter.h"


ACommonCharacter::ACommonCharacter()
{
	m_JobComposite = TL::Create<UJobComposite>::SubInit( this, "JobComposite", this );

	m_JobMaterial = TL::Create<UJobMaterial>::SubInit( this, "JobMaterial" );
	m_JobComposite->Append( m_JobMaterial );

	m_JobTimeDilation = TL::Create<UJobTimeDilation>::SubInit( this, "JobTimeDilation" );
	m_JobComposite->Append( m_JobTimeDilation );

	m_JobAniTransform = TL::Create<UJobAniTransform>::SubInit( this, "JobAniTransform" );
	m_JobComposite->Append( m_JobAniTransform );

	m_JobBehaviorTree = TL::Create<UJobBehaviorTree>::SubInit( this, "JobBehaviorTree" );
	m_JobComposite->Append( m_JobBehaviorTree );

	m_JobMaterial->AppendMatData();				// Append Default FJobMat
}

//	void ACommonCharacter::PostInitializeComponents()
//	{
//		Super::PostInitializeComponents();
//		m_JobMaterial->Append();
//	}

void ACommonCharacter::PostLoadSubobjects( FObjectInstancingGraph* pOuterInstanceGraph )
{
	Super::PostLoadSubobjects( pOuterInstanceGraph );

	m_JobMaterial->RebuildMat( true );

	m_JobMaterial->SetSourceFromMaterial( 0 );	// Default FJobMat's source data is initialized from Material.
}


#if WITH_EDITOR	
void ACommonCharacter::PostEditChangeChainProperty( struct FPropertyChangedChainEvent& rPropertyChangedEvent )
{
	if( "m_JobMaterial" != rPropertyChangedEvent.Property->GetFName() )
		return Super::PostEditChangeChainProperty( rPropertyChangedEvent );

	Super::PostEditChangeChainProperty( rPropertyChangedEvent );
}
#endif


void ACommonCharacter::Tick(float fDeltaTime)
{
	Super::Tick( fDeltaTime );
	m_JobComposite->Tick( fDeltaTime );
}


void ACommonCharacter::BeginPlay()
{
	Super::BeginPlay();
	m_JobComposite->BeginPlay( this );			// Renew Property
}


void ACommonCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay( EndPlayReason );
	m_JobComposite->EndPlay();
}


void ACommonCharacter::PossessedFromAI( ACommonAIController* pAIController )
{
	ACommonAIController* pController = pAIController ? pAIController : m_AIController;
	if( pController )
		pController->Possess( this );
}

