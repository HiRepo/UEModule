#pragma once
#include "JobComponent.h"
#include "Util/Tween.h"
#include "Material/TL.h"
#include "JobMaterial.generated.h"


UENUM(BlueprintType)
enum class EMatFlow : uint8
{
	None = 0,
	Forward,
	Backward,
	Return,
};



USTRUCT()
struct FMatData
{
	GENERATED_BODY()

	friend struct FJobMat;
	friend class UJobMaterial;

protected :
	UPROPERTY( EditAnywhere, meta=(DisplayName="Flow") )
	EMatFlow m_eFlow;

	UPROPERTY( EditAnywhere, meta=(DisplayName="Parameter Name") )
	FName	m_ParameterName;		

	UPROPERTY( EditAnywhere, meta=(DisplayName="Tween") )
	FTween 	m_Tween;

	UPROPERTY( EditAnywhere, meta = (ClampMin = "0", ClampMax = "1", UIMin = "-1", UIMax = "15", DisplayName="Start Time Rate") )
	float 	m_fStartTimeRate = 0.f;

	UPROPERTY( EditAnywhere, meta = (ClampMin = "0", ClampMax = "1", UIMin = "-1", UIMax = "15", DisplayName="End Time Rate"))
	float 	m_fEndTimeRate	= 1.f;
	
private :
	UPROPERTY( VisibleAnywhere, Transient, meta=(DisplayName="Percent") )
	float 	m_fPercent 	= 0.f;


public :
	FORCEINLINE
	void SetTweenFlag( FTween::EFlag eFlag )	{ m_Tween.SetFlag( eFlag );  }

	FORCEINLINE	
	void SetTarget( float val ) { m_Tween.SetTarget( val ); }

	FORCEINLINE	
	void SetTarget( FLinearColor val ) { m_Tween.SetTarget( val ); }

	FORCEINLINE	
	void SetTweenLeng( ETweenLeng eTweenLeng )
	{
		check( ETweenLeng::Element1 == eTweenLeng || ETweenLeng::Element4 == eTweenLeng );
		m_Tween.SetElementLeng( eTweenLeng );
	}

	void SetData( FName paraName, ETweenLeng eTweenLeng, EMatFlow eMatFlow = EMatFlow::Forward )
	{
		m_ParameterName = paraName;
		m_eFlow = eMatFlow;
		SetTweenLeng( eTweenLeng );
	}

	void SetSourceFromMaterial( UMaterialInterface* pBaseMaterial )
	{
		if( nullptr == pBaseMaterial || m_ParameterName == NAME_None )
			return;
		FLinearColor val;
		if( ETweenLeng::Element4 == m_Tween.GetElementLeng() )
			pBaseMaterial->GetVectorParameterValue( m_ParameterName, val );
		else 
		{
			float fScalar;
			pBaseMaterial->GetScalarParameterValue( m_ParameterName, fScalar );
			val.R = fScalar;
		}
		m_Tween.SetSource( val );
	}
};


USTRUCT(meta=(DisplayName ="Job Material"))
struct FJobMat
{
	GENERATED_BODY()

protected :
	UPROPERTY( EditAnywhere, meta=(DisplayName="Base Name") )
	FName 	m_BaseName;		// Selector. if Name is None, not filtering.

	UPROPERTY( EditAnywhere, meta=(DisplayName="Tag") )
	FName 	m_Tag;			// Selector. if Tag is None, not filtering.

	UPROPERTY( EditAnywhere, meta=(DisplayName="Is Loop") )
	bool	m_bLoop 	= false;

	UPROPERTY( EditAnywhere, meta=(DisplayName="Play") )
	bool 	m_bPlay 	= false;

	UPROPERTY( EditAnywhere, meta=(DisplayName="Time") )
	float 	m_fTime		= 1.f;

	UPROPERTY( EditAnywhere, meta=(DisplayName="Elapse Time") )
	float 	m_fElapseTime	= 0.f;

	UPROPERTY( EditAnywhere, meta=(DisplayName="+Data Array") )
	TArray<FMatData> m_Datas;
	
private :
	friend class UJobMaterial;

