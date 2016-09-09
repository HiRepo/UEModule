#pragma once
#include "BaseCamera.h"
#include "SpringArmCamera.generated.h"

UCLASS(Blueprintable,ClassGroup=Camera,meta=(BlueprintSpawnableComponent,ShortTooltip="Evogames SpringArmCamera"))
class _COMMON_API  USpringArmCamera : public UBaseCamera 
{
	GENERATED_BODY()

protected :
	USpringArmComponent* 	m_pSpringArm;

	virtual void 	_Zoom( float fDelta ) override;

public :
	USpringArmCamera() : USpringArmCamera( nullptr ) {}
	USpringArmCamera( AActor* pActor );

	virtual float	GetLength() override;
};

