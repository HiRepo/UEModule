#include "_Common.h"
#include "CommonCommand.h"

void UCommonCommand::__RegisterConsoleVariable( const TCHAR* psName, const TCHAR* psHelp )
{
#if !UE_BUILD_SHIPPING 
	IConsoleManager::Get().RegisterConsoleVariable( psName, 0, psHelp, ECVF_Cheat );
#endif
}


bool UCommonCommand::Exec( UWorld* pInWorld, const TCHAR* psCmd, FOutputDevice& Ar )
{
#if !UE_BUILD_SHIPPING 
	const TCHAR* _psCmd = psCmd;

	FString sCMD = FParse::Token( psCmd, false );
	if( m_DelegateMap.Contains(sCMD) )
	{
		FParse::Command( &psCmd, *sCMD );
		m_DelegateMap[sCMD].Broadcast( psCmd );
	}
 	else
		m_ExecEventBP.Broadcast( _psCmd );
#endif
	return false;
}


void UCommonCommand::Init()
{
	COMMAND_LIST( __COMM_INIT_SLOT );

	InitBP();
}


void UCommonCommand::SetRenderCharacterMode( bool bRenderCharacter )
{
	static TArray<AActor*> arrHide;

	UWorld* pWorld = UCommonLib::FindWorld( this );

	if( bRenderCharacter )
	{
		arrHide.Empty();
		TL::Actor<AActor>::GetAll( pWorld, arrHide, []( AActor* pActor )
			{
				if( false == pActor->GetClass()->IsChildOf( ACommonCharacter::StaticClass() ) )
				{
					if( false == pActor->bHidden && TL::Component<UStaticMeshComponent>::Get( pActor ) )
					{
						pActor->SetActorHiddenInGame( true );
						return true;
					}
				}
				return false;
			});
		m_isRenderCharacterMode = true;
	}
	else
	{
		for( AActor* pHideActor : arrHide )
		{
			pHideActor->SetActorHiddenInGame( false );
		}
		m_isRenderCharacterMode = false;
	}
}

void UCommonCommand::RenderCharacter()
{
	SetRenderCharacterMode( !m_isRenderCharacterMode );
}

