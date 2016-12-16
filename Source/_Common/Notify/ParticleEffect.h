#pragma once
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Component/ParticleComponent.h"
#if WITH_EDITOR
#include "Edit/TL.h"
#endif
#include "ParticleEffect.generated.h"

// UAnimNotify_PlayParticleEffect

UCLASS(meta=(DisplayName="Common Particle Effect"))
class _COMMON_API UParticleEffectNotify : public UAnimNotify
{
	GENERATED_BODY()

protected :
	// Should attach to the bone/socket
	UPROPERTY(EditAnywhere, Category="AnimNotify", meta=(DisplayName="Attached"))
	bool m_isAttached		= true; 	//~ Does not follow coding standard due to redirection from BP

	UPROPERTY(EditAnywhere, Category="AnimNotify", meta=(DisplayName="Only Once when Looping"))
	bool m_isOnlyOnce		= true; 	//~ Does not follow coding standard due to redirection from BP

	// SocketName to attach to
	UPROPERTY(EditAnywhere, Category="AnimNotify", meta=(DisplayName="Socket Name"))
	FName m_nSocket			= NAME_None;

	// Particle System to Spawn
	UPROPERTY(EditAnywhere, Category="AnimNotify", meta=(DisplayName="Particle System"))
	UParticleSystem* m_pParticleSystem = nullptr;

	// Location offset from the socket
	UPROPERTY(EditAnywhere, Category="AnimNotify", meta=(DisplayName="Location Offset"))
	FVector m_LocationOffset;

	// Rotation offset from socket
	UPROPERTY(EditAnywhere, Category="AnimNotify", meta=(DisplayName="Rotation Offset"))
	FRotator m_RotationOffset;

	// Scale offset
	UPROPERTY(EditAnywhere, meta=(DisplayName="Scale Offset"))
	FVector m_ScaleOffset	= FVector{ 1.f };

	// Ani Transform 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Use AniTransform"))
	bool m_UseAniTransform		= true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="AniTransform Data"))
	FAniTransform m_AniTransform;


protected :
	FQuat  m_RotationOffsetQuat;

	virtual void PostLoad() override
	{
		Super::PostLoad();

		m_RotationOffsetQuat = FQuat( m_RotationOffset );
	}

#if WITH_EDITOR
	virtual void PostEditChangeProperty( FPropertyChangedEvent& rPropertyChangedEvent ) override
	{
		Super::PostEditChangeProperty( rPropertyChangedEvent );

		if( rPropertyChangedEvent.MemberProperty && rPropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED( UParticleEffectNotify, m_RotationOffset ) )
		{
			m_RotationOffsetQuat = FQuat( m_RotationOffset );
		}
	}
#endif


private :
	UJobAniTransform* __GetJobAniTransform( USkeletalMeshComponent* pMeshComp )
	{
		UJobAniTransform* pJobAniTransform = nullptr;
		ACommonCharacter* pCharacter = TL::Character<ACommonCharacter>::Get( pMeshComp );
		if( pCharacter )
			pJobAniTransform = &pCharacter->GetJobAniTransform();
#if WITH_EDITOR
		else
		if( !pMeshComp->GetOwner() )
		{
			UJobActorComposite* pJobComposite = TL::GetNotifyJobActorComosite( pMeshComp );
				pJobAniTransform = pJobComposite->FindJob<UJobAniTransform>();
			if( nullptr == pJobAniTransform )
				pJobAniTransform = pJobComposite->AppendNew<UJobAniTransform>( pJobComposite );
		}
#endif
		return pJobAniTransform;
	}


