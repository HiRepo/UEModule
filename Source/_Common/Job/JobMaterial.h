#pragma once
#include "JobComponent.h"
#include "Util/Tween.h"
#include "Material/TL.h"
#include "JobMaterial.generated.h"

UENUM(BlueprintType)
enum class EMatParamLeng : uint8
{
	Scalar,
	Vector,
};


USTRUCT()
struct FMatData
{
	GENERATED_BODY()

	friend struct FJobMat;
	friend class UJobMaterial;

protected :
	UPROPERTY( EditAnywhere, meta=(DisplayName="Parameter Name") )
	FName	m_ParameterName;		

	UPROPERTY( EditAnywhere, meta=(DisplayName="Tween") )
	FTween 	m_Tween;

	UPROPERTY( EditAnywhere, meta=(DisplayName="ElementLeng"))
	EMatParamLeng  	m_eMatParamLeng 	= EMatParamLeng::Vector;

	UPROPERTY( EditAnywhere, meta=(ClampMin="0", ClampMax="1", UIMin="-1", UIMax="15", DisplayName="Start Time Rate") )
	float 	m_fStartTimeRate = 0.f;

	UPROPERTY( EditAnywhere, meta=(ClampMin="0", ClampMax="1", UIMin="-1", UIMax="15", DisplayName="End Time Rate"))
	float 	m_fEndTimeRate	= 1.f;
	
private :
//		UPROPERTY( VisibleAnywhere, Transient, meta=(DisplayName="Percent") )
//		float 	m_fPercent 	= 0.f;


public :
	FTween& GetTween() { return m_Tween; }

	EMatParamLeng GetMatParamLeng(){ return m_eMatParamLeng; }

	void SetTarget( float val ) { m_Tween.SetTarget( val ); }

	void SetTarget( FLinearColor val ) { m_Tween.SetTarget( val ); }

	void SetTweenFlow( ETweenFlow eFlow ) { m_Tween.SetFlowType( eFlow ); }

	void SetMatParamLeng( EMatParamLeng eMatParamLeng )
	{
		m_eMatParamLeng = eMatParamLeng;
		if( EMatParamLeng::Vector == eMatParamLeng )
			m_Tween.SetLengType( ETweenLeng::Element4 );
		else
			m_Tween.SetLengType( ETweenLeng::Element1 );
	}

	void SetData( FName paraName, EMatParamLeng eMatParamLeng, ETweenFlow eFlow = ETweenFlow::Forward, ETweenCurve eCurve =ETweenCurve::Linear )
	{
		m_ParameterName = paraName;
		m_Tween.SetFlowType( eFlow );
		m_Tween.SetCurveType( eCurve );

		SetMatParamLeng( eMatParamLeng );
	}

