#pragma once
#include "Engine.h"
#include "SlateBasics.h"
#include "SlateStyle.h"

class FMyStyle 
{

public:
	static TSharedPtr<class ISlateStyle> StylePtr;

	static const ISlateStyle& Get() { return *StylePtr; }

	static void Initialize()
	{
		if (!StylePtr.IsValid())
		{
#define IMAGE_BRUSH(RelativePath, ...)	FSlateImageBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
			TSharedPtr<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("PreloadStyle"));
			Style->SetContentRoot(FPaths::GameContentDir() / "Slate");
			Style->Set("tab_normal", new IMAGE_BRUSH("tab_normal", FVector2D(256, 64)));
			Style->Set("tab_active", new IMAGE_BRUSH("tab_active", FVector2D(256, 64)));
#undef IMAGE_BRUSH
			StylePtr = Style;
			FSlateStyleRegistry::RegisterSlateStyle(*StylePtr);
		}
	}

	static void Shutdown()
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StylePtr);
		ensure(StylePtr.IsUnique());
		StylePtr.Reset();
	}
};