	UPROPERTY( VisibleAnywhere, Transient ,meta=(DisplayName="Dynamic  Mat Array"))
	TArray<UMaterialInstanceDynamic*> m_MIDArray;

	UPROPERTY( VisibleAnywhere, meta=(DisplayName="Original Mat Array") )
	TArray<UMaterialInterface*> m_MatArray;

public :
	FORCEINLINE
 	FMatData& operator[]( int idx ) 
 	{ 
		check( m_Datas.IsValidIndex( idx ) );
 		return m_Datas[idx]; 
 	}

	FORCEINLINE
	void SetLoop( bool isLoop = true )
	{
		m_bLoop = isLoop;
	}

	FORCEINLINE
	void SetBaseName( FName name )
	{
		m_BaseName = name;
	}


	FORCEINLINE
	bool IsPlaying(){ return m_bPlay; }

	FORCEINLINE
	void SetPlay( bool isPlay = true )
	{
		m_bPlay = isPlay;
	}

	FORCEINLINE
	void SetTime( const float fTime )
	{
		m_fTime = fTime;
	}

	FORCEINLINE
	void SetTarget( int dataIdx, float val )
	{
		(*this)[dataIdx].SetTarget( val );
	}

	FORCEINLINE
	void SetTarget( int dataIdx, FLinearColor val  )
	{
		(*this)[dataIdx].SetTarget( val );
	}

	FORCEINLINE
	void SetTweenFlag( int dataIdx, FTween::EFlag eFlag )	
	{
		(*this)[dataIdx].SetTweenFlag( eFlag );;
	}

	FORCEINLINE
	void SetTweenFlag( FTween::EFlag eFlag )	
	{
		for( FMatData& rMatData : m_Datas )
			rMatData.SetTweenFlag( eFlag );
	}

public :
	void SetData( int dataIdx, FName paraName, ETweenLeng eTweenLeng, EMatFlow eMatFlow = EMatFlow::Forward )
	{
		int iNumData = m_Datas.Num();
		check( dataIdx <= iNumData  );
		if( dataIdx == iNumData )	// Append,  ! SetData functions's index shoube be progressively appended;
		{
			FMatData matData;
			matData.SetData( paraName, eTweenLeng, eMatFlow );
			m_Datas.Add( matData );
		}
		else						// Assign
		{
			FMatData& rMatData = m_Datas[dataIdx];
			rMatData.SetData( paraName, eTweenLeng, eMatFlow );
		}
	}

	void AppendData( FName paraName, ETweenLeng eTweenLeng, EMatFlow eMatFlow = EMatFlow::Forward )
	{
		FMatData matData;
		matData.SetData( paraName, eTweenLeng, eMatFlow );
		m_Datas.Add( matData );
	}


	FORCEINLINE
	void AppendData( const FMatData& rMatData )
	{
		m_Datas.Add( rMatData );
	}


	void SetSourceFromMaterial()
	{
		const int _iNum = m_Datas.Num();
		for( int i = 0; i < _iNum; ++i )
		{
			SetSourceFromMaterial( i );
		}
	}

