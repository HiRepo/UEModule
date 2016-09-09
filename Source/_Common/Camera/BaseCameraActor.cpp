#include "_Common.h"
#include "BaseCameraActor.h"
#include "SpringArmCamera.h"

ABaseCameraActor::ABaseCameraActor(class FObjectInitializer const &)
{
    m_pBaseCamera = CreateDefaultSubobject<USpringArmCamera>(TEXT("SpringArmCamera"));
}


void ABaseCameraActor::PostInitProperties()
{
	Super::PostInitProperties();
}


#if WITH_EDITOR		
void ABaseCameraActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	
	Super::PostEditChangeProperty( PropertyChangedEvent );
}

#endif
