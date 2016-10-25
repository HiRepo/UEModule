#include "_Common.h"
#include "AI/TL.h"
#include "Job/TL.h"
#include "Job/JobBehaviorTree.h"
#include "Job/JobActorComposite.h"
#include "Actor/CommonCharacter.h"
#include "CommonLib.h"



//--------------		Character 	----------------------------------------------------
ACommonCharacter* UCommonLib::GetCommonCharacter( UObject* pObject )
{
	if( nullptr == pObject )
		return nullptr;
	UStruct* pClass = pObject->GetClass();

	ACommonCharacter* pCharacter = nullptr;
	if( pClass->IsChildOf( UBehaviorTreeComponent::StaticClass() ) )
		pCharacter = TL::Character<ACommonCharacter>::Get( (UBehaviorTreeComponent*)pObject );
	else
	if( pClass->IsChildOf( USkeletalMeshComponent::StaticClass() ) )
		pCharacter = TL::Character<ACommonCharacter>::Get( (USkeletalMeshComponent*)pObject );
	else
	if( pClass->IsChildOf( AController::StaticClass() ) )
		pCharacter = TL::Character<ACommonCharacter>::Get( (AController*)pObject );
	else
	if( pClass->IsChildOf( APawn::StaticClass() ) )
		pCharacter = Cast<ACommonCharacter>( pObject );
	// KHI_ERROR
	return pCharacter;
}


//--------------		Job 	----------------------------------------------------
UJobActorComposite* UCommonLib::GetJobActorComposite( UWorld* pWorld, FString sActorName )
{
	AActor* pActor = TL::Actor<AActor>::GetByName( sActorName, pWorld  );
	if( nullptr == pActor )
		return nullptr;
	return TL::Component<UJobActorComposite>::Get( pActor );
}

UJobMaterial* UCommonLib::GetJobMaterial( UJobActorComposite* pJobActorComposite )
{
	if( UJobComposite* pJobComposite = pJobActorComposite->GetJobComposite() )
		return pJobComposite->FindJob<UJobMaterial>();
	return nullptr;
}



//--------------		Black Board 	----------------------------------------------------

UJobBehaviorTree* UCommonLib::GetJobBehaviorTree( APawn* pPawn )
{
	return TL::Job<UJobBehaviorTree>::Get( pPawn );
}


UBlackboardComponent* UCommonLib::__GetBlackboard( UObject* pObject )
{
	UStruct* pClass = pObject->GetClass();
	if( pClass->IsChildOf( AActor::StaticClass() ) )
		return TL::AI<UBlackboardComponent>::GetBlackboard( (AActor*)pObject );
	else
	if( pClass->IsChildOf( UBehaviorTreeComponent::StaticClass() ) )
		return TL::AI<UBlackboardComponent>::GetBlackboard( (UBehaviorTreeComponent*)pObject );
	else
	if( pClass->IsChildOf( UJobBehaviorTree::StaticClass() ) )
		return TL::AI<UBlackboardComponent>::GetBlackboard( (UJobBehaviorTree*)pObject );
	// KHI_ERROR
	return nullptr;
}

AActor* UCommonLib::GetBlackboardValueAsActor( const UBlackboardComponent* pBB, FName KeyName )
{
	return pBB ? Cast<AActor>(pBB->GetValueAsObject( KeyName )) : nullptr;
}

UObject* UCommonLib::GetBlackboardValueAsObject( const UBlackboardComponent* pBB, FName KeyName )
{
	return pBB ? pBB->GetValueAsObject( KeyName ) : nullptr;
}

UClass* UCommonLib::GetBlackboardValueAsClass( const UBlackboardComponent* pBB, FName KeyName )
{
	return pBB ? pBB->GetValueAsClass( KeyName ) : nullptr;
}

uint8 UCommonLib::GetBlackboardValueAsEnum( const UBlackboardComponent* pBB, FName KeyName )
{
	return pBB ? pBB->GetValueAsEnum( KeyName ) : 0;
}

int32 UCommonLib::GetBlackboardValueAsInt( const UBlackboardComponent* pBB, FName KeyName )
{
	return pBB ? pBB->GetValueAsInt( KeyName ) : 0;
}

float UCommonLib::GetBlackboardValueAsFloat( const UBlackboardComponent* pBB, FName KeyName )
{
	return pBB ? pBB->GetValueAsFloat( KeyName ) : 0.f;
}

bool UCommonLib::GetBlackboardValueAsBool( const UBlackboardComponent* pBB, FName KeyName )
{
	return pBB ? pBB->GetValueAsBool( KeyName ) : false;
}

FString UCommonLib::GetBlackboardValueAsString( const UBlackboardComponent* pBB, FName KeyName )
{
	return pBB ? pBB->GetValueAsString( KeyName ) : FString();
}

FName UCommonLib::GetBlackboardValueAsName( const UBlackboardComponent* pBB, FName KeyName )
{
	return pBB ? pBB->GetValueAsName( KeyName ) : NAME_None;
}

FVector UCommonLib::GetBlackboardValueAsVector( const UBlackboardComponent* pBB, FName KeyName )
{
	return pBB ? pBB->GetValueAsVector( KeyName ) : FVector::ZeroVector;
}

FRotator UCommonLib::GetBlackboardValueAsRotator( const UBlackboardComponent* pBB, FName KeyName )
{
	return pBB ? pBB->GetValueAsRotator( KeyName ) : FRotator::ZeroRotator;
}

void UCommonLib::SetBlackboardValueAsObject( UBlackboardComponent* pBB, FName KeyName, UObject* Value )
{
	if( pBB ) pBB->SetValueAsObject( KeyName, Value );
}

void UCommonLib::SetBlackboardValueAsClass( UBlackboardComponent* pBB, FName KeyName, UClass* Value )
{
	if( pBB )  pBB->SetValueAsClass( KeyName, Value  );
}

void UCommonLib::SetBlackboardValueAsEnum( UBlackboardComponent* pBB, FName KeyName, uint8 Value )
{
	if( pBB )  pBB->SetValueAsEnum( KeyName, Value  );
}

void UCommonLib::SetBlackboardValueAsInt( UBlackboardComponent* pBB, FName KeyName, int32 Value )
{
	if( pBB )  pBB->SetValueAsInt( KeyName, Value  );
}

void UCommonLib::SetBlackboardValueAsFloat( UBlackboardComponent* pBB, FName KeyName, float Value )
{
	if( pBB )  pBB->SetValueAsFloat( KeyName, Value  );
}

void UCommonLib::SetBlackboardValueAsBool( UBlackboardComponent* pBB, FName KeyName, bool Value )
{
	if( pBB )  pBB->SetValueAsBool( KeyName, Value  );
}

void UCommonLib::SetBlackboardValueAsString( UBlackboardComponent* pBB, FName KeyName, FString Value )
{
	if( pBB )  pBB->SetValueAsString( KeyName, Value  );
}

void UCommonLib::SetBlackboardValueAsName( UBlackboardComponent* pBB, FName KeyName, FName Value )
{
	if( pBB )  pBB->SetValueAsName( KeyName, Value  );
}

void UCommonLib::SetBlackboardValueAsVector( UBlackboardComponent* pBB, FName KeyName, FVector Value )
{
	if( pBB )  pBB->SetValueAsVector( KeyName, Value  );
}

void UCommonLib::SetBlackboardValueAsRotator( UBlackboardComponent* pBB, FName KeyName, FRotator Value )
{
	if( pBB )  pBB->SetValueAsRotator( KeyName, Value  );
}
