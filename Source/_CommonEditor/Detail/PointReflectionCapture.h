#pragma once
#include "Engine/ReflectionCapture.h"
#include "DetailLayoutBuilder.h"
#include "IDetailsView.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailCustomization.h"
#include "LevelEditor.h"
#include "Actor/PointReflectionCapture.h"

class PointReflectionCaptureDetails: public IDetailCustomization
{
private :
	TArray<TWeakObjectPtr<APointReflectionCapture>> m_arrReflectionCapture;


public:
	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable( new PointReflectionCaptureDetails );
	}

private:

	FReply OnUpdateReflectionCaptures()
	{
		struct PointReflectionData
		{ 
			bool bEnableLight;
			APointReflectionCapture*  pPointReflection;
		};
		TArray<PointReflectionData> arrPointRefectionData;

		APointReflectionCapture* pPointReflection = nullptr; 

		// Turn on the light
		for( TWeakObjectPtr<APointReflectionCapture>& wpPointReflection : m_arrReflectionCapture )
		{
			if( wpPointReflection.IsValid() )
			{
				APointReflectionCapture* pPRC = wpPointReflection.Get();
				arrPointRefectionData.Add( PointReflectionData{ pPRC->GetEnableLights(), pPRC } );
//					pPRC->SetEnableLights( true );
				if( nullptr == pPointReflection )
					pPointReflection = pPRC;
			}
		}
		if( nullptr == pPointReflection )
			return FReply::Handled();

		if( m_arrReflectionCapture.Num() )
			GEditor->UpdateReflectionCaptures();

//			// Turn off the light.
//			for( PointReflectionData& rPRD : arrPointRefectionData )
//			{
//				if( false == rPRD.bEnableLight )
//					rPRD.pPointReflection->SetEnableLights( false );
//			}

//			pPointReflection->ReregisterAllComponents();

//			pPointReflection->GetNextTickDelegate().BindRaw( this, &PointReflectionCaptureDetails::__StartUpdateRefectionCaptures );
//			pPointReflection->RegisterNextTickEvent();

		return FReply::Handled();
	}


	void __StartUpdateRefectionCaptures()
	{
		// Update Reflection
//			FlushRenderingCommands();
//			FPlatformProcess::Sleep(1.0f);
//			if( m_arrReflectionCapture.Num() )
//				GEditor->UpdateReflectionCaptures();

		for( TWeakObjectPtr<APointReflectionCapture>& wpPointReflection : m_arrReflectionCapture )
		{
			if( wpPointReflection.IsValid() )
				wpPointReflection->SetEnableLights( false );
		}

//			// Turn off the light.
//			for( PointReflectionData& rPRD : arrPointRefectionData )
//			{
//				if( false == rPRD.bEnableLight )
//					rPRD.pPointReflection->SetEnableLights( false );
//			}
	}


	FReply OnAddPointLight()
	{
		for( TWeakObjectPtr<APointReflectionCapture>& wpPointReflection : m_arrReflectionCapture )
		{
			if( wpPointReflection.IsValid() )
				wpPointReflection->AddPointLight();
		}

		// Update Tree ( Component )
//		NewComponent = AddNewNodeForInstancedComponent( NewInstanceComponent, Asset );
		FLevelEditorModule& rLevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>( "LevelEditor" );
		rLevelEditor.BroadcastComponentsEdited();
		return FReply::Handled();
	}


	/** IDetailCustomization interface */
	virtual void CustomizeDetails( IDetailLayoutBuilder& rDetailLayout ) override
	{
		const TArray<TWeakObjectPtr<UObject>>& _rSelectedObjectArray = rDetailLayout.GetDetailsView().GetSelectedObjects();
		for( const TWeakObjectPtr<UObject>& rSelectObject : _rSelectedObjectArray )
		{
			if( rSelectObject.IsValid() )
			{
				if( auto* pCaptureActor = Cast<APointReflectionCapture>( rSelectObject.Get() ) )
					m_arrReflectionCapture.Add( pCaptureActor );
			}
		}

#define LOCTEXT_NAMESPACE "PointReflectionCaptureDetails"
		IDetailCategoryBuilder& rReflectionCaptureCategory = rDetailLayout.EditCategory( "ReflectionCapture" );

		rReflectionCaptureCategory.AddCustomRow( LOCTEXT("UpdateReflectionCaptures", "Update Captures") )
		.NameContent() [ SNullWidget::NullWidget ]
		.ValueContent()
		[
			SNew( SBox )
			.WidthOverride( 125 )
			[
				SNew( SButton )
				.ContentPadding( 3 )
				.VAlign( VAlign_Center )
				.HAlign( HAlign_Center )
				.OnClicked(this, &PointReflectionCaptureDetails::OnUpdateReflectionCaptures)
				[
					SNew( STextBlock )
					.Text( LOCTEXT("UpdateReflectionCaptures", "Update Captures") )
					.Font( IDetailLayoutBuilder::GetDetailFont() )
				]
			]
		];

		rReflectionCaptureCategory.AddCustomRow( LOCTEXT("AddPointLight", "Add PointLight") )
		.NameContent() [ SNullWidget::NullWidget ]
		.ValueContent()
		[
			SNew( SBox )
			.WidthOverride( 125 )
			[
				SNew( SButton )
				.ContentPadding( 3 )
				.VAlign( VAlign_Center )
				.HAlign( HAlign_Center )
				.OnClicked(this, &PointReflectionCaptureDetails::OnAddPointLight)
				[
					SNew( STextBlock )
					.Text( LOCTEXT("AddPointLight", "Add PointLight") )
					.Font( IDetailLayoutBuilder::GetDetailFont() )
				]
			]
		];
#undef LOCTEXT_NAMESPACE
	}
};

