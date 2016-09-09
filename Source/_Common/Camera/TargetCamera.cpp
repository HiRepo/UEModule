#include "_Common.h"
#include "TargetCamera.h"


UTargetCamera::UTargetCamera( AActor* pActor )
:  UBaseCamera( pActor ) 
{
    m_pTarget = CreateDefaultSubobject<USceneComponent>(TEXT("TargetComponent"));
	SetupAttachment( m_pTarget );
	if( pActor )
	{
 	 	m_pTarget->AttachToComponent( pActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform );
	}
}



void UTargetCamera::_Zoom( float fDelta )
{

}

