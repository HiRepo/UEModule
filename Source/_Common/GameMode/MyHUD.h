#pragma once
#include "SWidgetSwitcher.h"
#include "MyStyle.h"
#include "MyHUD.generated.h"


class SMyUIWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SMyUIWidget){}

    /*See private declaration of OwnerHUD below.*/
	SLATE_ARGUMENT(TWeakObjectPtr<class AMyHUD>, OwnerHUD)

	/** The visual style of the button */
	SLATE_END_ARGS()
 

private:
	TWeakObjectPtr<class AMyHUD> OwnerHUD;
 
	int32 TabIndex;
 
	int32 GetCurrentTabIndex() const
	{
		return TabIndex;
	}

public:
int FirstTabActive  = 1;
int SecondTabActive = 0;

int ftClick = 1;
int stClick = 0;

	const FSlateBrush* GetFirstImageBrush() const
	{
		FName BrushName;
		(FirstTabActive == 0) ? BrushName = TEXT("tab_normal") : BrushName = TEXT("tab_active");
	 
		return FMyStyle::Get().GetBrush(BrushName);
	}
	const FSlateBrush* GetSecondImageBrush() const
	{
		FName BrushName;
		(SecondTabActive == 0) ? BrushName = TEXT("tab_normal") : BrushName = TEXT("tab_active");
	 
		return FMyStyle::Get().GetBrush(BrushName);
	}
 

	FReply FirstTabClicked()
	{
		TabIndex = 0;
		return FReply::Handled();
	}
	 
	FReply SecondTabClicked()
	{
		TabIndex = 1;
		return FReply::Handled();
	}


	void Construct(const FArguments& InArgs)
	{
		OwnerHUD = InArgs._OwnerHUD;
	 
		TabIndex = 0;
	 
		ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.FillHeight(0.2)										// 탭영역 Slot지정
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()							// 첫번째 탭
				[
					SNew(SButton)
					.OnClicked(this, &SMyUIWidget::FirstTabClicked)	// 클릭시 이벤트
					[
						SNew(SBorder)
						.BorderImage(this, &SMyUIWidget::GetFirstImageBrush)
						[
							SNew(STextBlock)
							.Font(FSlateFontInfo("Veranda", 54))
							.ColorAndOpacity(FLinearColor(0, 0, 0, 1))
							.Text(FText::FromString("Page One"))
						]
					]
				]

				+ SHorizontalBox::Slot()
				.FillWidth(0.1)										// 중간 비워둠

				+ SHorizontalBox::Slot()							// 두번째 탭
				[
					SNew(SButton)
					.OnClicked(this, &SMyUIWidget::SecondTabClicked)// 클릭시 이벤트
					[
						SNew(SBorder)
						.BorderImage(this, &SMyUIWidget::GetSecondImageBrush)
						[
							SNew(STextBlock)
							.Font(FSlateFontInfo("Veranda", 54))
							.ColorAndOpacity(FLinearColor(0, 0, 0, 1))
							.Text(FText::FromString("Page Two"))
						]
					]
				]
			]

			+ SVerticalBox::Slot()
			.FillHeight(0.8)
			[
				SNew(SWidgetSwitcher)
				.WidgetIndex(this, &SMyUIWidget::GetCurrentTabIndex)
				+ SWidgetSwitcher::Slot() 
				[
					SNew(SBorder)
					.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
					[
						SNew(STextBlock)
						.ColorAndOpacity(FLinearColor(1, 1, 1, 1))
						.Font(FSlateFontInfo("Veranda", 72))
						.Text(FText::FromString("Page One"))
					]
				]
				+ SWidgetSwitcher::Slot() 
				[
					SNew(SBorder)
					.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
					[
						SNew(STextBlock)
						.ColorAndOpacity(FLinearColor(1, 1, 1, 1))
						.Font(FSlateFontInfo("Veranda", 72))
						.Text(FText::FromString("Page Two"))
					]
				]
			]
		];
	}



};



UCLASS()
class AMyHUD : public AHUD
{
	GENERATED_BODY()
 
private:
	TSharedPtr<SMyUIWidget> MyUIWidget;

public:
	void BeginPlay()
	{
		SAssignNew(MyUIWidget, SMyUIWidget).OwnerHUD(this);
	 
		if (GEngine->IsValidLowLevel())
			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MyUIWidget.ToSharedRef()));
	 
		if (MyUIWidget.IsValid())
			MyUIWidget->SetVisibility(EVisibility::Visible);
	}
 
	virtual void DrawHUD() override
	{
		Super::DrawHUD();
	}
};

