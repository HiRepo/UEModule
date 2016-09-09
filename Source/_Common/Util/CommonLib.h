#pragma once
#include "GameFramework/WorldSettings.h"
#include "Engine.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "CommonLib.generated.h"

class UBlackboardComponent;
class ACommonCharacter;

class UJobActorComposite;
class UJobMaterial;

UCLASS()
class _COMMON_API UCommonLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
//	UFUNCTION(BlueprintPure, Category="Util|Common", meta=(HidePin="pWorldContext", DefaultToSelf="pWorldContext", DisplayName="NewObject", CompactNodeTitle = "NewObject", Keywords="NewObject"))
	UFUNCTION(BlueprintPure, Category="Util|Common", meta=(WorldContext="pWorldContext", DisplayName="NewObject", CompactNodeTitle = "NewObject", Keywords="NewObject"))
	static UObject* NewObject( UObject* pWorldContext, UClass* pClass )
	{
		UWorld* pWorld = TL::GetWorld( pWorldContext );
		UObject* pOuter = (UObject*)GetTransientPackage();
		UObject* pNewObject = ::NewObject<UObject>( pOuter, pClass );
		if( pNewObject )
			pNewObject->AddToRoot();
		return pNewObject;
	}

	UFUNCTION(BlueprintPure, Category="Util|Common" )
	static ACommonCharacter* GetCommonCharacter( UObject* pObject );

	UFUNCTION(BlueprintPure, Category="Util|Common", meta=(DefaultToSelf="pObject"))
	static UWorld* __GetWorld( UObject* pObject )
	{
		UStruct* pClass = pObject->GetClass();
		if( pClass->IsChildOf( AActor::StaticClass() ) )
			return TL::GetWorld( (AActor*)pObject );
		return TL::GetWorld( pObject );
	}
	

//----------		Controller		---------------------------------------------------
	UFUNCTION(BlueprintPure, Category="Util|Common", Meta=(DefaultToSelf="pPawn") )
	static bool IsPlayer( const APawn* pPawn )
	{
		check( pPawn );
		return Cast<APlayerController>( pPawn->GetController() ) ? true : false ; 
	}

	UFUNCTION(BlueprintPure, Category="Util|Common", meta=(WorldContext="pWorldContext"))
	static APlayerController* GetPlayerController( UObject* pWorldContext, int index = 0 )
	{
		return TL::Controller<APlayerController>::Get( TL::GetWorld( pWorldContext ), index );
	}


//----------		String		---------------------------------------------------
	UFUNCTION(BlueprintPure, Category="Util|String", Meta=(DefaultToSelf="pObject") )
	static FString GetClassName( const UObject* pObject )
	{
		return pObject ? pObject->GetName() : TEXT("None");
	}

	UFUNCTION(BlueprintPure, Category="Util|String", Meta=(HidePin="pObject", DefaultToSelf="pObject", DisplayName="GetFName", CompactNodeTitle = "FName"))
	static FName GetFName( const UObject* pObject )
	{
		return pObject ? pObject->GetClass()->GetFName() : NAME_None;
	}


// --------------- Debug -----------------------------------------------------
//	UFUNCTION(BlueprintCallable, Category="Analytics")

	static void PrintError()
	{
 		UE_LOG(LogClass, Warning, TEXT("StartSession: Failed to get the default analytics provider. Double check your [Analytics] configuration in your INI"));
	}

	UFUNCTION(BlueprintCallable, Category="Util|Common")
	static void PrintWarning()
	{
 		UE_LOG(LogClass, Warning, TEXT("StartSession: Failed to get the default analytics provider. Double check your [Analytics] configuration in your INI"));
	}



//----------		Compare Operator		---------------------------------------------------

//		/* Returns true if A is less than or equal to B (A <= B) */
//		UFUNCTION(BlueprintPure, meta=(DisplayName = "Enum <= Enum", CompactNodeTitle = "<=", Keywords = "<= less"), Category="Util|Math")
//		static bool LessEqual_Enum( UEnum* A, UEnum* B )
//		{
//			A->GetInwk
//			A->GetValueByIndex(	)
//		}

//		/* Returns true if A is greater than or equal to B (A >= B) */
//		UFUNCTION(BlueprintPure, meta=(DisplayName = "Byte >= Byte", CompactNodeTitle = ">=", Keywords = ">= greater"), Category="Math|Byte")
//		static bool GreaterEqual_Enum(uint8 A, uint8 B);


//----------		Material		---------------------------------------------------
	UFUNCTION(BlueprintCallable, Category="Util|Common" )
	static UJobActorComposite* GetJobActorComposite( UWorld* pWorld, FString sActorName );

	UFUNCTION(BlueprintCallable, Category="Util|Common" )
	static UJobMaterial* GetJobMaterial( UJobActorComposite* pJobActorComposite );


//----------		Time Dilation		---------------------------------------------------
protected :
	UFUNCTION(BlueprintCallable, Category="Util|Common" )
	static void __SetTimeDilation( UObject* pObject, const float fVal )
	{
		UStruct* pClass = pObject->GetClass();
		if( pClass->IsChildOf( UWorld::StaticClass() ) )
			SetTimeDilation( (UWorld*)pObject, fVal );
		else
		if( pClass->IsChildOf( AActor::StaticClass() ) )
			SetTimeDilation( (AActor*)pObject, fVal );
	}

	UFUNCTION(BlueprintPure, Category="Util|Common" )
	static float __GetTimeDilation( UObject* pObject, const float fVal )
	{
		UStruct* pClass = pObject->GetClass();
		if( pClass->IsChildOf( UWorld::StaticClass() ) )
			return GetTimeDilation( (UWorld*)pObject );
		else
		if( pClass->IsChildOf( AActor::StaticClass() ) )
			return GetTimeDilation( (AActor*)pObject );
		// KHI_ERROR
		return 1.f;
	}


