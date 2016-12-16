#pragma once
#include "_CommonType.h"
#include "JobComponent.h"
#include "JobTimeDilation.generated.h"

USTRUCT(BlueprintType, meta=(DisplayName ="Time Scale"))
struct FTimeDilation
{
	friend class UJobTimeDilation;

	GENERATED_BODY()
protected :
	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta=(UIMin = 0, ClampMin = 0), meta=(DisplayName="Only Animation") )
	bool 	m_isOnlyAnim		= false;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta=(UIMin = 0, ClampMin = 0), meta=(DisplayName="Time Scale") )
	float 	m_fTimeScale		= 1.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta=(UIMin = 0, ClampMin = 0), meta=(DisplayName="Available Time") )
	float 	m_fAvailableTime	= 0.f;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName="Life Cycle") )
	ELifeCycle m_eLifeCycle 	= ELifeCycle::Infinity;

public :
	ELifeCycle GetLifeCycle(){ return m_eLifeCycle; }

	void SetOnlyAnim( bool isOnlyAnim ){ m_isOnlyAnim = isOnlyAnim; }

	bool IsOnlyAnim( bool isOnlyAnim ){ return m_isOnlyAnim; } 

	float GetAvailableTime(){ return m_fAvailableTime; }

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
		m_fTimeScale = (fTimeScale > 0.f) ? fTimeScale : 0.f;
	}

protected :
	bool _UpdateAvailableTime( float fDeltaTime )
	{
		if( ELifeCycle::Infinity != m_eLifeCycle  )
		{
			if( m_fAvailableTime < 0.f )
				return false;
			m_fAvailableTime -= fDeltaTime;
		}
		return true;
	}

};

UCLASS(meta=(DisplayName ="Time Scale"))
class _COMMON_API UJobTimeDilation : public UJobComponent
{
	GENERATED_BODY()
friend struct TL::Create<UJobTimeDilation, false>;
friend class UTimeDilationNotifyState;

private :
//		UPROPERTY( EditAnywhere, Category="Job|Time", meta=(DisplayName = "Default Time Scale List") )
	UPROPERTY( EditAnywhere, Category="JobComponent",  meta=(DisplayName = "Default Time List") )
	TArray<FTimeDilation> 	m_TimeDilationDatas;
	TArray<FTimeDilation*> 	m_TimeDilationExterns;	// ex) Referfece NofifyState::FTimeDilation

	float m_fTimeDilation = 1.f;

	float m_fTimeScaleAnim = 1.f;

protected :
	void _UpdateCurTimeDilation();

	void _Update( FTimeDilation& rTD )
	{
		if( rTD._UpdateAvailableTime( GetWorld()->GetDeltaSeconds() ) )
		{
			if( rTD.m_isOnlyAnim )
				m_fTimeScaleAnim *=  rTD.m_fTimeScale;
			else
				m_fTimeDilation *= rTD.m_fTimeScale;
		}
	}

	void _ResetTimeScale()
	{
		m_fTimeDilation = 1.f;
		m_fTimeScaleAnim = 1.f;
	}

	void _init()
	{
	}

public :
	UJobTimeDilation()
	{ m_EnableTick = true; }

	UFUNCTION(BlueprintPure, Category="Job|TimeDilation")
	float GetCurTimeDilation() { return m_fTimeDilation; }

 	FTimeDilation& operator[]( int idx )
 	{ 
		check( m_TimeDilationDatas.IsValidIndex( idx ) );
 		return m_TimeDilationDatas[ idx ]; 
 	}

	UFUNCTION(BlueprintCallable, Category="Job|TimeScale" )
	void SetOnlyAnim( int index, bool bOnlyAnim )
	{
		m_TimeDilationDatas[index].SetOnlyAnim( bOnlyAnim );
	}

	UFUNCTION(BlueprintCallable, Category="Job|TimeScale" )
	void SetTimeScale( int index, float fTimeScale )
	{
		m_TimeDilationDatas[index].SetTimeScale( fTimeScale );
	}

	UFUNCTION(BlueprintCallable, Category="Job|TimeScale" )
	void SetAvailableTime( int index, float fAvailableTime )
	{
		m_TimeDilationDatas[index].m_fAvailableTime = fAvailableTime;
	}

	UFUNCTION(BlueprintCallable, Category="Job|TimeScale" )
	void SetLifeCycle( int index, ELifeCycle eLifeCycle )
	{
		m_TimeDilationDatas[index].m_eLifeCycle = eLifeCycle;
	}

	void SetTimeDilation( int index, FTimeDilation timeDilation )
	{
		m_TimeDilationDatas[index] = timeDilation;
	}

	UFUNCTION(BlueprintCallable, Category="Job|TimeScale" )
	void Add( FTimeDilation timeDilation )
	{
		m_TimeDilationDatas.Push( timeDilation );
	}

	void Append( FTimeDilation timeDilation = FTimeDilation{} )
	{
		m_TimeDilationDatas.Push( timeDilation );
	}

	// Extern : FTimeDilation Data is managed by the outside.
	void AppendExtern( FTimeDilation* pTimeDilation )
	{
		check(pTimeDilation);
		m_TimeDilationExterns.AddUnique( pTimeDilation );
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
		_ResetTimeScale();
		_UpdateCurTimeDilation();
	}

	virtual void Tick( float fDeltaTime ) override
	{
		Super::Tick( fDeltaTime );

		_ResetTimeScale();

		for( int i= 0; i < m_TimeDilationDatas.Num(); ++i )
		{
			FTimeDilation& rTD = m_TimeDilationDatas[i];
			_Update( rTD );
			if( ELifeCycle::TimeAutoDelete == rTD.m_eLifeCycle && rTD.m_fAvailableTime < 0.f )
				m_TimeDilationDatas.RemoveAt( i );
		}

		for( FTimeDilation* pTD : m_TimeDilationExterns )
		{
			FTimeDilation& rTD = *pTD;
			_Update( rTD );
			if( ELifeCycle::TimeAutoDelete == rTD.m_eLifeCycle && rTD.m_fAvailableTime < 0.f )
				m_TimeDilationExterns.Remove( pTD );
		}

		_UpdateCurTimeDilation();
	}
};