	void SetSourceFromMaterial( int dataIdx )
	{
		if( !m_MatArray.Num() )
			return ;

		FMatData& rMatData = (*this)[dataIdx];
		rMatData.SetSourceFromMaterial( m_MatArray[0] );
	}


private :
	void __SetTweenFlow( FTween& rTween, const EMatFlow eMatFlow, float fPercent )
	{
		if( EMatFlow::Forward == eMatFlow && !rTween.IsForward() )
			rTween.SetForward();
		else
		if( EMatFlow::Backward == eMatFlow && !rTween.IsBackward() )
			rTween.SetBackward();
		else
		if( EMatFlow::Return == eMatFlow  )
		{
			if( rTween.IsForward() && 1.f < fPercent  )
				rTween.SetBackward();
			else
//				if( rTween.IsBackward() && ( fPercent < 1.f || 2.f < fPecent ) )
			if( rTween.IsBackward() && fPercent < 1.f  )
				rTween.SetForward();
		}
	}

public :
	void operator()()
	{
		const float _fTime = m_fTime;
		const float _fElapseTime = m_fElapseTime;

		for( FMatData& rMatData : m_Datas )
		{
			FTween& rTween = rMatData.m_Tween;
			const EMatFlow _eMatFlow = rMatData.m_eFlow;
			if( EMatFlow::None == rMatData.m_eFlow )
				continue;

			const float _fStartTime = rMatData.m_fStartTimeRate * _fTime;
			const float _fEndTime = rMatData.m_fEndTimeRate * _fTime;
			if( _fElapseTime < _fStartTime )
				continue;

			const float _fOldPercent = rMatData.m_fPercent;
			float fPercent = (_fElapseTime - _fStartTime)  / (_fEndTime - _fStartTime) ;
			if( EMatFlow::Return == _eMatFlow )
				fPercent *= 2.f;
			rMatData.m_fPercent = fPercent;

			if( _fOldPercent < 1.f && 1.f < fPercent )
				fPercent = 1.f;
			if( _fOldPercent < 2.f && 2.f < fPercent )
				fPercent = 1.f;

			__SetTweenFlow( rTween, _eMatFlow, fPercent );

			if( EMatFlow::Return == _eMatFlow &&  1.f < fPercent )
				fPercent = FMath::Fmod( fPercent, 1.f );

			for( UMaterialInstanceDynamic* pMID : m_MIDArray )
			{
				if( ETweenLeng::Element4 == rTween.GetElementLeng()  )
					pMID->SetVectorParameterValue( rMatData.m_ParameterName, rTween.GetValue( fPercent ) );
				else 
					pMID->SetScalarParameterValue( rMatData.m_ParameterName, rTween.GetValueX( fPercent ) );
			}
		}
	}

};




UCLASS()
class _COMMON_API UJobMaterial : public UJobComponent
{
	GENERATED_BODY()
friend struct TL::Create<UJobMaterial, false>;

protected :

	FORCEINLINE
	virtual void _SetOwner( AActor* pOwner ) override
	{
		Super::_SetOwner( pOwner );
		RebuildMat( true );
	}

	void _ResetMatArray()
	{
		for( FJobMat& rJobMat :	JobMatArray )
		{
			rJobMat.m_MatArray.Reset();
			rJobMat.m_MIDArray.Reset();
		}
	}
	
	void _init(){	}

public :
	UJobMaterial()
	{ m_EnableTick = true; }

	//UJobMaterial( AActor* pMeshComponent ) { _init(pMeshComponent); }
	UPROPERTY( EditAnywhere, Category="JobComponent" )
	TArray<FJobMat> JobMatArray;

	FORCEINLINE
 	FJobMat& operator[]( int idx ) 
 	{ 
		check( JobMatArray.IsValidIndex( idx ) );
 		return JobMatArray[idx]; 
 	}

	FORCEINLINE
	FJobMat& GetJobMat( int idx = 0 )
	{
		return (*this)[ idx ];
	}

	FORCEINLINE
	void RebuildMat( int idx, bool isCreateMID = true )
	{
		RebuildMat( (*this)[idx], isCreateMID );
	}

	// Rebuild MID All
	void RebuildMat( bool isCreateMID = true )
	{
		for( FJobMat& rJobMat :	JobMatArray )
		{
			RebuildMat( rJobMat, isCreateMID );
		}
	}

