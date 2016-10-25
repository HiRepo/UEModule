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

	UPROPERTY(Transient, BlueprintReadOnly)
	UCommonCommand* m_Command = nullptr;

public :
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Out = *GLog) override
	{
		return m_Command->Exec( InWorld, Cmd, Out );
	}

	virtual void Init() override
	{
		if( nullptr == CommandClass )
			CommandClass = UCommonCommand::StaticClass();
		m_Command = NewObject<UCommonCommand>( this, CommandClass );
		m_Command->Init();
	}

public:
	UCommonInstance() {}

	UCommonCommand* GetCommannd(){ return m_Command; }
};
