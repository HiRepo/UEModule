#include "_Common.h"
#include "Engine.h"
#include "SVirtualJoystick.h"
#include "Camera/BaseCameraActor.h"
#include "Camera/SpringArmCamera.h"
#include "Actor/CommonCharacter.h"
#include "CommonAIController.h"
#include "CommonPlayerController.h"

ACommonPlayerController::ACommonPlayerController()
{
	bFindCameraComponentWhenViewTarget = true;
	//bTakeCameraControlWhenPossessed = true;

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

void ACommonPlayerController::Possess( APawn* pPawn )
{
	Super::Possess( pPawn );
		UE_LOG( _Common, Error, TEXT("ACommonPlayerController::Possess ") );

	pPawn = GetPawn();
	if( nullptr == pPawn )
		return ;

	if( m_pCamera )
		_PossessCamera();
	else
		SetViewTarget( this );		

	if( ACommonCharacter* pCharacter = Cast<ACommonCharacter>(pPawn) )
		AttachToActor( pCharacter, FAttachmentTransformRules::SnapToTargetNotIncludingScale );
}

void ACommonPlayerController::SetCamera( ABaseCameraActor* pCamera )
{	
	check(pCamera);
	m_pCamera = pCamera;
	_PossessCamera();
}

void ACommonPlayerController::BeginPlay()
{
//		if( UWorld* pWorld = GetWorld() )
//		{
//			ABaseCameraActor* pCamera = Cast<ABaseCameraActor>(pWorld->SpawnActor(ABaseCameraActor::StaticClass()));
//	#if WITH_EDITOR
//			pCamera->SetActorLabel("_BaseCameraActor");
//	#endif
//			USceneComponent* pCameraRootComponent =	pCamera->GetRootComponent();
//			pCamera->AttachToActor( this, FAttachmentTransformRules::SnapToTargetNotIncludingScale );
//			m_pCamera = pCamera;
//		}

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

	// ReAttach  
	AttachToPawn( pCharacter );

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


DECLARE_DELEGATE( FTouchStartDelegate );
DECLARE_DELEGATE( FTouchEndDelegate );

class SNewVirtualJoystick : public SVirtualJoystick
{
friend class ACommonPlayerController;
private :
	FTouchStartDelegate m_TouchStartDelegate; 
	FTouchEndDelegate   m_TouchEndDelegate;

	virtual FReply OnTouchStarted( const FGeometry& MyGeometry, const FPointerEvent& Event ) override
	{
		FReply reply = SVirtualJoystick::OnTouchStarted( MyGeometry, Event );
//			if( reply =! FReply::Unhandled() )
			m_TouchStartDelegate.Execute(); 
		return reply;
	}
	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& Event) override
	{
		FReply reply = SVirtualJoystick::OnTouchEnded( MyGeometry, Event );
//			if( reply =! FReply::Unhandled() )
			m_TouchEndDelegate.Execute(); 
		return reply;
	}
};

void ACommonPlayerController::CreateTouchInterface() 		// Copy From Version 14.1 
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);

	// do we want to show virtual joysticks?
	if (LocalPlayer && LocalPlayer->ViewportClient && SVirtualJoystick::ShouldDisplayTouchInterface())
	{
		// in case we already had one, remove it
		if (VirtualJoystick.IsValid())
		{
			Cast<ULocalPlayer>(Player)->ViewportClient->RemoveViewportWidgetContent(VirtualJoystick.ToSharedRef());
		}

		if (CurrentTouchInterface == nullptr)
		{
			// load what the game wants to show at startup
			FStringAssetReference DefaultTouchInterfaceName = GetDefault<UInputSettings>()->DefaultTouchInterface;

			if (DefaultTouchInterfaceName.IsValid())
			{
				// activate this interface if we have it
				CurrentTouchInterface = LoadObject<UTouchInterface>(NULL, *DefaultTouchInterfaceName.ToString());
			}
		}

		if (CurrentTouchInterface)
		{
			// create the joystick 
			TSharedPtr<SNewVirtualJoystick> joystick = SNew(SNewVirtualJoystick);
			joystick->m_TouchStartDelegate.BindUObject( this, &ACommonPlayerController::OnJoystickTouchStart );
			joystick->m_TouchEndDelegate.BindUObject( this, &ACommonPlayerController::OnJoystickTouchEnd );
			VirtualJoystick = joystick;

			// add it to the player's viewport
			LocalPlayer->ViewportClient->AddViewportWidgetContent(VirtualJoystick.ToSharedRef());

			ActivateTouchInterface(CurrentTouchInterface);
		}
	}
}