	void RebuildMat( FJobMat& rJobMat, bool isCreateMID = true )
	{
		rJobMat.m_MIDArray.Reset();
		rJobMat.m_MatArray.Reset();

		AActor* pOwner = GetOwner();
		if( !GetComposite() || !pOwner )
			return;

		TArray<UMeshComponent*> meshComponentArray;
		TL::Component<UMeshComponent>::TagAll( pOwner, meshComponentArray, rJobMat.m_Tag );
		for( UMeshComponent* pMeshComponent  : meshComponentArray )
		{
			TArray<UMaterialInterface*> matArray;
			TL::Material<UMaterialInterface>::GetAll( matArray, pMeshComponent, rJobMat.m_BaseName );
			for( UMaterialInterface* pMaterial : matArray )
			{
				// if Material is UMaterialInstanceDynamic
				if( auto* pCastMID = Cast<UMaterialInstanceDynamic>(pMaterial) )
				{
					rJobMat.m_MatArray.Add( pCastMID->Parent );
					rJobMat.m_MIDArray.Add( pCastMID );
				}
				else
				{
//					rJobMat.m_MatArray.Add( pMaterial->GetBaseMaterial() );  	Original Shader
					rJobMat.m_MatArray.Add( pMaterial );
					if( isCreateMID )
					{
						const int _matIdx = TL::MaterialIndex( pMeshComponent, pMaterial );
						if( auto pMID =	pMeshComponent->CreateDynamicMaterialInstance(_matIdx) )
							rJobMat.m_MIDArray.Add( pMID );
					}
				}
			}
		}

	}


	FORCEINLINE
	void SetSourceFromMaterial( int jobMatIdx )
	{
		(*this)[jobMatIdx].SetSourceFromMaterial();
	}

	FORCEINLINE
	void SetSourceFromMaterial( int jobMatIdx, int dataIdx )
	{
		(*this)[jobMatIdx].SetSourceFromMaterial( dataIdx );
	}

	FORCEINLINE
	void SetSourceFromMaterial()
	{
		for( FJobMat& rJobMat :	JobMatArray )
			rJobMat.SetSourceFromMaterial();
	}


	FORCEINLINE
	FMatData& GetMatData( int jobMatIdx = 0, int dataIdx = 0  )
	{
		return (*this)[jobMatIdx][dataIdx];
	}

	FORCEINLINE
	void SetTime( const float fTime, int jobMatIdx = 0 )
	{
		(*this)[jobMatIdx].SetTime( fTime );
	}


	void Assign( FJobMat& rJobMat, int jobMatIdx =0 )
	{
		(*this)[jobMatIdx] = rJobMat;
	}

	void Assign( FMatData& rMatData, int jobMatIdx, int dataIdx = 0 )
	{
		(*this)[jobMatIdx][jobMatIdx] = rMatData;
	}

	void AppendMatData( FJobMat& rJobMat, bool isCreateMID = false )
	{
		JobMatArray.Add( rJobMat );
		RebuildMat( rJobMat, isCreateMID );
	}

	void AppendMatData( FName baseMatName = NAME_None, FName tag = NAME_None, 
		FName paraName = NAME_None, ETweenLeng eTweenLeng =ETweenLeng::Element4, 
		EMatFlow eMatFlow = EMatFlow::Forward, bool isCreateMID = false )
	{
		FJobMat jobMat;

		jobMat.m_BaseName = baseMatName;
		jobMat.m_Tag = tag;

		FMatData matData;
		matData.SetData( paraName, eTweenLeng, eMatFlow );
		jobMat.m_Datas.Add( matData );
		
		JobMatArray.Add( jobMat );

		RebuildMat( jobMat, isCreateMID );

		jobMat.SetSourceFromMaterial();
	}

	bool IsPlayState()
	{
		for( FJobMat& rJobMat :	JobMatArray )
		{
			if( rJobMat.m_bPlay )
				return true;
		}
		return false;
	}


protected :
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty( struct FPropertyChangedChainEvent& rPropertyChangedEvent ) override
	{
		const int _index = rPropertyChangedEvent.GetArrayIndex( "m_AniTDatas" );

		for( auto* pNode = rPropertyChangedEvent.PropertyChain.GetActiveNode(); pNode; pNode = pNode->GetNextNode() )
		{
			FName name = pNode->GetValue()->GetFName();
			if( "m_Tag" == name || "m_BaseName" == name )
				RebuildMat( _index );
			if( "m_fElapseTime" ==	name )
				(*this)();
			if( "m_eFlow" == name )
			{
				RebuildMat( _index );
				(*this)();
			}
		}

		Super::PostEditChangeChainProperty( rPropertyChangedEvent );
	}
#endif

