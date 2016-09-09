#pragma once
#include "Tween.generated.h"

UENUM(BlueprintType)
enum class ETweenLeng : uint8
{
	Element1,
	Element2,
	Element3,
	Element4,
};

UENUM(BlueprintType)
enum class ETweenFlow : uint8
{
	Linear_Forward  	= 0x05,
	Linear_Backward		= 0x06,
	Sine_Forward		= 0x09,
	Sine_Backward		= 0x0A,
	Pow2_Forward		= 0x11,
	Pow2_Backward		= 0x12,
	Pow3_Forward		= 0x21,
	Pow3_Backward		= 0x22,
	RPow2_Forward		= 0x41,
	RPow2_Backward		= 0x42,
	RPow3_Forward		= 0x81,
	RPow3_Backward		= 0x82,
};


USTRUCT(BlueprintType)
struct FTween
{
	GENERATED_BODY()

	enum EFlag 
	{
		Forward		= 0x01,
		Backward	= 0x02,
		FlagWard	= 0x03,

		Linear		= 0x04,
		Sine		= 0x08,

		Pow2		= 0x10,
		Pow3		= 0x20,

		RPow2		= 0x40,
		RPow3		= 0x80,
	};

protected :
	UPROPERTY( EditAnywhere, meta=(DisplayName="Type") )
	ETweenFlow 	m_Type 			= ETweenFlow::Linear_Forward;

	UPROPERTY( EditAnywhere, meta=(DisplayName="ElementLeng"))
	ETweenLeng 	m_eElementLeng	= ETweenLeng::Element4;

	UPROPERTY( EditAnywhere, meta=(DisplayName="Source") )
	FLinearColor m_Source		= FLinearColor::Black;

	UPROPERTY( EditAnywhere, meta=(DisplayName="Target") )
	FLinearColor m_Target		= FLinearColor::White;

public :
	FORCEINLINE	
	ETweenFlow GetType() { return m_Type; }
	FORCEINLINE	
	FLinearColor GetSource() { return m_Source; }
	FORCEINLINE	
	FLinearColor GetTarget() { return m_Target; }
	FORCEINLINE	
	ETweenLeng GetElementLeng() { return m_eElementLeng; }


	void SetElementLeng( ETweenLeng eVal ) { m_eElementLeng = eVal; }

	FORCEINLINE	
	void SetTargetX( float val ) { m_Target.R = val; }
	FORCEINLINE	
	void SetTargetY( float val ) { m_Target.G = val; }
	FORCEINLINE	
	void SetTargetZ( float val ) { m_Target.B = val; }
	FORCEINLINE	
	void SetTargetA( float val ) { m_Target.A = val; }

	FORCEINLINE	
	void SetSoureR( float val ) { m_Source.R = val; }
	FORCEINLINE	
	void SetSoureG( float val ) { m_Source.G = val; }
	FORCEINLINE	
	void SetSoureB( float val ) { m_Source.B = val; }
	FORCEINLINE	
	void SetSoureA( float val ) { m_Source.A = val; }

	FORCEINLINE	
	void SetTarget( float val ) { m_Target.R = val; }
	FORCEINLINE	
	void SetSoure( float val ) 	{ m_Source.R = val; }
	FORCEINLINE	
	void SetTarget( FLinearColor val ) { m_Target = val; }
	FORCEINLINE	
	void SetSource( FLinearColor val ) { m_Source = val; }

public :
	FORCEINLINE	
	bool IsForward()	{ return !!(Forward & (int)m_Type); }

	FORCEINLINE	
	bool IsBackward()	{ return !!(Backward & (int)m_Type); }

	FORCEINLINE	
	bool IsLinear()		{ return !!( Linear & (int)m_Type); }

	FORCEINLINE	
	bool IsSine()		{ return !!( Sine & (int)m_Type); }

	FORCEINLINE	
	bool IsRPow2()		{ return !!( RPow2 & (int)m_Type); }

	FORCEINLINE	
	bool IsRPow3()		{ return !!( RPow3 & (int)m_Type); }

	FORCEINLINE	
	bool IsPower2()		{ return !!( Pow2 & (int)m_Type); }

	FORCEINLINE	
	bool IsPower3()		{ return !!( Pow3 & (int)m_Type); }

	FORCEINLINE
	void SetFlag( EFlag eFlag )	{ m_Type = ETweenFlow( ((int)m_Type & FlagWard) | eFlag ); }

	FORCEINLINE
	void SetBackward()	{ m_Type = ETweenFlow( ((int)m_Type & ~FlagWard) | Backward ); }

	FORCEINLINE
	void SetForward()	{ m_Type = ETweenFlow( ((int)m_Type & ~FlagWard) | Forward ); }

//		float start = IsForward() ? m_Source.##X : m_Target.##X;							\
//		float dir   = IsForward() ? m_Target.##X - m_Source.##X : m_Source.##X - m_Target.##X;	
#define __GET_VALUE_1( _X )															\
	float start = m_Source._X ;														\
	float dir   = m_Target._X - m_Source._X ;											\
	if( IsBackward() )																\
		fPercent = 1.f - fPercent;													\
	float fMagnitude = 1.f;															\
	if( IsLinear() )																\
		fMagnitude = fPercent;														\
	else if( IsSine() )																\
		fMagnitude = FMath::Sin( FMath::DegreesToRadians(fPercent * 90.f) );		\
	else if( IsPower2() )															\
		fMagnitude = FMath::Sqrt( fPercent );										\
	else if( IsPower3() )															\
		fMagnitude = FMath::Pow( fPercent, 0.3333f );								\
	else if( IsRPow2() )															\
		fMagnitude = fPercent * fPercent;											\
	else if( IsRPow3() )															\
		fMagnitude = fPercent * fPercent * fPercent;								\
	return start + dir * fMagnitude;

	float  GetValueX( float fPercent )
	{
		__GET_VALUE_1( R )
	}

	float  GetValueY( float fPercent )
	{
		__GET_VALUE_1( B )
	}

	float  GetValueZ( float fPercent )
	{
		__GET_VALUE_1( G )
	}

	float  GetValueA( float fPercent )
	{
		__GET_VALUE_1( A )
	}

	FLinearColor GetValue( float fPercent )
	{
//			FLinearColor& start= IsForward() ? m_Source : m_Target;
//			FLinearColor dir  = IsForward() ? m_Target-m_Source : m_Source-m_Target;
		if( IsBackward() )
			fPercent = 1.f - fPercent;

		float fMagnitude = 1.f;
		if( IsLinear() )
			fMagnitude = fPercent;
		else
		if( IsSine() )		// Util 90 degree,
			fMagnitude = FMath::Sin( FMath::DegreesToRadians(fPercent * 90.f) );
		else
		if( IsPower2() )
			fMagnitude = FMath::Sqrt( fPercent );
		else
		if( IsPower3() )
			fMagnitude = FMath::Pow( fPercent, 0.3333f );
		else
		if( IsRPow2() )						
			fMagnitude = fPercent * fPercent;
		else
		if( IsRPow3() )						
			fMagnitude = fPercent * fPercent * fPercent;

		return FLinearColor( m_Source + (m_Target-m_Source)*fMagnitude );
//			return FLinearColor( start + dir*fMagnitude );
	}
};

