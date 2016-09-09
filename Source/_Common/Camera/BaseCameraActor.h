#pragma once
#include "BaseCamera.h"
#include "BaseCameraActor.generated.h"

class USpringArmComponent;

UCLASS(ClassGroup=Common, hideCategories=(Input, Rendering), showcategories=("Input|MouseInput", "Input|TouchInput"), Blueprintable)
class _COMMON_API ABaseCameraActor : public AActor
{
	GENERATED_UCLASS_BODY()

protected :
	UPROPERTY()
	UBaseCamera* 			m_pBaseCamera;
//		USpringArmComponent*	m_pSpringArm;


public :
    FORCEINLINE 
    UBaseCamera* GetCameraComponent() const { return m_pBaseCamera; }

//	    FORCEINLINE 
//	    USpringArmComponent* GetSpringArm() const { return m_pSpringArm; }


	virtual void PostInitProperties() override;

#if WITH_EDITOR		
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif




//		/** Returns CameraComponent subobject **/
//		class UCameraComponent* GetCameraComponent() const;


//	private_subobject:
//	
//		/** The camera component for this camera */
//		DEPRECATED_FORGAME(4.6, "CameraComponent should not be accessed directly, please use GetCameraComponent() function instead. CameraComponent will soon be private and your code will not compile.")
//		UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
//		class UCameraComponent* CameraComponent;
//	
//		UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
//		class USceneComponent* SceneComponent;
//	public:
//	
//		/** If this CameraActor is being used to preview a CameraAnim in the editor, this is the anim being previewed. */
//		TWeakObjectPtr<class UCameraAnim> PreviewedCameraAnim;
//	
//		/** Returns index of the player for whom we auto-activate, or INDEX_NONE (-1) if disabled. */
//		UFUNCTION(BlueprintCallable, Category="AutoPlayerActivation")
//		int32 GetAutoActivatePlayerIndex() const;
//	
//	public:
//		//~ Begin UObject Interface
//		virtual void Serialize(FArchive& Ar) override;
//		virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
//	
//	#if WITH_EDITOR
//		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
//	#endif
//		virtual class USceneComponent* GetDefaultAttachComponent() const override;
//	
//		//~ End UObject Interface
//		
//		//~ Begin AActor Interface
//		virtual void BeginPlay() override;
//		//~ End AActor Interface
//	
//		/** Returns CameraComponent subobject **/
//		class UCameraComponent* GetCameraComponent() const;

};

