#pragma once
#include "_CommonType.h"
#include "JobComponent.h"
#include "Util/Tween.h"
#include "JobAniTransform.generated.h"


UENUM(BlueprintType)
enum class ETransformType : uint8
{
	Scale		= 0x0001,
	Location	= 0x0002,
	Rotation	= 0x0004,
};


USTRUCT(meta=(DisplayName ="Ani Transform"))
struct FAniTData
{
	GENERATED_BODY()
	friend struct FAniTransform;

protected :
	UPROPERTY( EditAnywhere )
	ETransformType	m_eType 	= ETransformType::Scale;

	UPROPERTY( EditAnywhere )
	FTween 	m_Tween;

public :
	FAniTData()
	{
		SetType( m_eType, true );
	}

	FTween& GetTween(){ return m_Tween; }

	ETransformType GetType() { return m_eType; }

	void SetType( ETransformType eType, bool bInitialize = true )
	{
		m_eType = eType;
		if( bInitialize )
		{
			if( ETransformType::Scale == eType )
				m_Tween.SetSource( FLinearColor{ 1.f, 1.f, 1.f, 1.f } );
			else	
				m_Tween.SetSource( FLinearColor{ FVector::ZeroVector } );
		}
	}
};


USTRUCT(meta=(DisplayName ="Ani Transform"))
struct FAniTransform
{
	friend class UJobAniTransform;

	GENERATED_BODY()

protected :
	UPROPERTY( EditAnywhere, meta=(DisplayName="Play") )
	bool  m_bPlay 			= false;

	UPROPERTY( EditAnywhere, meta=(DisplayName="Loop") )
	bool  m_bLoop 			= false;

	UPROPERTY( VisibleAnywhere )
	ELifeCycle m_eLifeCycle	= ELifeCycle::Time;

	UPROPERTY( EditAnywhere, meta=(UIMin=0, ClampMin=0, DisplayName="Time" ) )
	float 	m_fTime			= 1.f;

	UPROPERTY( EditAnywhere, meta=(UIMin=0, ClampMin=0, DisplayName="Elapse Time" ) )
	float 	m_fElapseTime 	= 0.f;

	UPROPERTY( EditAnywhere, meta=(DisplayName="Tag") )
	FName	m_Tag			= NAME_None;

	UPROPERTY( EditAnywhere, meta=(DisplayName="AniData List") )
	TArray<FAniTData>  m_ArrAniData;

	UPROPERTY( VisibleAnywhere, Transient, meta=(DisplayName="Component List")  )
	TArray<USceneComponent*> m_ArrComponent;

public :
	FAniTransform()
	{
	}

 	FAniTData& operator[]( int idx ) 
 	{ 
		check( m_ArrAniData.IsValidIndex( idx ) );
 		return m_ArrAniData[idx]; 
 	}

	ELifeCycle GetLifeCycle(){ return m_eLifeCycle; }

	float GetTime(){ return m_fTime; }

	int GetNumOfData(){ return m_ArrAniData.Num(); }

	void SetLifeCycle( ELifeCycle eLifeCycle )
	{
		m_eLifeCycle = eLifeCycle;
	}

	void SetTime( float fTime )
	{
		check( 0.f <= fTime );
		m_fTime = fTime;
	}

	void AddComponent( USceneComponent* pComp )
	{
		m_ArrComponent.AddUnique( pComp );
	}

	void RemoveComponent( USceneComponent* pComp )
	{
		m_ArrComponent.Remove( pComp );
	}

	TArray<USceneComponent*>& GetComponents(){ return  m_ArrComponent; }

private :
	void __RebuildAll( USceneComponent* pRootComponent )
	{
		m_ArrComponent.Reset();
		TL::Component<USceneComponent>::TagAll( pRootComponent, m_ArrComponent, m_Tag );
	}


	bool m_bSkipUpdateTime = false;


