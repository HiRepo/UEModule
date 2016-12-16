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
	Forward,
	Backward,
	Return,
};

UENUM(BlueprintType)
enum class ETweenCurve : uint8
{
	Linear,
	Sine,
	Pow2,
	Pow3,
	RPow2,
	RPow3,
};



USTRUCT(BlueprintType)
struct FTween
{
	GENERATED_BODY()

protected :
	UPROPERTY( EditAnywhere, meta=(DisplayName="Flow") )
	ETweenFlow 	m_eFlow 		= ETweenFlow::Forward;

	UPROPERTY( EditAnywhere, meta=(DisplayName="Curve") )
	ETweenCurve	m_eCurve		= ETweenCurve::Linear;

	UPROPERTY( EditAnywhere, meta=(DisplayName="ElementLeng"))
	ETweenLeng 	m_eElementLeng	= ETweenLeng::Element4;

	UPROPERTY( EditAnywhere, meta=(DisplayName="Source") )
	FLinearColor m_Source		= FLinearColor::Black;

	UPROPERTY( EditAnywhere, meta=(DisplayName="Target") )
	FLinearColor m_Target		= FLinearColor::White;

public :
	ETweenFlow	 GetFlowType() { return m_eFlow; }
	ETweenCurve  GetCurveType() { return m_eCurve; }
	ETweenLeng 	 GetLengType() { return m_eElementLeng; }
	FLinearColor GetSource() { return m_Source; }
	FLinearColor GetTarget() { return m_Target; }

	void SetLengType( ETweenLeng eVal ) { m_eElementLeng = eVal; }
	void SetFlowType( ETweenFlow eVal ) { m_eFlow = eVal; }
	void SetCurveType( ETweenCurve eVal ) { m_eCurve = eVal; }

	void SetTargetX( float val ) { m_Target.R = val; }
	void SetTargetY( float val ) { m_Target.G = val; }
	void SetTargetZ( float val ) { m_Target.B = val; }
	void SetTargetA( float val ) { m_Target.A = val; }

	void SetSoureR( float val ) { m_Source.R = val; }
	void SetSoureG( float val ) { m_Source.G = val; }
	void SetSoureB( float val ) { m_Source.B = val; }
	void SetSoureA( float val ) { m_Source.A = val; }

	void SetTarget( float val ) { m_Target.R = val; }
	void SetSoure( float val ) 	{ m_Source.R = val; }
	void SetTarget( FLinearColor val ) { m_Target = val; }
	void SetSource( FLinearColor val ) { m_Source = val; }

private :
	float __GetMagnitude( float fPercent )
	{
		float fMagnitude = 1.f;
		switch( m_eCurve )
		{
		case ETweenCurve::Linear :
			fMagnitude = fPercent;
			break;
		case ETweenCurve::Sine :
			fMagnitude = FMath::Sin( FMath::DegreesToRadians(fPercent * 90.f) );
			break;
		case ETweenCurve::Pow2 :
			fMagnitude = FMath::Sqrt( fPercent );
			break;
		case ETweenCurve::Pow3 :
			fMagnitude = FMath::Pow( fPercent, 0.3333f );
			break;
		case ETweenCurve::RPow2 :
			fMagnitude = fPercent * fPercent;
			break;
		case ETweenCurve::RPow3 :
			fMagnitude = fPercent * fPercent * fPercent;
			break;
		}
		return fMagnitude;
	}

	template<class T>
	T __GetValue( float fPercent, T& source, T& target )
	{
		if( ETweenFlow::Backward == m_eFlow )
			fPercent = 1.f - fPercent;
		else
		if( ETweenFlow::Return == m_eFlow )
		{
			if( 1.f < fPercent )
				return source;
			fPercent *= 2.f;
			if( fPercent > 1.f )
				fPercent = 2.f - fPercent;
		}
		float fMagnitude = __GetMagnitude( fPercent );
		return source + (target - source) * fMagnitude;
	}
public :
	float  GetValueX( float fPercent )
	{
		return __GetValue( fPercent, m_Source.R, m_Target.R );
	}

	float  GetValueY( float fPercent )
	{
		return __GetValue( fPercent, m_Source.G, m_Target.G );
	}

	float  GetValueZ( float fPercent )
	{
		return __GetValue( fPercent, m_Source.B, m_Target.B );
	}

	float  GetValueA( float fPercent )
	{
		return __GetValue( fPercent, m_Source.A, m_Target.A );
	}

	FLinearColor GetValue( float fPercent )
	{
		return __GetValue( fPercent, m_Source, m_Target );
	}

private :
	bool m_bSkipFrame = false;

	float m_fOldPercent = 0.f;

	FLinearColor m_Current = FLinearColor::Black;

public :
	FLinearColor GetCurrent() { return m_Current; }
	float GetCurrentR() { return m_Current.R; }
	float GetCurrentG() { return m_Current.G; }
	float GetCurrentB() { return m_Current.B; }
	float GetCurrentA() { return m_Current.A; }

	bool IsOverTime(){ return 1.f <= m_fOldPercent; }

	// return UpdateTime
	bool Update( const float fPercent, bool isLoop )
	{
		bool bUpdateTime = true;
		float fFixedPercent = fPercent;
		const float _fOldPercent = m_fOldPercent;
		
		if( ETweenFlow::Return == m_eFlow )
		{
			if( _fOldPercent < 0.5f && 0.5f <= fPercent )
			{
				bUpdateTime = false;
				fFixedPercent = 0.5f;
			}
		}
		if( _fOldPercent < 1.f && 1.f < fPercent )
		{
			fFixedPercent = 1.f;
			if( isLoop )
				bUpdateTime = false;
		}

		switch( m_eElementLeng )
		{
		case ETweenLeng::Element1 :
			m_Current.R = GetValueX( fFixedPercent );
		case ETweenLeng::Element4 :
			m_Current = GetValue( fFixedPercent );
		}
		m_fOldPercent = fPercent;

		return bUpdateTime;
	}
};