public :
	FORCEINLINE
	static void SetTimeDilation( UWorld* pWorld, const float fVal )
	{
		check( pWorld );
		pWorld->GetWorldSettings()->TimeDilation = fVal;
	}

	FORCEINLINE
	static void SetTimeDilation( AActor* pActor, const float fVal )
	{
		check( pActor );
		pActor->CustomTimeDilation = fVal;
	}

	FORCEINLINE
	static float GetTimeDilation( UWorld* pWorld )
	{
		check( pWorld );
		return pWorld->GetWorldSettings()->TimeDilation;
	}

	FORCEINLINE
	static float GetTimeDilation( AActor* pActor )
	{
		check( pActor );
		return	pActor->CustomTimeDilation;
	}


//----------		AI		---------------------------------------------------
	UFUNCTION(BlueprintPure, Category="Util|AI" )
	static UBlackboardComponent* __GetBlackboard( UObject* pObject );

	UFUNCTION(BlueprintPure, Category="Util|AI" )
	static UJobBehaviorTree* GetJobBehaviorTree( APawn* pPawn );

	UFUNCTION(BlueprintPure, Category="Util|AI|BB" )
	static AActor* GetBlackboardValueAsActor( const UBlackboardComponent* pBB, FName KeyName );

	UFUNCTION(BlueprintPure, Category="Util|AI|BB" )
	static UObject* GetBlackboardValueAsObject( const UBlackboardComponent* pBB,FName KeyName );

	UFUNCTION(BlueprintPure, Category="Util|AI|BB" )
	static UClass* GetBlackboardValueAsClass( const UBlackboardComponent* pBB,FName KeyName );

	UFUNCTION(BlueprintPure, Category="Util|AI|BB" )
	static uint8 GetBlackboardValueAsEnum( const UBlackboardComponent* pBB,FName KeyName );

	UFUNCTION(BlueprintPure, Category="Util|AI|BB" )
	static int32 GetBlackboardValueAsInt( const UBlackboardComponent* pBB,FName KeyName );

	UFUNCTION(BlueprintPure, Category="Util|AI|BB" )
	static float GetBlackboardValueAsFloat( const UBlackboardComponent* pBB,FName KeyName );

	UFUNCTION(BlueprintPure, Category="Util|AI|BB" )
	static bool GetBlackboardValueAsBool( const UBlackboardComponent* pBB,FName KeyName );

	UFUNCTION(BlueprintPure, Category="Util|AI|BB" )
	static FString GetBlackboardValueAsString( const UBlackboardComponent* pBB,FName KeyName );
	
	UFUNCTION(BlueprintPure, Category="Util|AI|BB" )
	static FName GetBlackboardValueAsName( const UBlackboardComponent* pBB, FName KeyName );

	UFUNCTION(BlueprintPure, Category="Util|AI|BB" )
	static FVector GetBlackboardValueAsVector( const UBlackboardComponent* pBB, FName KeyName );

	UFUNCTION(BlueprintPure, Category="Util|AI|BB" )
	static FRotator GetBlackboardValueAsRotator( const UBlackboardComponent* pBB, FName KeyName );

	UFUNCTION(BlueprintCallable, Category="Util|AI|BB" )
	static void SetBlackboardValueAsObject( UBlackboardComponent* pBB, FName KeyName, UObject* Value );

	UFUNCTION(BlueprintCallable, Category="Util|AI|BB" )
	static void SetBlackboardValueAsClass( UBlackboardComponent* pBB, FName KeyName, UClass* Value );

	UFUNCTION(BlueprintCallable, Category="Util|AI|BB" )
	static void SetBlackboardValueAsEnum( UBlackboardComponent* pBB, FName KeyName, uint8 Value );

	UFUNCTION(BlueprintCallable, Category="Util|AI|BB" )
	static void SetBlackboardValueAsInt( UBlackboardComponent* pBB, FName KeyName, int32 Value );

	UFUNCTION(BlueprintCallable, Category="Util|AI|BB" )
	static void SetBlackboardValueAsFloat( UBlackboardComponent* pBB, FName KeyName, float Value );

	UFUNCTION(BlueprintCallable, Category="Util|AI|BB" )
	static void SetBlackboardValueAsBool( UBlackboardComponent* pBB, FName KeyName, bool Value );

	UFUNCTION(BlueprintCallable, Category="Util|AI|BB" )
	static void SetBlackboardValueAsString( UBlackboardComponent* pBB, FName KeyName, FString Value );

	UFUNCTION(BlueprintCallable, Category="Util|AI|BB" )
	static void SetBlackboardValueAsName( UBlackboardComponent* pBB, FName KeyName, FName Value );

	UFUNCTION(BlueprintCallable, Category="Util|AI|BB" )
	static void SetBlackboardValueAsVector( UBlackboardComponent* pBB, FName KeyName, FVector Value );

	UFUNCTION(BlueprintCallable, Category="Util|AI|BB" )
	static void SetBlackboardValueAsRotator( UBlackboardComponent* pBB, FName KeyName, FRotator Value );

};