	void __OvertimeMatrialReset()
	{
		for( FJobMat& rJobMat :	JobMatArray )
		{
			for( FMatData& rMatData : rJobMat.m_Datas )
			{
				for( UMaterialInstanceDynamic* pMID : rJobMat.m_MIDArray )
				{
					FTween& rTween = rMatData.m_Tween;
					FLinearColor vLastValue;
//						if( rTween.IsForward() )
//							vLastValue = rTween.GetTarget();
//						else
						vLastValue = rTween.GetSource();

					if( ETweenLeng::Element4 == rTween.GetElementLeng()  )
						pMID->SetVectorParameterValue( rMatData.m_ParameterName, vLastValue );
					else
						pMID->SetScalarParameterValue( rMatData.m_ParameterName, vLastValue.R );
				}
			}
		}
	}

	virtual void Tick( float fDeltaTime ) override
	{
		Super::Tick( fDeltaTime );
		bool isPlay = false;
		for( FJobMat& rJobMat :	JobMatArray )
		{
			if( rJobMat.m_bPlay )
			{
				rJobMat.m_fElapseTime += fDeltaTime;
				isPlay = true;
			}
		}

		if( !isPlay )
			return;			// ignore

		(*this)();			// Do Job 

		for( FJobMat& rJobMat :	JobMatArray )
		{

			if( !rJobMat.m_bPlay )
				continue;

			const bool _isOverTime = rJobMat.m_fTime < rJobMat.m_fElapseTime;
			// this is for Editor
			if( _isOverTime )
			{
				rJobMat.m_fElapseTime = 0.f;
				__OvertimeMatrialReset();
			}

			// Loop
			if (rJobMat.m_bLoop)
				break;	

			// Stop
			for( FMatData& rMatData : rJobMat.m_Datas )
			{
				if( EMatFlow::None == rMatData.m_eFlow )
					continue;

				if(	_isOverTime )
					rJobMat.m_bPlay = false;	
			}
			
		}
	}



public :
	void Play( int jobMatIdx = 0 )
	{
		(*this)[jobMatIdx].SetPlay( true );
	}

	void Stop( int jobMatIdx = 0 )
	{
		(*this)[jobMatIdx].SetPlay( false );
	}

public :
	virtual void operator()() override
	{
		Super::operator()();
		for( FJobMat& rJobMat :	JobMatArray )
		{
			rJobMat();
		}
//			for( FJobMat& rJobMat :	JobMatArray )
//			{
//				const float _fTime = rJobMat.m_fTime;
//				const float _fElapseTime = rJobMat.m_fElapseTime;
//				for( FMatData& rMatData : rJobMat.m_Datas )
//				{
//					const EMatFlow _eMatFlow = rMatData.m_eFlow;
//					if( EMatFlow::None == rMatData.m_eFlow )
//						continue;
//	
//					const float _fStartTime = rMatData.m_fStartTimeRate * _fTime;
//					const float _fEndTime = rMatData.m_fEndTimeRate * _fTime;
//					if( _fElapseTime < _fStartTime )
//						continue;
//	
//					float fPercent = (_fElapseTime - _fStartTime)  / (_fEndTime - _fStartTime) ;
//					if( EMatFlow::Return == _eMatFlow )
//						fPercent *= 2.f;
//	
//					FTween& rTween = rMatData.m_Tween;
//					__SetTweenFlow( rTween, _eMatFlow, fPercent );
//	
//					const float _fOldPercent = rMatData.m_fPercent;
//					rMatData.m_fPercent = fPercent;
//					if( 1.f < fPercent )
//					{
//						if( _fOldPercent < 1.f )
//							fPercent = 1.f;
//						else
//							fPercent = FMath::Fmod( fPercent, 1.f );
//					}
//	
//					for( UMaterialInstanceDynamic* pMID : rJobMat._MIDArray )
//					{
//						if( rTween.ElementLeng == ETweenLeng::Element4 )
//							pMID->SetVectorParameterValue( rMatData.m_ParameterName, rTween.GetValue( fPercent ) );
//						else 
//							pMID->SetScalarParameterValue( rMatData.m_ParameterName, rTween.GetValueX( fPercent ) );
//					}
//				}
//			}
	}
};