	void SetSourceFromMaterial( UMaterialInterface* pBaseMaterial )
	{
		if( nullptr == pBaseMaterial || m_ParameterName == NAME_None )
			return;
		FLinearColor val;
		if( EMatParamLeng::Vector == m_eMatParamLeng )
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
	FName 	m_BaseName;				// Selector. if Name is None, not filtering.

	UPROPERTY( EditAnywhere, meta=(DisplayName="Component Tag") )
	FName 	m_CompTag = NAME_None;	// Selector. if Tag is None, not filtering.

	UPROPERTY( EditAnywhere, meta=(DisplayName="Ignore Other Actor") )
	bool 	m_bIgnoreOtherActor = true;		

	UPROPERTY( EditAnywhere, meta=(DisplayName="Is Loop") )
	bool	m_bLoop 	= false;

	UPROPERTY( EditAnywhere, meta=(DisplayName="Play") )
	bool 	m_bPlay 	= false;

	UPROPERTY( EditAnywhere, meta=(ClampMin="0", DisplayName="Time") )
	float 	m_fTime		= 1.f;

	UPROPERTY( EditAnywhere, meta=(ClampMin="0", DisplayName="Elapse Time") )
	float 	m_fElapseTime	= 0.f;

	UPROPERTY( EditAnywhere, meta=(DisplayName="+Data Array") )
	TArray<FMatData> m_Datas;

private :
	friend class UJobMaterial;

	UPROPERTY( VisibleAnywhere, Transient ,meta=(DisplayName="Dynamic  Mat Array"))
	TArray<UMaterialInstanceDynamic*> m_MIDArray;

	UPROPERTY( VisibleAnywhere, meta=(DisplayName="Original Mat Array") )
	TArray<UMaterialInterface*> m_MatArray;

	bool m_bSkipUpdateTime = false;

public :
 	FMatData& operator[]( int idx ) 
 	{ 
		check( m_Datas.IsValidIndex( idx ) );
 		return m_Datas[idx]; 
 	}

	void SetLoop( bool isLoop = true )
	{
		m_bLoop = isLoop;
	}

	void SetBaseName( FName name )
	{
		m_BaseName = name;
	}


	bool IsPlaying(){ return m_bPlay; }

	bool IsLoop(){ return m_bLoop; }

	void Play()
	{	
		m_bPlay = true;
	}

	void Stop( bool isChangeMaterial = true )
	{
		m_bPlay = false;
		m_fElapseTime = 0.f;
		if( isChangeMaterial )
			(*this)();
	}

	void SetTime( const float fTime )
	{
		m_fTime = fTime;
	}

	void SetTarget( int dataIdx, float val )
	{
		(*this)[dataIdx].SetTarget( val );
	}

	void SetTarget( int dataIdx, FLinearColor val  )
	{
		(*this)[dataIdx].SetTarget( val );
	}

public :

	void SetData( int dataIdx, FName paraName, EMatParamLeng eMatParamLeng, ETweenFlow eFlow = ETweenFlow::Forward, ETweenCurve eCurve =ETweenCurve::Linear )
	{
		int iNumData = m_Datas.Num();
		check( dataIdx <= iNumData  );
		if( dataIdx == iNumData )	// Append,  ! SetData functions's index shoube be progressively appended;
		{
			FMatData matData;
			matData.SetData( paraName, eMatParamLeng, eFlow, eCurve );
			m_Datas.Add( matData );
		}
		else						// Assign
		{
			FMatData& rMatData = m_Datas[dataIdx];
			rMatData.SetData( paraName, eMatParamLeng, eFlow, eCurve );
		}
	}

	void AppendData( FName paraName, EMatParamLeng eMatParamLeng, ETweenFlow eFlow = ETweenFlow::Forward )
	{
		FMatData matData;
		matData.SetData( paraName, eMatParamLeng, eFlow );
		m_Datas.Add( matData );
	}


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


	void SetParameter( int index, const float fValue ) 
	{
 		FMatData& rMatData = (*this)[index];
		check( EMatParamLeng::Scalar == rMatData.GetMatParamLeng() );

		for( UMaterialInstanceDynamic* pMID : m_MIDArray )
		{
			pMID->SetScalarParameterValue( rMatData.m_ParameterName, fValue );
		}
	}

	void SetParameter( int index, const FLinearColor& vColor ) 
	{
 		FMatData& rMatData = (*this)[index];
		check( EMatParamLeng::Vector == rMatData.GetMatParamLeng() );

		for( UMaterialInstanceDynamic* pMID : m_MIDArray )
		{
			pMID->SetVectorParameterValue( rMatData.m_ParameterName, vColor );
		}
	}


public :
	void operator()()
	{
		const float _fTime = m_fTime;
		const float _fElapseTime = m_fElapseTime;

		bool bSkipUpdateTime = false;
		for( FMatData& rMatData : m_Datas )
		{
			FTween& rTween = rMatData.m_Tween;

			const float _fStartTime = rMatData.m_fStartTimeRate * _fTime;
			const float _fEndTime = rMatData.m_fEndTimeRate * _fTime;
			if( _fElapseTime < _fStartTime )
				continue;

			const float _fPercent = (_fElapseTime - _fStartTime)  / (_fEndTime - _fStartTime);
			if( 1.f < _fPercent && rTween.IsOverTime() )
				continue;

		 	if( false == rTween.Update( _fPercent, m_bLoop ) )
				bSkipUpdateTime = true;

			for( UMaterialInstanceDynamic* pMID : m_MIDArray )
			{
				if( EMatParamLeng::Vector == rMatData.GetMatParamLeng() )
					pMID->SetVectorParameterValue( rMatData.m_ParameterName, rTween.GetCurrent() );
				else 
					pMID->SetScalarParameterValue( rMatData.m_ParameterName, rTween.GetCurrentR() );
			}
		}
		m_bSkipUpdateTime = bSkipUpdateTime;
	}

};




UCLASS()
class _COMMON_API UJobMaterial : public UJobComponent
{
	GENERATED_BODY()
friend struct TL::Create<UJobMaterial, false>;

protected :

