#include "_CommonEditor.h"
#include "UnrealEd.h"
#include "CommonModule.h"
#include "ModuleManager.h"
#include "PropertyEditorDelegates.h"
#include "Detail/PointReflectionCapture.h"

DEFINE_LOG_CATEGORY(_CommonEditor)


void FCommonModule::_RegisterCustomClassLayout( FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate )
{
	check( ClassName != NAME_None );
	m_RegisteredClassNames.Add( ClassName );

	static FName s_PropertyEditor("PropertyEditor");
	FPropertyEditorModule& rPropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(s_PropertyEditor);
	rPropertyModule.RegisterCustomClassLayout( ClassName, DetailLayoutDelegate );

}

void FCommonModule::_RegisterCustomPropertyTypeLayout( FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate )
{
	check( PropertyTypeName != NAME_None );
	m_RegisteredPropertyTypes.Add( PropertyTypeName );

	static FName s_PropertyEditor( "PropertyEditor" );
	FPropertyEditorModule& rPropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(s_PropertyEditor);
	rPropertyModule.RegisterCustomPropertyTypeLayout( PropertyTypeName, PropertyTypeLayoutDelegate );
}


void FCommonModule::ShutdownModule()
{
	if( FModuleManager::Get().IsModuleLoaded( "PropertyEditor" ) )
	{
		FPropertyEditorModule& rPropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>( "PropertyEditor" );

		// Unregister all classes customized by name
		for( auto It = m_RegisteredClassNames.CreateConstIterator(); It; ++It )
		{
			if( It->IsValid() )
				rPropertyModule.UnregisterCustomClassLayout(*It);
		}

		// Unregister all structures
		for( auto It = m_RegisteredPropertyTypes.CreateConstIterator(); It; ++It )
		{
			if( It->IsValid() )
				rPropertyModule.UnregisterCustomPropertyTypeLayout(*It);
		}
	
		rPropertyModule.NotifyCustomizationModuleChanged();
	}
}


class FCommonEditorModule : public FCommonModule
{
private:

public:
	virtual void StartupModule() override
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
//		_RegisterCustomPropertyTypeLayout("CaptureResolution", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FCaptureResolutionCustomization::MakeInstance));
//		_RegisterCustomPropertyTypeLayout("CaptureProtocolID", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FCaptureTypeCustomization::MakeInstance));
//		_RegisterCustomPropertyTypeLayout("MaterialProxySettings", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FMaterialProxySettingsCustomizations::MakeInstance));

		_RegisterCustomClassLayout("PointReflectionCapture", FOnGetDetailCustomizationInstance::CreateStatic(&PointReflectionCaptureDetails::MakeInstance));

		PropertyModule.NotifyCustomizationModuleChanged();
	}
};


IMPLEMENT_GAME_MODULE( FCommonEditorModule, _CommonEditor );
