﻿#pragma once
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


#define __COMM_INIT_SLOT( _CLASS, _PREFIX, _COMM, _PARAM, _HELP ) 		\
	AddExec<_CLASS>( TEXT( #_PREFIX"."#_COMM ), &_CLASS::_CB_##_COMM, TEXT( _HELP ) );

#define __COMM_FUNC_SLOT( _CLASS, _PREFIX, _COMM, _PARAM, _HELP ) 		\
	__COMM_FUNC##_PARAM( _COMM )


#define __COMM_PARCING_S( _V ) 											\
	FString _V = FParse::Token( param, false );

#define __COMM_PARCING_I( _V ) 											\
	int _V = FCString::Atoi( *FParse::Token( param, false ) );

#define __COMM_PARCING_F( _V ) 											\
	float  _V = FCString::Atof( *FParse::Token( param, false ) );

#define __COMM_FUNC( _COMM )											\
	void _COMM();														\
	void _CB_##_COMM( const TCHAR* ){ _COMM(); }

#define __COMM_FUNC_F( _COMM )											\
	void _COMM( float );												\
	void _CB_##_COMM( const TCHAR* param ){  							\
		__COMM_PARCING_F( v );	_COMM( v ); }

#define __COMM_FUNC_S( _COMM )											\
	void _COMM( FString );												\
	void _CB_##_COMM( const TCHAR* param ){  							\
		__COMM_PARCING_S( v );	_COMM( v ); }

#define __COMM_FUNC_I( _COMM )											\
	void _COMM( int );													\
	void _CB_##_COMM( const TCHAR* param ){  							\
		__COMM_PARCING_I( v );	_COMM( v ); }

#define __COMM_FUNC_I_S( _COMM )										\
	void _COMM( int, FString );											\
	void _CB_##_COMM( const TCHAR* param ){ 							\
		__COMM_PARCING_I( i );											\
		__COMM_PARCING_S( s );											\
		_COMM( i ,s	);	}

#define __COMM_FUNC_F_S( _COMM )										\
	void _COMM( float, FString );										\
	void _CB_##_COMM( const TCHAR* param ){ 							\
		__COMM_PARCING_F( f );											\
		__COMM_PARCING_S( s );											\
		_COMM( f ,s	);	}

#define __COMM_FUNC_S_I( _COMM )										\
	void _COMM( int, FString );											\
	void _CB_##_COMM( const TCHAR* param ){ 							\
		__COMM_PARCING_S( s );											\
		__COMM_PARCING_I( i );											\
		_COMM( s, i	);	}

#define __COMM_FUNC_S_F( _COMM )										\
	void _COMM( float, FString );										\
	void _CB_##_COMM( const TCHAR* param ){ 							\
		__COMM_PARCING_S( s );											\
		__COMM_PARCING_F( f );											\
		_COMM( s, f	);	}

#define __COMM_FUNC_I_I( _COMM )										\
	void _COMM( int, FString );											\
	void _CB_##_COMM( const TCHAR* param ){ 							\
		__COMM_PARCING_I( i1 );											\
		__COMM_PARCING_I( i2 );											\
		_COMM( i1 , i2	);	}

#define __COMM_FUNC_F_F( _COMM )										\
	void _COMM( int, FString );											\
	void _CB_##_COMM( const TCHAR* param ){ 							\
		__COMM_PARCING_F( f1 );											\
		__COMM_PARCING_F( f2 );											\
		_COMM( f1 , f2	);	}	

#define __COMM_FUNC_S_S( _COMM )										\
	void _COMM( FString, FString );										\
	void _CB_##_COMM( const TCHAR* param ){ 							\
		__COMM_PARCING_S( s1 );											\
		__COMM_PARCING_S( s2 );											\
		_COMM( s1 ,s2 );	}	

