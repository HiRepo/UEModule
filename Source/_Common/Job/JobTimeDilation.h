#pragma once
#include "_CommonType.h"
#include "JobComponent.h"
#include "JobTimeDilation.generated.h"

USTRUCT(meta=(DisplayName ="Time Speed"))
struct FTimeDilation
{
	friend class UJobTimeDilation;

	GENERATED_BODY()
protected :
	UPROPERTY( EditAnywhere, meta=(UIMin = 0, ClampMin = 0), meta=(DisplayName="Time Scale") )
	float 	m_fTimeScale		= 1.f;

	UPROPERTY( EditAnywhere, meta=(UIMin = 0, ClampMin = 0), meta=(DisplayName="Available Time") )
	float 	m_fAvailableTime	= 0.f;

	UPROPERTY( VisibleAnywhere, meta=(DisplayName="Life Cycle") )
	ELifeCycle m_eLifeCycle 	= ELifeCycle::Time;

public :
	FORCEINLINE
	ELifeCycle GetLifeCycle(){ return m_eLifeCycle; }
	FORCEINLINE
	float GetAvailableTime(){ return m_fAvailableTime; }
	FORCEINLINE
	float GetTimeScale(){ return m_fTimeScale; }

	void SetLifeCycle( ELifeCycle eLifeCycle )
	{
		m_eLifeCycle = eLifeCycle;
	}

	void SetAvailableTime( float fAvailableTime )
	{
		m_fAvailableTime = fAvailableTime;
	}
	
	void SetTimeScale( float fTimeScale )
	{
		m_fTimeScale = fTimeScale;
	}
};


//	UCLASS(Category="Job", meta=(DisplayName ="_Job Time Speed"))
UCLASS(meta=(DisplayName ="Time Speed"))
class _COMMON_API UJobTimeDilation : public UJobComponent
{
	GENERATED_BODY()
friend struct TL::Create<UJobTimeDilation, false>;
friend class UTimeDilationNotifyState;

private :
//		UPROPERTY( EditAnywhere, Category="Job|Time", meta=(DisplayName = "Default Time Speed List") )
	UPROPERTY( EditAnywhere, Category="JobComponent",  meta=(DisplayName = "Default Time List") )
	TArray<FTimeDilation> 	m_TimeDilationDatas;
	TArray<FTimeDilation*> 	m_TimeDilationExterns;	// ex) Referfece NofifyState::FTimeDilation

	float m_fTimeDilation = 1.f;

protected :
	void _UpdateCurTimeDilation();

	void _Calculate( FTimeDilation& rTD )
	{
		if( ELifeCycle::Infinity != rTD.m_eLifeCycle && rTD.m_fAvailableTime <= 0  )
			return;

		m_fTimeDilation *= rTD.m_fTimeScale;
		if( ELifeCycle::Infinity != rTD.m_eLifeCycle  )
			rTD.m_fAvailableTime -= GetWorld()->GetDeltaSeconds();
	}


	void _init()
	{
	}

public :
	UJobTimeDilation()
	{ m_EnableTick = true; }

	UFUNCTION(BlueprintPure, Category="Job|TimeDilation")
	float GetCurTimeDilation() { return m_fTimeDilation; }

	FORCEINLINE
 	FTimeDilation& operator[]( int idx )
 	{ 
		check( m_TimeDilationDatas.IsValidIndex( idx ) );
 		return m_TimeDilationDatas[ idx ]; 
 	}

	FORCEINLINE
	void ResetTimeDilation()
	{
		m_fTimeDilation = 1.f;
		_UpdateCurTimeDilation();		// Recurvery orignal
	}


	void SetTimeScale( int index, float fTimeScale )
	{
		m_TimeDilationDatas[index].m_fTimeScale = fTimeScale;
	}

	void SetAvailableTime( int index, float fAvailableTime )
	{
		m_TimeDilationDatas[index].m_fAvailableTime = fAvailableTime;
	}

	void SetLifeCycle( int index, ELifeCycle eLifeCycle )
	{
		m_TimeDilationDatas[index].m_eLifeCycle = eLifeCycle;
	}

	void SetTimeDilation( int index,  FTimeDilation timeDilation )
	{
		m_TimeDilationDatas[index] = timeDilation;
	}

	void Append( FTimeDilation timeDilation = FTimeDilation{} )
	{
		m_TimeDilationDatas.Push( timeDilation );
	}

	// Extern : FTimeDilation Data is managed by the outside.
	void AppendExtern( FTimeDilation* pTimeDilation )
	{
		check(pTimeDilation);
		m_TimeDilationExterns.Push( pTimeDilation );
	}

	void RemoveExtern( FTimeDilation* pTimeDilation )
	{
		m_TimeDilationExterns.Remove( pTimeDilation );
	}

	bool IsContainExtern( FTimeDilation* pTimeDilation )
	{
		return m_TimeDilationExterns.Contains( pTimeDilation );
	}

protected :
	virtual void OnDetach() override
	{
		Super::OnDetach();
		ResetTimeDilation();
	}

	virtual void Tick( float fDeltaTime ) override
	{
		Super::Tick( fDeltaTime );
		m_fTimeDilation = 1.f;

		for( int i= 0; i < m_TimeDilationDatas.Num(); ++i )
		{
			FTimeDilation& rTD = m_TimeDilationDatas[i];
			_Calculate( rTD );
			if( ELifeCycle::TimeAutoDelete == rTD.m_eLifeCycle && rTD.m_fAvailableTime < 0.f )
				m_TimeDilationDatas.RemoveAt( i );
		}

		for( FTimeDilation* pTD : m_TimeDilationExterns )
		{
			FTimeDilation& rTD = *pTD;
			_Calculate( rTD );
			if( ELifeCycle::TimeAutoDelete == rTD.m_eLifeCycle && rTD.m_fAvailableTime < 0.f )
				m_TimeDilationExterns.Remove( pTD );
		}

		_UpdateCurTimeDilation();
	}
};