	virtual void _SetOwner( AActor* pOwner ) override
	{
		Super::_SetOwner( pOwner );
		RebuildMat( true );
	}

	void _ResetMatArray()
	{
		for( auto& elem : m_JobMatData )
		{
			FJobMat& rJobMat = elem.Value;
			rJobMat.m_MatArray.Reset();
			rJobMat.m_MIDArray.Reset();
		}
	}
	
	void _init(){	}

protected :
	UPROPERTY( EditAnywhere, Category="JobComponent" )
	TMap<FName, FJobMat> m_JobMatData;

	FJobMat* _FindFirstData()
	{
		if( m_JobMatData.Num() )
			return &m_JobMatData.CreateIterator().Value();
		return nullptr;
	}

public :
	UJobMaterial()
	{ m_EnableTick = true; }

 	const FJobMat& operator[]( const FName key ) const
 	{ 
		check( m_JobMatData.Contains( key ) );
 		return m_JobMatData[key]; 
 	}

 	FJobMat& operator[]( const FName key )
 	{ 
 		return m_JobMatData.FindOrAdd( key ); 
 	}

	FJobMat* FindJobMat( FName key )
	{
		return m_JobMatData.Find( key );
	}

	void RebuildMat( FName key, bool isCreateMID = true )
	{
		RebuildMat( (*this)[key], isCreateMID );
	}