	void Tick( float fDeltaTime )
	{
		if( false == m_bPlay )
			return;

		if( false == m_bSkipUpdateTime )
			m_fElapseTime += fDeltaTime;

		const float _fPercent = m_fElapseTime / m_fTime;

		bool bSkipUpdateTime = false;
		bool bStop = true;
		for( FAniTData& rATD : m_ArrAniData )
		{
			FTween& rTween = rATD.m_Tween;

			if( 1.f < _fPercent && rTween.IsOverTime() )	
				continue;
			bStop = false;

			if( false == rTween.Update( _fPercent, m_bLoop ) )
				bSkipUpdateTime = true;

			for( USceneComponent* pComp : m_ArrComponent )
			{
				switch( rATD.m_eType )
				{
					case ETransformType::Scale :
						pComp->SetRelativeScale3D ( FVector{ rTween.GetCurrent() } );
					break;
					case ETransformType::Location :
						pComp->SetRelativeLocation( FVector{ rTween.GetCurrent() } );
					break;
					case ETransformType::Rotation :
						FLinearColor val = rTween.GetCurrent();
						pComp->SetRelativeRotation( FRotator{ val.R, val.G, val.B } );
					break;
				}
			}
		}
		m_bSkipUpdateTime = bSkipUpdateTime;

		if( bStop )
		{
			if( IsLoop() )
				m_fElapseTime = 0.f;
			else
				m_bPlay = false;
		}
	}

public :
	void Play()
	{ 
		m_bPlay = true; 
		m_fElapseTime = 0.f;
	}

	bool IsPlaying() { return  m_bPlay; }

	bool IsFinished(){ return false == m_bPlay && m_fTime <= m_fElapseTime; }

	bool IsLoop()
	{ 
		return m_bLoop && ELifeCycle::TimeAutoDelete != m_eLifeCycle; 
	}


};



UCLASS(Category="Job", meta=(DisplayName ="Ani Transform"))
class _COMMON_API UJobAniTransform : public UJobComponent
{
	GENERATED_BODY()
friend struct TL::Create<UJobAniTransform, false>;

private :
//		UPROPERTY( EditAnywhere, Category="Job|Time", meta=(DisplayName = "Default Time Speed List") )
	UPROPERTY( EditAnywhere, Category="JobComponent",  meta=(DisplayName = "Ani Transform") )
	TArray<FAniTransform> 	m_AniTDatas;

	TArray<FAniTransform*> 	m_AniTDataExterns;	// ex) Referfece Nofify::FAniTransform

protected :

	void _UpdateTransfrom( FAniTransform& rAT )
	{
	}


	void _init()
	{
	}

public :
	UJobAniTransform()
	{ m_EnableTick = true; }

 	FAniTransform& operator[]( int idx )
 	{ 
		check( m_AniTDatas.IsValidIndex( idx ) );
 		return m_AniTDatas[ idx ]; 
 	}

	void Append( FAniTransform timeDilation = FAniTransform{} )
	{
		m_AniTDatas.Push( timeDilation );
	}

	// Extern : FAniTransform Data is managed by the outside.
	void AppendExtern( FAniTransform* pTimeDilation )
	{
		check(pTimeDilation);
		m_AniTDataExterns.Push( pTimeDilation );
	}

	void RemoveExtern( FAniTransform* pTimeDilation )
	{
		m_AniTDataExterns.Remove( pTimeDilation );
	}

	bool IsContainExtern( FAniTransform* pTimeDilation )
	{
		return m_AniTDataExterns.Contains( pTimeDilation );
	}



protected :
	virtual void OnDetach() override
	{
		Super::OnDetach();
	}

	virtual void Tick( float fDeltaTime ) override
	{
		Super::Tick( fDeltaTime );

		UWorld* pWorld = GetWorld();
			
		for( int i= 0; i < m_AniTDatas.Num(); ++i )
		{
			FAniTransform& rAT = m_AniTDatas[i];
			rAT.Tick( fDeltaTime );
			if( ELifeCycle::TimeAutoDelete == rAT.m_eLifeCycle && rAT.IsFinished() )
				m_AniTDatas.RemoveAt( i );
		}

		for( FAniTransform* pAT : m_AniTDataExterns )
		{
			FAniTransform& rAT = *pAT;
			rAT.Tick( fDeltaTime );
			if( ELifeCycle::TimeAutoDelete == rAT.m_eLifeCycle && rAT.IsFinished() )
				m_AniTDataExterns.Remove( pAT );
		}
	}

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty( struct FPropertyChangedChainEvent& rPropertyChangedEvent ) override
	{
		const int _index = rPropertyChangedEvent.GetArrayIndex( "m_AniTDatas" );

		for( auto* pNode = rPropertyChangedEvent.PropertyChain.GetActiveNode(); pNode; pNode = pNode->GetNextNode() )
		{
			UProperty* pProperty = pNode->GetValue();
			const FName _name = pProperty->GetFName();
			if( "m_Tag" == _name )
				m_AniTDatas[_index].__RebuildAll( GetOwner()->GetRootComponent() );
		}

		Super::PostEditChangeChainProperty( rPropertyChangedEvent );
	}
#endif
};
