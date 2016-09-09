#pragma once
#include "Job/JobActorComposite.h"

namespace TL{

	UJobActorComposite* GetNotifyJobActorComosite( USkeletalMeshComponent * pMeshComp )
	{
		UJobActorComposite* pJobComposite = TL::Component<UJobActorComposite>::Get( pMeshComp );
 		if( nullptr == pJobComposite )
		{
			pJobComposite = NewObject<UJobActorComposite>( pMeshComp );
			pJobComposite->RegisterComponentWithWorld( pMeshComp->GetWorld() );
			pJobComposite->AttachToComponent( pMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale );
		}
		return pJobComposite;
	}
}