	// Rebuild MID All
	void RebuildMat( bool isCreateMID = true )
	{
		for( auto& elem : m_JobMatData )
		{
			RebuildMat( elem.Value, isCreateMID );
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

		if( rJobMat.m_bIgnoreOtherActor )
			TL::Component<UMeshComponent>::TagAll( pOwner, meshComponentArray, rJobMat.m_CompTag );
		else
			TL::Component<UMeshComponent>::TagAll( pOwner->GetRootComponent(), meshComponentArray, rJobMat.m_CompTag );

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

	void SetSourceFromMaterial( FName key )
	{
		(*this)[key].SetSourceFromMaterial();
	}

	void SetSourceFromMaterial( FName key, int dataIdx )
	{
		(*this)[key].SetSourceFromMaterial( dataIdx );
	}

	void SetSourceFromMaterial()
	{
		for( auto& elem : m_JobMatData )
		{
			elem.Value.SetSourceFromMaterial();
		}
	}

	FMatData& GetMatData( FName key=NAME_None, int dataIdx = 0  )
	{
		return (*this)[key][dataIdx];
	}

	void SetTime( const float fTime, FName key =NAME_None )
	{
		if( key == NAME_None )
		{
			if( FJobMat* pJobMat = _FindFirstData() )
				(*pJobMat).SetTime( fTime );
		}
		else
			(*this)[key].SetTime( fTime );
	}

	void Assign( FMatData& rMatData, FName key, int dataIdx = 0 )
	{
		(*this)[key][dataIdx] = rMatData;
	}

	void AppendMatData( FName key )
	{
		if( false ==  m_JobMatData.Contains( key ) )
			m_JobMatData.Add( key );
	}

	void AppendMatData( FName key, FJobMat& rJobMat, bool isCreateMID = false )
	{
		if( m_JobMatData.Contains( key ) )
			m_JobMatData[key] = rJobMat;
		else
			m_JobMatData.Add( key, rJobMat );
		RebuildMat( rJobMat, isCreateMID );
	}

	void AppendMatData( FName key, FName baseMatName = NAME_None, FName compTag = NAME_None, 
		FName paraName = NAME_None, EMatParamLeng eMatParamLeng =EMatParamLeng::Vector, 
		ETweenFlow eMatFlow = ETweenFlow::Forward, bool isCreateMID = false )
	{
		FJobMat jobMat;

		jobMat.m_BaseName = baseMatName;
		jobMat.m_CompTag = compTag;

		FMatData matData;
		matData.SetData( paraName, eMatParamLeng, eMatFlow );
		jobMat.m_Datas.Add( matData );
		
		AppendMatData( key, jobMat, isCreateMID );

		jobMat.SetSourceFromMaterial();
	}

	bool IsPlayState()
	{
		for( auto& elem : m_JobMatData )
		{
			FJobMat& rJobMat = elem.Value;
			if( rJobMat.m_bPlay )
				return true;
		}
		return false;
	}


protected :
#if WITH_EDITOR

	virtual void PostEditChangeProperty( FPropertyChangedEvent& rPropertyChangedEvent ) override
	{
		if( nullptr == rPropertyChangedEvent.Property )			
			return	Super::PostEditChangeProperty(rPropertyChangedEvent);

		const FName _memberName = rPropertyChangedEvent.MemberProperty->GetFName();
		const FName _propertyName = rPropertyChangedEvent.Property->GetFName();
		if( _memberName == "m_JobMatData" )
		{
			if( "m_fElapseTime" == _propertyName || "m_fTime" == _propertyName )
				(*this)();
			else
			if( "m_CompTag" == _propertyName || "m_BaseName" == _propertyName )
				RebuildMat();
			else
			if( "m_Tween" == _propertyName )
			{
				RebuildMat();
				(*this)();
			}
		}
		Super::PostEditChangeProperty(rPropertyChangedEvent);
	}

//		virtual void PostEditChangeChainProperty( struct FPropertyChangedChainEvent& rPropertyChangedEvent ) override
//		{
//			const int _indexMat = rPropertyChangedEvent.GetArrayIndex( "JobMatArray" );
//			const int _indexData = rPropertyChangedEvent.GetArrayIndex( "m_Datas" );
//	
//			for( auto* pNode = rPropertyChangedEvent.PropertyChain.GetActiveNode(); pNode; pNode = pNode->GetNextNode() )
//			{
//				FName name = pNode->GetValue()->GetFName();
//				if( "m_fElapseTime" == name || "m_fTime" == name )
//					JobMatArray[_indexMat]();
//				else
//				if( "m_CompTag" == name || "m_BaseName" == name )
//					RebuildMat( _indexData );
//				else
//				if( "m_Tween" == name )
//				{
//					RebuildMat( _indexData );
//					JobMatArray[_indexMat]();
//				}
//			}
//	
//			Super::PostEditChangeChainProperty( rPropertyChangedEvent );
//		}
#endif


	virtual void Tick( float fDeltaTime ) override
	{
		Super::Tick( fDeltaTime );
		bool isPlay = false;
		for( auto& elem : m_JobMatData )
		{
			FJobMat& rJobMat = elem.Value;
			if( rJobMat.IsPlaying() )
			{
				isPlay = true;
				if( rJobMat.m_bSkipUpdateTime )
					continue;
				rJobMat.m_fElapseTime += fDeltaTime;
			}
		}
		if( false == isPlay )
			return;

		Update();

		for( auto& elem : m_JobMatData )
		{
			FJobMat& rJobMat = elem.Value;
			if( false == rJobMat.IsPlaying() )
				continue;

			if( rJobMat.m_fTime < rJobMat.m_fElapseTime )		// Over time
			{
				if( rJobMat.IsLoop() )
					rJobMat.m_fElapseTime = 0.f;
				else
					rJobMat.Stop( false );
			}
		}
	}


public :
	bool IsPlaying()
	{
		bool isPlaying = false;
		for( auto& elem : m_JobMatData )
		{
			FJobMat& rJobMat = elem.Value;
			if( rJobMat.IsPlaying() )
			{
				isPlaying = true;
				break;
			}
		}
		return isPlaying;
	}

	void Play( FName key =NAME_None )
	{
		if( key == NAME_None )
		{
			if( FJobMat* pJobMat = _FindFirstData() )
				(*pJobMat).Play();
		}
		else
			(*this)[key].Play();
	}

	void Stop( FName key =NAME_None )
	{
		if( key == NAME_None )
		{
			if( FJobMat* pJobMat = _FindFirstData() )
				(*pJobMat).Stop();
		}
		else
			(*this)[key].Stop();
	}

public :
	void Update()
	{
		Super::operator()();

		for( auto& elem : m_JobMatData )
		{
			FJobMat& rJobMat = elem.Value;
			if( rJobMat.IsPlaying() )
				rJobMat();
		}
	}

	virtual void operator()() override
	{
		for( auto& elem : m_JobMatData )
		{
			elem.Value();
		}
	}
};
