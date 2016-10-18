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
