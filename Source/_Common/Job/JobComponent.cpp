#include "_Common.h"
#include "JobComponent.h"
#include "JobComposite.h"
#include "Util/CommonLib.h"

//==============	UJobComponent	===============================================
void UJobComponent::_SetComposite( UJobComposite* pComposite )
{ 
	m_pJobComposite = pComposite; 
	if(	pComposite )
		_SetOwner( pComposite->GetOwner() );
}

UJobComposite* UJobComponent::GetRootComposite() 
{ 
	UJobComposite* pNextComposite = m_pJobComposite;
	while( pNextComposite )
	{
		pNextComposite = m_pJobComposite->GetComposite();
		if( nullptr == pNextComposite )
			return m_pJobComposite; 
	}
	return nullptr;
}


//==============	UJobComposite	===============================================

void UJobComposite::Append( TSubclassOf<UJobComponent> pJobClass )
{
	check( pJobClass )
	UJobComponent* pComponent = TL::Create<UJobComponent>::Init( this, pJobClass, pJobClass->GetFName() );
	if( pComponent )
		Append( pComponent );
}


//==============	UJobTimeDilation	===============================================/
#include "JobActorComposite.h"
#include "JobTimeDilation.h"

void UJobTimeDilation::_UpdateCurTimeDilation()
{
	if( AActor* pOwner = GetOwner() )
	{
		pOwner->CustomTimeDilation  = m_fTimeDilation;

		if( USkeletalMeshComponent* pSkeleton = TL::Component<USkeletalMeshComponent>::Get( pOwner ) )
			pSkeleton->GlobalAnimRateScale = m_fTimeScaleAnim;
	}
#if WITH_EDITOR
	else
	if( UJobActorComposite* pJobActorComposite = GetRootComposite()->GetJobActorComposite() )
	{
		UWorld* pWorld = pJobActorComposite->GetWorld();
		if( TL::IsEditorWorld( pWorld ) )
			UCommonLib::SetTimeDilation( pWorld, m_fTimeDilation );

		USkeletalMeshComponent* pSkeleton = Cast<USkeletalMeshComponent>( pJobActorComposite->GetAttachParent() );
		check( pSkeleton );
		pSkeleton->GlobalAnimRateScale = m_fTimeScaleAnim;
	}
#endif	

}

//==============	UJobBehaviorTree	===============================================
#include "JobBehaviorTree.h"
#include "AIController.h"

bool UJobBehaviorTree::ChangeBT( int index )
{
	if( false == BTDataArray.IsValidIndex( index ) )		// if Not search
		return false;										// Init BT DataIndex == 0
	// KHI_WARN index

	APawn* pPawn = GetPawn();
	FBTData& rBTData = BTDataArray[index];
	AAIController* pAIController = TL::Controller<AAIController>::Get( pPawn );
	if( nullptr == pAIController || nullptr == rBTData.AssetBT )
		return false;

	if(	UBrainComponent* pCompBrain = pAIController->GetBrainComponent() )
		pCompBrain->StopLogic( "Change BT" );

	if( UJobDataBB* pJBB = rBTData.GetJobDataBB( this, pPawn ) )
		pJBB->UseBlackboard( pAIController );

	m_iCurBT = index;
	pAIController->RunBehaviorTree( rBTData.AssetBT );
	auto* pBTComp =	Cast<UBehaviorTreeComponent>( pAIController->GetBrainComponent() );
	rBTData.BTComp = pBTComp;
	if( false == m_bPlaying )
		pBTComp->StopTree(EBTStopMode::Safe);

	return true;
}


