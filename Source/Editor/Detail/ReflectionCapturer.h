#pragma once
#include "Engine/ReflectionCapture.h"
#include "DetailLayoutBuilder.h"
#include "IDetailsView.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailCustomization.h"

#define LOCTEXT_NAMESPACE "ReflectionCapturerDetails"

class ReflectionCapturerDetails: public IDetailCustomization
{
private :
	/** The selected reflection capture */
	TWeakObjectPtr<AReflectionCapture> m_ReflectionCapture;

public:
	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable( new ReflectionCapturerDetails );
	}

private:

	FReply OnUpdateReflectionCaptures()
	{
//			if( m_ReflectionCapture.IsValid() )
//			{
			GEditor->UpdateReflectionCaptures();
//			}

		return FReply::Handled();
	}


	/** IDetailCustomization interface */
	virtual void CustomizeDetails( IDetailLayoutBuilder& rDetailLayout ) override
	{
		const TArray<TWeakObjectPtr<UObject>>& rSelectedObjects = rDetailLayout.GetDetailsView().GetSelectedObjects();
		for( int i = 0; i < rSelectedObjects.Num(); ++i )
		{
			const TWeakObjectPtr<UObject>& rCurrentObject = rSelectedObjects[i];
			if ( rCurrentObject.IsValid() )
			{
				if( AReflectionCapture* pCurrentCaptureActor = Cast<AReflectionCapture>( rCurrentObject.Get() ) )
				{
					m_ReflectionCapture = pCurrentCaptureActor;
					break;
				}
			}
		}

		rDetailLayout.EditCategory( "ReflectionCapture" )
		.AddCustomRow( LOCTEXT("UpdateReflectionCaptures", "JJJJJJJJJJs Update Captures") )
		.NameContent()
		[
			SNullWidget::NullWidget
		]
		.ValueContent()
		[
			SNew( SBox )
			.WidthOverride( 125 )
			[
				SNew( SButton )
				.ContentPadding( 3 )
				.VAlign( VAlign_Center )
				.HAlign( HAlign_Center )
				.OnClicked(this, &ReflectionCapturerDetails::OnUpdateReflectionCaptures)
				[
					SNew( STextBlock )
					.Text( LOCTEXT("UpdateReflectionCaptures", "zzzzzzzzzzzzzzzz  Update Captures") )
					.Font( IDetailLayoutBuilder::GetDetailFont() )
				]
			]
		];
	}


};

#undef LOCTEXT_NAMESPACE
