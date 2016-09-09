#include "_CommonEditor.h"
#include "IDetailsView.h"
#include "DetailWidgetRow.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "ReflectionCapturer.h"
//	
//	#define LOCTEXT_NAMESPACE "ReflectionCapturerDetails"
//	
//	void ReflectionCapturerDetails::CustomizeDetails( IDetailLayoutBuilder& rDetailLayout )
//	{
//		const TArray<TWeakObjectPtr<UObject>>& rSelectedObjects = rDetailLayout.GetDetailsView().GetSelectedObjects();
//	
//		for( int i = 0; i < rSelectedObjects.Num(); ++i )
//		{
//			const TWeakObjectPtr<UObject>& rCurrentObject = rSelectedObjects[i];
//			if ( rCurrentObject.IsValid() )
//			{
//				if( AReflectionCapture* pCurrentCaptureActor = Cast<AReflectionCapture>( rCurrentObject.Get() ) )
//				{
//					m_ReflectionCapture = pCurrentCaptureActor;
//					break;
//				}
//			}
//		}
//	
//		rDetailLayout.EditCategory("ReflectionCapture")
//		.AddCustomRow(NSLOCTEXT("ReflectionCaptureDetails", "UpdateReflectionCaptures", "JJJJJJJJJJs Update Captures"))
//		.NameContent()
//		[
//			SNullWidget::NullWidget
//		]
//		.ValueContent()
//		[
//			SNew(SBox)
//			.WidthOverride(125)
//			[
//				SNew(SButton)
//				.ContentPadding(3)
//				.VAlign(VAlign_Center)
//				.HAlign(HAlign_Center)
//				.OnClicked(this, &ReflectionCapturerDetails::OnUpdateReflectionCaptures)
//				[
//					SNew(STextBlock)
//					.Text(NSLOCTEXT("ReflectionCaptureDetails", "UpdateReflectionCaptures", "zzzzzzzzzzzzzzzz  Update Captures"))
//					.Font(IDetailLayoutBuilder::GetDetailFont())
//				]
//			]
//		];
//	}
//	
//	FReply ReflectionCapturerDetails::OnUpdateReflectionCaptures()
//	{
//		if( m_ReflectionCapture.IsValid() )
//		{
//			GEditor->UpdateReflectionCaptures();
//		}
//	
//		return FReply::Handled();
//	}
//	
//	#undef LOCTEXT_NAMESPACE
