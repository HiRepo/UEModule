#include "_Common.h"
#include "Engine.h"
#include "Camera/BaseCameraActor.h"
#include "Camera/SpringArmCamera.h"
#include "CommonAIController.h"
#include "CommonPlayerController.h"

ACommonPlayerController::ACommonPlayerController()
{
	if( UWorld* pWorld = GetWorld() )
	{
		ABaseCameraActor* pCamera = Cast<ABaseCameraActor>(pWorld->SpawnActor(ABaseCameraActor::StaticClass()));
#if WITH_EDITOR
		pCamera->SetActorLabel("_BaseCameraActor");
#endif
		USceneComponent* pCameraRootComponent =	pCamera->GetRootComponent();
		if( false == pCameraRootComponent->IsRegistered() )
   			pCameraRootComponent->SetupAttachment( GetRootComponent() );
	}

	SetAsLocalPlayerController();

	// Mouse Settig
	bEnableClickEvents = true;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

}


//		virtual void PostInitializeComponents() override;
//	void ACommonPlayerController::PostInitializeComponents()
//	{
//		Super::PostInitializeComponents();;
//	}

void ACommonPlayerController::_PossessCamera()
{
	check(m_pCamera);
    m_pCamera->AttachToComponent( GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale );

	m_pCamera->SetActorRelativeLocation(FVector::ZeroVector);

	SetViewTarget(m_pCamera);
}

void ACommonPlayerController::SetCamera(ABaseCameraActor* pCamera )
{	
	check(pCamera);
	m_pCamera = pCamera;
	_PossessCamera();
}

void ACommonPlayerController::BeginPlay()
{
	Super::BeginPlay();
	APawn* pPawn = GetPawn();
}
void ACommonPlayerController::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	Super::EndPlay( EndPlayReason );
}

void ACommonPlayerController::Tick( float fTimeDelta )
{
	Super::Tick( fTimeDelta );
}

void ACommonPlayerController::UnPossess()
{
	ACommonCharacter* pCharacter = Cast<ACommonCharacter>(	GetPawn() );

	Super::UnPossess();
	
	if( nullptr == pCharacter )
		return;

	// Default AI Controller Possesss
	pCharacter->PossessedFromAI();
}

void ACommonPlayerController::OnMouseLeftPressed()
{
	//SetViewTarget(this);
	static ACharacter* s_oldCharacter = Cast<ACharacter>(GetPawn());
	ACharacter* pCharacter = FindObjectFast<ACharacter>(GetWorld(), TEXT("_NewCharacter"));
	pCharacter = FindObject<ACharacter>(GetWorld(), TEXT("_NewCharacter"));
	if( !pCharacter )
	{
		for (TActorIterator<ACharacter> It(GetWorld()); It; ++It)
		{
			ACharacter* pCA = *It;
			if (pCA->GetName() == TEXT("_NewCharacter"))
				pCharacter = pCA;
		}
	}
	if (pCharacter)
	{
		if( GetPawn() != pCharacter )
			Possess(pCharacter);
		else
			Possess(s_oldCharacter);
	}

}

void ACommonPlayerController::OnMouseLeftReleased()
{

}

void ACommonPlayerController::OnXAxis(float AxisValue)
{
	float value	 = FMath::Clamp(AxisValue, -1.0f, 1.0f);

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	APawn* pPawn = GetPawn();
	if( pPawn )
		pPawn->AddMovementInput(Direction, value);

}
void ACommonPlayerController::OnYAxis(float AxisValue)
{
	float value = FMath::Clamp(AxisValue, -1.0f, 1.0f);

	// find out which way is right
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// add movement in that direction
	APawn* pPawn = GetPawn();
	if (pPawn)
		pPawn->AddMovementInput(Direction, value);

}


void ACommonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

//			InputComponent->BindAction("SetDestination", IE_Pressed, this, &ACommonPlayerController::OnMouseLeftPressed);
//			InputComponent->BindAction("SetDestination", IE_Released, this, &ACommonPlayerController::OnMouseLeftReleased);
//	
//			//// keyboard move action
//			InputComponent->BindAxis("MoveForward", this, &ACommonPlayerController::OnXAxis);
//			InputComponent->BindAxis("MoveRight", this, &ACommonPlayerController::OnYAxis);
//	

	//// keyboard action
	//InputComponent->BindAction("AttackAction", IE_Pressed, this, &AProject_EPlayerController::OnSetAttackAction);
	////InputComponent->BindAction("ChangeHero", IE_Pressed, this, &AProject_EPlayerController::OnChangeHero);



	//// support touch devices
	////InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AProject_EPlayerController::MoveToTouchLocation);
	////InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AProject_EPlayerController::MoveToTouchLocation);

	//InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AProject_EPlayerController::CameraZoomPressed);
	//InputComponent->BindTouch(EInputEvent::IE_Released, this, &AProject_EPlayerController::CameraZoomReleased);

}


