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
	ETransformType	Type 	= ETransformType::Scale;

	UPROPERTY( EditAnywhere )
	FTween 	Tween;

public :
	FAniTData()
	{
		SetType( Type, true );
	}

	FTween& GetTween(){ return Tween; }

	ETransformType GetType() { return Type; }

	void SetType( ETransformType eType, bool bInitialize = true )
	{
		Type = eType;
		if( bInitialize )
		{
			if( ETransformType::Scale == Type )
				Tween.SetSource( FLinearColor{ 1.f, 1.f, 1.f, 1.f } );
			else	
				Tween.SetSource( FLinearColor{ FVector::ZeroVector } );
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
	bool  IsPlaying 		= false;

	UPROPERTY( VisibleAnywhere )
	ELifeCycle LifeCycle 	= ELifeCycle::Time;

	UPROPERTY( EditAnywhere, meta=(UIMin = 0, ClampMin = 0) )
	float 	Time			= 1.f;

	UPROPERTY( EditAnywhere, meta=(UIMin = 0, ClampMin = 0) )
	float 	ElapseTime 		= 0.f;

	UPROPERTY( EditAnywhere )
	FName	EnableTag		= NAME_None;

	UPROPERTY( EditAnywhere )
	TArray<FAniTData>  ArrAniData;

	UPROPERTY( VisibleAnywhere, Transient )
	TArray<USceneComponent*> ArrComponent;

public :
	FAniTransform()
	{
	}

 	FAniTData& operator[]( int idx ) 
 	{ 
		check( ArrAniData.IsValidIndex( idx ) );
 		return ArrAniData[idx]; 
 	}

	ELifeCycle GetLifeCycle(){ return LifeCycle; }

	float GetTime(){ return Time; }

	int GetNumOfData(){ return ArrAniData.Num(); }

	void SetLifeCycle( ELifeCycle eLifeCycle )
	{
		LifeCycle = eLifeCycle;
	}

	void SetTime( float fTime )
	{
	}

	void AddComponent( USceneComponent* pComp )
	{
		ArrComponent.AddUnique( pComp );
	}

	void RemoveComponent( USceneComponent* pComp )
	{
		ArrComponent.Remove( pComp );
	}

	TArray<USceneComponent*>& GetComponents(){ return  ArrComponent; }

private :
	bool __IsOverTime(){ return Time < ElapseTime; }

	void __RebuildAll( USceneComponent* pRootComponent )
	{
		ArrComponent.Reset();
		TL::Component<USceneComponent>::TagAll( pRootComponent, ArrComponent, EnableTag );
	}


	DECLARE_DELEGATE_OneParam( DeleAniTransform, USceneComponent* )
	DeleAniTransform	m_EventAniTrasnform;

	void Tick( float fDeltaTime )
	{
		if( false == IsPlaying )
			return;
		if( Time < ElapseTime )
		{
			ElapseTime = 0.f;
			if( ELifeCycle::Infinity != LifeCycle )
				IsPlaying = false;
			return;
		}

		ElapseTime += fDeltaTime;

		const float _fPercent = __IsOverTime() ? 1.f : ElapseTime / Time;

		for( USceneComponent* pComp : ArrComponent )
		{
			for( FAniTData& rATD :  ArrAniData )
			{
				switch( rATD.Type )
				{
					case ETransformType::Scale :
						pComp->SetRelativeScale3D ( FVector{ rATD.Tween.GetValue( _fPercent ) } );
					break;
					case ETransformType::Location :
						pComp->SetRelativeLocation( FVector{ rATD.Tween.GetValue( _fPercent ) } );
					break;
					case ETransformType::Rotation :
						FLinearColor val = rATD.Tween.GetValue( _fPercent );
						pComp->SetRelativeRotation( FRotator{ val.R, val.G, val.B } );
					break;
				}
			}

		}

	}

public :
	void Play()
	{ 
		IsPlaying = true; 
		ElapseTime = 0.f;
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
			if( rAT.__IsOverTime() )
			{
				if( ELifeCycle::TimeAutoDelete == rAT.LifeCycle && rAT.__IsOverTime() )
					m_AniTDatas.RemoveAt( i );
			}
		}

		for( FAniTransform* pAT : m_AniTDataExterns )
		{
			FAniTransform& rAT = *pAT;
			rAT.Tick( fDeltaTime );
			if( ELifeCycle::TimeAutoDelete == rAT.LifeCycle && rAT.__IsOverTime() )
				m_AniTDataExterns.Remove( pAT );
		}

//			_UpdateCurTimeDilation();
	}

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty( struct FPropertyChangedChainEvent& rPropertyChangedEvent ) override
	{
		const int _index = rPropertyChangedEvent.GetArrayIndex( "m_AniTDatas" );

		for( auto* pNode = rPropertyChangedEvent.PropertyChain.GetActiveNode(); pNode; pNode = pNode->GetNextNode() )
		{
			UProperty* pProperty = pNode->GetValue();
			const FName _name = pProperty->GetFName();
			if( "EnableTag" == _name )
				m_AniTDatas[_index].__RebuildAll( GetOwner()->GetRootComponent() );
		}

		Super::PostEditChangeChainProperty( rPropertyChangedEvent );
	}
#endif
};
