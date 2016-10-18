#pragma once
#include "Delegate.h"
#include "CommonCommand.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FExecEventBP, FString, sCMD );


UCLASS( Blueprintable )
class _COMMON_API UCommonCommand : public UObject
{
	GENERATED_BODY()

	void __RegisterConsoleVariable( const TCHAR* psName, const TCHAR* psHelp );

public :
	DECLARE_EVENT_OneParam(UCommonCommand, FCommnadEvent, const TCHAR* );

protected :
	TMap<FString, FCommnadEvent>   m_DelegateMap;

	template<class T>
	void AddExec( const TCHAR* psName, typename TMemFunPtrType<false, T, void(const TCHAR*)>::Type MethodPtr, const TCHAR* psHelp =nullptr )
	{
		m_DelegateMap.Add( psName ); 
		m_DelegateMap[psName].AddUObject( (T*)this, MethodPtr );
		__RegisterConsoleVariable( psName, psHelp );
	}


public :
	UFUNCTION(BlueprintCallable, Category="Util|Command")
	virtual void Init(){  InitBP(); }

	bool Exec( UWorld* pInWorld, const TCHAR* psCmd, FOutputDevice& Ar );

	// ----------- 	Blueprint	--------------------------------
protected :
	UPROPERTY( BlueprintAssignable, Category="Util|Command" )
	FExecEventBP m_ExecEventBP;

	UFUNCTION(BlueprintImplementableEvent, Category="Util|Command", meta=(DisplayName = "Init"))
	void InitBP();

	UFUNCTION(BlueprintCallable, Category="Util|Command", meta=(DisplayName="RegisterConsoleVariable"))
	void RegisterConsoleVariableBP( FString sName, FString sHelp )
	{
		__RegisterConsoleVariable( *sName, *sHelp );
	}

};
