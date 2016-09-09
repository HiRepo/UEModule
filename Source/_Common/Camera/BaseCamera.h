#pragma once
#include "BaseCamera.generated.h"

UCLASS(ClassGroup=Camera)
class _COMMON_API  UBaseCamera : public UCameraComponent 
{
	GENERATED_BODY()
private :
protected :
	virtual void _Zoom( float fDelta ){}

	USceneComponent* m_pTarget = nullptr;

public :
	UBaseCamera();
	UBaseCamera( AActor* pActor ) : UBaseCamera() {}	

	virtual float GetLength(){	return 0; }

	FORCEINLINE
	void ZoomIn( float fDelta ) 	{ _Zoom( fDelta ); }

	FORCEINLINE
	void ZoomOut( float fDelta )	{ _Zoom(-fDelta ); }

	FORCEINLINE
	void SetActor( AActor* pActor )
	{
		if( pActor && m_pTarget )
    		m_pTarget->AttachToComponent( pActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform );
//	    		m_pTarget->AttachTo( pActor->GetRootComponent(), NAME_None,  EAttachLocation::KeepWorldPosition );
	}
};

