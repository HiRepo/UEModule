#include "_Common.h"
#include "MyStyle.h"
#include "MyHUD.h"
#include "Controller/CommonPlayerController.h"
#include "CommonGameMode.h"


TSharedPtr<ISlateStyle> FMyStyle::StylePtr = NULL;


ACommonGameMode::ACommonGameMode()
{
//		HUDClass = nullptr;
	HUDClass = AMyHUD::StaticClass();
	DefaultPawnClass =nullptr;
	PlayerControllerClass = ACommonPlayerController::StaticClass();
}
