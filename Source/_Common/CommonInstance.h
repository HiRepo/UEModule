#pragma once
#include "Util/CommonCommand.h"
#include "CommonInstance.generated.h"

UCLASS(ClassGroup=(Common,Obj))
class _COMMON_API UCommonInstance : public UGameInstance
{
	GENERATED_BODY()
private :

protected :
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="InGameMode")
	TSubclassOf<UCommonCommand> CommandClass = nullptr;

public :
	UPROPERTY(Transient)
	UCommonCommand* m_Command = nullptr;

	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Out = *GLog) override
	{
		return m_Command->Exec( InWorld, Cmd, Out );
	}

public:
	UCommonInstance() {}
//		m_GameCommnd = (UGameCommand*)CreateDefaultSubobject("GameCommnad", UGameCommand::StaticClass(), UGameCommand::StaticClass(), /*bIsRequired =*/ true, /*bIsAbstract =*/ false, /*bTransient*/false);

	UCommonCommand* GetCommannd(){ return m_Command; }

	virtual void Init() override
	{
		if( nullptr == CommandClass )
			CommandClass = UCommonCommand::StaticClass();
		m_Command = NewObject<UCommonCommand>( this, CommandClass );
		m_Command->Init();
	}
};
