#pragma once
#include "BaseCamera.h"
#include "SpringArmCamera.generated.h"

UCLASS(Blueprintable,ClassGroup=Camera,meta=(BlueprintSpawnableComponent,ShortTooltip="Evogames SpringArmCamera"))
class _COMMON_API  USpringArmCamera : public UBaseCamera 
{
	GENERATED_BODY()

protected :
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* 	m_pSpringArm = nullptr;

	virtual void 	_Zoom( float fDelta ) override;

public :
	USpringArmCamera() : USpringArmCamera( nullptr ) {}
	USpringArmCamera( AActor* pActor );

	virtual float GetLength() override;
	virtual void  SetLength( float fLength ) override;

 	void CameraAttachment( USceneComponent* pParent )
	{
		m_pSpringArm->SetupAttachment( pParent );
	}
};

