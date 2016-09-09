#include "_Common.h"
#include "MyStyle.h"

DEFINE_LOG_CATEGORY(_Common)

class FCommonModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		FSlateStyleRegistry::UnRegisterSlateStyle( "PreloadStyle" );
		FMyStyle::Initialize();
	}
	virtual void ShutdownModule() override
	{
		FMyStyle::Shutdown();
	}
 
};

IMPLEMENT_GAME_MODULE( FCommonModule, _Common );