public:

	virtual void Notify( USkeletalMeshComponent* pMeshComp, UAnimSequenceBase* pAnimation ) override
	{
		// Don't call super to avoid unnecessary call in to blueprints
		if( nullptr == m_pParticleSystem )
		{
			UE_LOG( _Common, Warning, TEXT("Particle Notify: Null m_pParticleSystem for particle notify in anim: %s"), *GetNameSafe(pAnimation));
			return ;
		}

		if( m_pParticleSystem->IsImmortal() )
		{
			UE_LOG( _Common, Warning, TEXT("Particle Notify: Anim %s tried to spawn infinitely looping particle system %s. Spawning suppressed."), *GetNameSafe(pAnimation), *GetNameSafe(m_pParticleSystem));
			return;
		}


		UJobAniTransform* pJobAniTransform = __GetJobAniTransform( pMeshComp );
		if( nullptr == pJobAniTransform )
		{
			UE_LOG( _Common, Warning, TEXT("Particle Found Found  JobAniTransform" ) );
			return;
		}

		if( m_pParticleSystem->IsLooping() && m_isOnlyOnce )
		{
			if( pJobAniTransform->IsContainExtern( &m_AniTransform ) && m_AniTransform.GetComponents().Num() )
			{
//					if( m_UseAniTransform )
//						m_AniTransform.Play();
				return ;
			}
		}


		UWorld* pWorld = pMeshComp->GetWorld();
		UParticleComponent* pPC = NewObject<UParticleComponent>( pWorld );
		pPC->m_pAniTransform = &m_AniTransform;
		pPC->bAutoDestroy = true;
		pPC->bAutoActivate = false;
		pPC->bAllowAnyoneToDestroyMe = true;
		pPC->SecondsBeforeInactive = 0.0f;
		pPC->SetTemplate( m_pParticleSystem );
		pPC->bOverrideLODMethod = false;


		FVector vStartScale;
		FVector vStartLocation;
		FRotator rotStartRotation;

		if( m_isAttached )
		{
//			UGameplayStatics::SpawnEmitterAttached( m_pParticleSystem, pMeshComp, m_nSocket, m_LocationOffset, m_RotationOffset );
			pPC->SetupAttachment( pMeshComp, m_nSocket );
//			if( LocationType == EAttachLocation::KeepWorldPosition )
//			{
//				const FTransform _tParentToWorld = pMeshComp->GetSocketTransform( m_nSocket );
//				const FTransform _tComponentToWorld( m_RotationOffset, m_LocationOffset );
//				const FTransform _tRelativeTM = _tComponentToWorld.GetRelativeTransform( _tParentToWorld );
//				vStartLocation 	= _tRelativeTM.GetLocation();
//				vStartScale3D 	= _tRelativeTM.GetScale3D();
//				rotStartRotation= _tRelativeTM.GetRotation().Rotator();
//			}
//			else
//			{
				vStartScale = m_ScaleOffset;
				vStartLocation = m_LocationOffset;
				rotStartRotation = m_RotationOffset;
//			}

		}
		else
		{
			const FTransform meshTransform = pMeshComp->GetSocketTransform( m_nSocket );
			FTransform spawnTransform;
			spawnTransform.SetScale3D( meshTransform.GetScale3D() * m_ScaleOffset );
			spawnTransform.SetLocation( meshTransform.TransformPosition(m_LocationOffset) );
			spawnTransform.SetRotation( meshTransform.GetRotation() * m_RotationOffsetQuat );

			vStartLocation = spawnTransform.GetLocation();
			vStartScale = spawnTransform.GetScale3D();
			rotStartRotation = spawnTransform.GetRotation().Rotator();

//			UGameplayStatics::SpawnEmitterAtLocation( pMeshComp->GetWorld(), m_pParticleSystem, spawnTransform );
			pPC->bAbsoluteScale = true;
			pPC->bAbsoluteLocation = true;
			pPC->bAbsoluteRotation = true;
		}

		pPC->RelativeScale3D = vStartScale;
		pPC->RelativeLocation = vStartLocation;
		pPC->RelativeRotation = rotStartRotation;

		pPC->RegisterComponentWithWorld(pWorld);
		pPC->ActivateSystem(true);



#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		if( pPC->Template && pPC->Template->IsImmortal() )
		{
			UE_LOG( _Common, Warning, TEXT("Particle spawned potentially immortal particle system! %s (%s) may stay in world despite never spawning particles after burst spawning is over."),
				*(pPC->GetPathName()), *(pPC->Template->GetPathName())
				);
		}
#endif

		m_AniTransform.AddComponent( pPC );
		if( false == pJobAniTransform->IsContainExtern( &m_AniTransform ) )
			pJobAniTransform->AppendExtern( &m_AniTransform );


	//------------ Play Ani Transform  --------------------------
		if( false == m_UseAniTransform )
			return ;

		// Sync Animation Start value;
		const int _iNumAniTData = m_AniTransform.GetNumOfData();
		for( int i = 0; i < _iNumAniTData; ++i )
		{
			FAniTData& rATD = m_AniTransform[i];
			switch( rATD.GetType() )
			{
				case ETransformType::Scale :
					rATD.GetTween().SetSource( vStartScale );
				break;
				case ETransformType::Location :
					rATD.GetTween().SetSource( vStartLocation );
				break;
				case ETransformType::Rotation :
					rATD.GetTween().SetSource( FLinearColor{ rotStartRotation.Pitch, rotStartRotation.Yaw, rotStartRotation.Roll } );
				break;
			}
		}

		m_AniTransform.Play();
	}



//		// Begin UAnimNotify interface
//		virtual FString GetNotifyName_Implementation() const override
//		{
//			if (m_pParticleSystem)
//			{
//				return m_pParticleSystem->GetName();
//			}
//			else
//			{
//				return Super::GetNotifyName_Implementation();
//			}
//		}
//		// End UAnimNotify interface

};

