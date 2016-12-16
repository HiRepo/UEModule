#pragma once
#include "Components/SphereReflectionCaptureComponent.h"
#include "Components/DrawSphereComponent.h"
#include "EditorSupportDelegates.h"
#include "PointReflectionCapture.generated.h"

DECLARE_DELEGATE( FNextTick )

UCLASS(ClassGroup=(Common,Actor), MinimalAPI, HideCategories=(Collision, Attachment, Actor))
class APointReflectionCapture : public AActor
{
	GENERATED_BODY()
private :
	UPROPERTY(VisibleAnywhere, Category=DecalActor, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	USphereReflectionCaptureComponent* m_pSphereReflectionComponent;

	UPROPERTY()
	UDrawSphereComponent* m_pDrawCaptureRadius;

	UPROPERTY(EditAnywhere, Category=ReflectionCapture, meta=(DisplayName="Enable Light"))
	bool m_bEnableLights = false;

	FTimerHandle m_LoopTimerHandle;

public :
	bool GetEnableLights(){ return m_bEnableLights; }

	UReflectionCaptureComponent* GetCaptureComponent() const { return m_pSphereReflectionComponent; }

	UDrawSphereComponent* GetDrawCaptureRadius() const { return m_pDrawCaptureRadius; }

	APointReflectionCapture()
	{ 
		SetActorHiddenInGame( true );

		// Add USphereReflectionCaptureComponent
		m_pSphereReflectionComponent = CreateDefaultSubobject<USphereReflectionCaptureComponent>(TEXT("NewReflectionComponent"));
		RootComponent = m_pSphereReflectionComponent;
		{
			UDrawSphereComponent* pDrawInfluenceRadius = CreateDefaultSubobject<UDrawSphereComponent>(TEXT("DrawRadius0"));
//			pDrawInfluenceRadius->SetupAttachment( m_pSphereReflectionComponent );
			pDrawInfluenceRadius->SetupAttachment( m_pSphereReflectionComponent );
			pDrawInfluenceRadius->bDrawOnlyIfSelected = true;
			pDrawInfluenceRadius->bUseEditorCompositing = true;
			pDrawInfluenceRadius->SetCollisionProfileName( UCollisionProfile::NoCollision_ProfileName );
			m_pSphereReflectionComponent->PreviewInfluenceRadius = pDrawInfluenceRadius;
		}

		// Add UDrawSphereComponent
		m_pDrawCaptureRadius = CreateDefaultSubobject<UDrawSphereComponent>(TEXT("DrawRadius1"));
		m_pDrawCaptureRadius->SetupAttachment( RootComponent );
		m_pDrawCaptureRadius->bDrawOnlyIfSelected = true;
		m_pDrawCaptureRadius->bUseEditorCompositing = true;
		m_pDrawCaptureRadius->SetCollisionProfileName( UCollisionProfile::NoCollision_ProfileName );
		m_pDrawCaptureRadius->ShapeColor = FColor(100, 90, 40);


		// Add Sphere Component
		static ConstructorHelpers::FObjectFinder<UStaticMesh> s_SphereMesh(TEXT("/Engine/EngineMeshes/Sphere"));
		static ConstructorHelpers::FObjectFinder<UMaterialInterface> s_Material(TEXT("/_Common/Material/PointReflection"));
		auto* pShapeComponent = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
		pShapeComponent->SetupAttachment( RootComponent );
	//	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Engine/EditorLandscapeResources/AlphaBrushMaterial"));
		pShapeComponent->SetStaticMesh( s_SphereMesh.Object );
		pShapeComponent->SetMaterial( 0, s_Material.Object );
		pShapeComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
		pShapeComponent->bOwnerNoSee = true;
		pShapeComponent->bCastDynamicShadow = false;
		pShapeComponent->bGenerateOverlapEvents = false;
		pShapeComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		pShapeComponent->SetCanEverAffectNavigation(false);
		pShapeComponent->SetRelativeScale3D( FVector(0.3) );
	//	pShapeComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	}

//		virtual bool IsLevelBoundsRelevant() const override { return false; }



#if WITH_EDITOR
protected :
	virtual void PostEditMove( bool bFinished ) override
	{
		Super::PostEditMove( bFinished );

		m_pSphereReflectionComponent->SetCaptureIsDirty();
	}

	virtual void PostEditChangeProperty( FPropertyChangedEvent& rPropertyChangedEvent ) override
	{
		if( nullptr == rPropertyChangedEvent.Property )			
			return	Super::PostEditChangeProperty( rPropertyChangedEvent );

		const FName _memberName = rPropertyChangedEvent.MemberProperty->GetFName();
		const FName _propertyName = rPropertyChangedEvent.Property->GetFName();
		if( "m_bEnableLights" == _memberName )
			SetEnableLights( m_bEnableLights );
		Super::PostEditChangeProperty( rPropertyChangedEvent );
	}

	virtual void PostLoadSubobjects( FObjectInstancingGraph* pOuterInstanceGraph ) override
	{
		Super::PostLoadSubobjects( pOuterInstanceGraph );
	}

	void EditorApplyScale( const FVector& vDeltaScale, const FVector* PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown )
	{
		const FVector ModifiedScale = vDeltaScale * ( AActor::bUsePercentageBasedScaling ? 5000.0f : 50.0f );
		FMath::ApplyScaleToFloat( m_pSphereReflectionComponent->InfluenceRadius, ModifiedScale );
		m_pSphereReflectionComponent->SetCaptureIsDirty();
		PostEditChange();
	}

private :
	FNextTick m_NextTickDelegate;

	void __OnNextTickEvent()
	{
		if(	m_NextTickDelegate.IsBound() )
			m_NextTickDelegate.Execute();
	}

public :
	FNextTick& GetNextTickDelegate(){ return m_NextTickDelegate; }

	void RegisterNextTickEvent()
	{
		GetWorld()->GetTimerManager().SetTimer( m_LoopTimerHandle, this, &APointReflectionCapture::__OnNextTickEvent, 1.0f, false );
//			m_NextTickDelegate.Bind( this, &APointReflectionCapture::__OnNextTickEvent );
	}

	void AddPointLight()
	{
//			UPointLightComponent* pPointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
		UClass* pClass = UPointLightComponent::StaticClass();

		FName name = *FComponentEditorUtils::GenerateValidVariableName( pClass, this );
		UPointLightComponent* pPointLight = NewObject<UPointLightComponent>( this, pClass, name, RF_Transactional );
//		UPointLightComponent* pPointLight = NewObject<UPointLightComponent>( this, pClass, name, RF_Transactional );
		pPointLight->Mobility = EComponentMobility::Static;
		pPointLight->AttachToComponent( RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale );
		
//	FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor").


//	  FKismetEditorUtilities
//			if( EditorMode == EComponentEditorMode::BlueprintSCS)
//	Cast<UBlueprint>(ActorClass->ClassGeneratedBy)
      	// Add to SerializedComponents array so it gets saved
		AddInstanceComponent(pPointLight);
		pPointLight->OnComponentCreated();
		pPointLight->RegisterComponent();
		pPointLight->bAffectsWorld = m_bEnableLights;

		// Rerun construction scripts
		RerunConstructionScripts();
	}

#endif

	void SetEnableLights( bool bEnable =true )
	{
		m_bEnableLights = bEnable;

		TArray<UPointLightComponent*> arrPointLight;
		TL::Component<UPointLightComponent>::GetAll( this, arrPointLight );
		for( UPointLightComponent* pPointLight : arrPointLight )
		{
			pPointLight->bAffectsWorld = bEnable;
			GetWorld()->Scene->UpdateLightColorAndBrightness( pPointLight );
		}

		if( bEnable )
		{

		}
//			ReregisterAllComponents();
//				RerunConstructionScripts();
//				GEngine->BroadcastOnActorMoved( this );
//				GetWorld()->bDoDelayedUpdateCullDistanceVolumes = true;
//				FEditorSupportDelegates::UpdateUI.Broadcast();
	}

};



//	class AReflectionCapture : public AActor
//	{
//	#if WITH_EDITORONLY_DATA
//		UPROPERTY()
//		UBillboardComponent* SpriteComponent;
//	
//		UPROPERTY()
//		UBillboardComponent* CaptureOffsetComponent;
//
//		/** Returns SpriteComponent subobject **/
//		ENGINE_API UBillboardComponent* GetSpriteComponent() const;
//		ENGINE_API UBillboardComponent* GetCaptureOffsetComponent() const { return CaptureOffsetComponent; }
//
//	#endif // WITH_EDITORONLY_DATA
//	};
//	


//	AReflectionCapture::AReflectionCapture(const FObjectInitializer& ObjectInitializer)	: Super(ObjectInitializer)
//	{
//	#if WITH_EDITORONLY_DATA
//		SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
//		if (!IsRunningCommandlet() && (SpriteComponent != nullptr))
//		{
//			// Structure to hold one-time initialization
//			struct FConstructorStatics
//			{
//				FName NAME_ReflectionCapture;
//				ConstructorHelpers::FObjectFinderOptional<UTexture2D> DecalTexture;
//				FConstructorStatics()
//					: NAME_ReflectionCapture(TEXT("ReflectionCapture"))
//					, DecalTexture(TEXT("/Engine/EditorResources/S_ReflActorIcon"))
//				{
//				}
//			};
//			static FConstructorStatics ConstructorStatics;
//	
//			SpriteComponent->Sprite = ConstructorStatics.DecalTexture.Get();
//			SpriteComponent->RelativeScale3D = FVector(0.5f, 0.5f, 0.5f);
//			SpriteComponent->bHiddenInGame = true;
//			SpriteComponent->bAbsoluteScale = true;
//			SpriteComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
//			SpriteComponent->bIsScreenSizeScaled = true;
//		}
//	
//		CaptureOffsetComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("CaptureOffset"));
//		if (!IsRunningCommandlet() && (CaptureOffsetComponent != nullptr))
//		{
//			// Structure to hold one-time initialization
//			struct FConstructorStatics
//			{
//				FName NAME_ReflectionCapture;
//				ConstructorHelpers::FObjectFinderOptional<UTexture2D> DecalTexture;
//				FConstructorStatics()
//					: NAME_ReflectionCapture(TEXT("ReflectionCapture"))
//					, DecalTexture(TEXT("/Engine/EditorResources/S_ReflActorIcon"))
//				{
//				}
//			};
//			static FConstructorStatics ConstructorStatics;
//	
//			CaptureOffsetComponent->Sprite = ConstructorStatics.DecalTexture.Get();
//			CaptureOffsetComponent->RelativeScale3D = FVector(0.2f, 0.2f, 0.2f);
//			CaptureOffsetComponent->bHiddenInGame = true;
//			CaptureOffsetComponent->bAbsoluteScale = true;
//			CaptureOffsetComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
//			CaptureOffsetComponent->bIsScreenSizeScaled = true;
//		}
//	
//		if (CaptureComponent)
//		{
//			CaptureComponent->CaptureOffsetComponent = CaptureOffsetComponent;
//		}
//		
//	#endif // WITH_EDITORONLY_DATA
//	}
//	


