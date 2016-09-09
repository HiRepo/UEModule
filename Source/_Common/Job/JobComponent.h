#pragma once
#include "UObjectThreadContext.h"
#include "JobComponent.generated.h"

class UJobComposite;

//	class _COMMON_API UJobComponent : public UObject
UCLASS(Abstract, DefaultToInstanced, Blueprintable)
class _COMMON_API UJobComponent : public UObject
{
	GENERATED_BODY()
	friend UJobComposite;
template<class,bool> friend struct TL::Create;

protected :
	UPROPERTY(EditAnywhere, meta=(DisplayName="EnableTick"))
	bool m_EnableTick = true;

	UPROPERTY()
	AActor* m_pOwner = nullptr;

	UPROPERTY()
	UJobComposite* m_pJobComposite = nullptr;

protected :
	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected, DisplayName = "BeginPlay"))
	void _BeginPlay(); 

	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected,DisplayName = "EndPlay"))
	void _EndPlay(); 

	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected, DisplayName = "Tick"))
	void _Tick( float fDeltaTime );

	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected, DisplayName = "OnAttach"))
	void _OnAttach();                                                
                                                                     
	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected, DisplayName = "OnDetach"))
	void _OnDetach();                                                
                                                                     
	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected, DisplayName = "Do"))
	void _Do();                                                      
                                                                     
	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected, DisplayName = "Init"))
	void _Init();

protected :
	virtual void BeginPlay(){ _BeginPlay(); }
	virtual void EndPlay(){ _EndPlay(); }
	virtual void Tick( float fDeltaTime ){ if( m_EnableTick ) _Tick( fDeltaTime ); }
	virtual void OnDetach(){ _OnDetach(); }
	virtual void OnAttach()
	{ 
		if( false == FUObjectThreadContext::Get().IsRoutingPostLoad )
			_OnAttach(); 
	}
public :
	virtual void operator()(){ _Do(); } 


protected :
	UJobComponent()
	{ 
		if( false == FUObjectThreadContext::Get().IsRoutingPostLoad )
	  	 	_Init(); 
	}

	void _init(){ }

	void _SetComposite( UJobComposite* pComposite );


	FORCEINLINE
	virtual void _SetOwner( AActor* pOwner )
	{
		m_pOwner = pOwner;
	}

public :
	virtual UWorld* GetWorld() const override
	{
		return	TL::GetWorld( m_pOwner );
	}

	FORCEINLINE
	AActor* GetOwner(){ return m_pOwner; }

	FORCEINLINE
	UJobComposite* GetComposite() 
	{ 
		return m_pJobComposite; 
	}

	UJobComposite* GetRootComposite();
};




UCLASS(Abstract)
class _COMMON_API UJobComponentController : public UJobComponent
{
GENERATED_BODY()
	friend class UJobComposite;
	friend class ACommonAIController;


protected :

	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected), Category="Job|Controller")
	void _OnPossess( APawn* pPawn );

	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected), Category="Job|Controller")
	void _OnUnPossess();

	UPROPERTY(VisibleAnywhere, Transient, Category="JobComponent", meta=(DisplayName="Possessed Pawn"))
	APawn* m_pPawn = nullptr;

public :
	UFUNCTION(BlueprintPure, Category="Job|Controller")
	APawn* GetPawn()
	{ 
		return m_pPawn ? m_pPawn : Cast<APawn>( GetOwner() );
	}

protected :
	virtual void Possess( APawn* pPawn )
	{	
		m_pPawn = pPawn;
		if( pPawn )
			_OnPossess( pPawn );
	} 
	
	virtual void UnPossess()
	{
		m_pPawn = nullptr;
		_OnUnPossess();
	}
};



namespace TL 
{
	FORCEINLINE
	static void DoJob( UJobComponent* pComponent )
	{
		check( pComponent );
		(*pComponent)();
	}

	FORCEINLINE
	static void DoJob( UJobComponent& rComponent )
	{
		rComponent();
	}
}
