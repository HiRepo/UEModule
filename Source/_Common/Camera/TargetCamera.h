#pragma once
#include "BaseCamera.h"
#include "TargetCamera.generated.h"

UCLASS(Blueprintable,ClassGroup=Camera,meta=(BlueprintSpawnableComponent,ShortTooltip="Evogames Target Camera"))
class _COMMON_API  UTargetCamera : public UBaseCamera 
{
	GENERATED_BODY()

protected :


protected :
	virtual void _Zoom( float fDelta ) override;

public :
	UTargetCamera() : UTargetCamera( nullptr ){}
	UTargetCamera( AActor* pActor );

};

