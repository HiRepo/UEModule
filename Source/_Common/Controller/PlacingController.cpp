#include "_Common.h"
#include "PlacingController.h"
#include "EngineUtils.h"
#include "CommonPlayerController.h"
#include "Engine.h"


APlacingController::APlacingController()
{
	int kk = KK;
	CommonPlayerControllerClass = ACommonPlayerController::StaticClass();
}

APlacingController::APlacingController(const FObjectInitializer& ObjectInitializer )
: Super(ObjectInitializer)
{
	ACameraActor* cameraActor = FindObjectFast<ACameraActor>( GetWorld(), TEXT("MyCameraActor"));

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	int kk = KK;
	CommonPlayerControllerClass = ACommonPlayerController::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	SetActorTickEnabled(true);
}


void APlacingController::BeginPlay()
{
	Super::BeginPlay();
	int kk = KK;
}

void APlacingController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}




void APlacingController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	APlayerController* pPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ACommonPlayerController* pPlayerController = Cast<ACommonPlayerController>(pPlayer);
//		if (pPlayerController)
//			pPlayerController->SetCamera(pPlayerController->GetCamera());

	for( FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator )
	{
		pPlayer = *Iterator;
	}

////////////////////////////////////////////////////////////////////
	FString sScreen;
	static int count = 0;
	APlayerController* basePlayerController = FindObject<APlayerController>(GetWorld(), TEXT("MyCameraActor"));
	if (!basePlayerController)
	{
		for (TActorIterator<APlayerController> It(GetWorld()); It; ++It)
		{
			APlayerController* pa = *It;
			if (pa->GetName() != TEXT("_BasePlayerController") )
			{
				basePlayerController = pa;
			}
		}
	}
	if (basePlayerController->GetPawn())
	{
		sScreen.Append(FString::Printf(TEXT("%d BasePlayerController Component's Pawn Name %s  \n"), ++count, *basePlayerController->GetPawn()->GetName()));
		sScreen.Append(FString::Printf(TEXT("%d BasePlayerController Component's Actor %s  \n"), ++count, *basePlayerController->GetRootComponent()->GetOwner()->GetName()));
		sScreen.Append(FString::Printf(TEXT("%d BasePlayerController Component's Pawn %s  \n"), ++count, *basePlayerController->GetPawn()->GetActorLocation().ToString()));
		sScreen.Append(FString::Printf(TEXT("%d BasePlayerController Target Location %s \n"), ++count, *(basePlayerController->GetTargetLocation().ToString())));
		sScreen.Append(FString::Printf(TEXT("%d BasePlayerController Component Location %s \n"), ++count, *(basePlayerController->GetRootComponent()->GetComponentLocation().ToString())));
	}
	if (count == 1000)
	{
		AGameModeBase* pGameMode = UGameplayStatics::GetGameMode(GetWorld());
		//pGameMode->InitNew
		//basePlayerController

		//	->SwapPlayerControllers( )

	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, sScreen);
	

//@@@@@@@@@@@@@@@		Array Remove Test 	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//		FString str;
//		TArray<int> iArray;	
//		iArray.Push( 1 );
//		iArray.Push( 2 );
//		iArray.Push( 3 );
//		iArray.Push( 4 );
//		iArray.Push( 5 );
//		iArray.Push( 6 );
//	
//		int c = 0;
//	//	for( int i : iArray )
//		for( int i= 0; i < iArray.Num(); ++i )
//		{
//			int c = iArray[i];
//			if( c == 3 || c == 6 )
//				iArray.RemoveAt( i );
//	//			++i;
//		}
//		for( int i : iArray )
//		{
//			str.Append( FString::FromInt(i) );
//			str.Append( " " );
//		}
//		str.Append( "\n" );
//	
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, str);
//	
}



void APlacingController::PostLoad()
{
	Super::PostLoad();
	int kk = KK;
	UClass* pClass = CommonPlayerControllerClass;


}


void APlacingController::PostDuplicate(bool bDuplicateForPIE)
{

	Super::PostDuplicate(bDuplicateForPIE);
	int kk = KK;
	UClass* pClass = CommonPlayerControllerClass;


}


void APlacingController::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{

	Super::PostLoadSubobjects(OuterInstanceGraph);
	int kk = KK;
	UClass* pClass = CommonPlayerControllerClass;


}


void APlacingController::PostActorCreated()
{
	Super::PostActorCreated();
	int kk = KK;
	UClass* pClass = CommonPlayerControllerClass;
}


void APlacingController::PostInitProperties()
{
	Super::PostInitProperties();
	int kk = KK;
	UClass* pClass = CommonPlayerControllerClass;


}
	
#if WITH_EDITOR													// 에디터에서 즉시 변경사항 수정을 위해
void APlacingController::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)		
{
	int kk = KK;
	UClass* pClass = CommonPlayerControllerClass;
	//FTransform(const FRotator& InRotation, const FVector& InTranslation
	const FTransform _transform( GetTransform().GetRotation(), GetTransform().GetLocation() );
	GetWorld()->SpawnActor<ACommonPlayerController>(CommonPlayerControllerClass, _transform );

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
