#include "_CommonEditor.h"
#include "UnrealEd.h"
#include "ModuleManager.h"
#include "ModuleInterface.h"
#include "PropertyEditorModule.h"
#include "PropertyEditorDelegates.h"
#include "Detail/PointReflectionCapture.h"

DEFINE_LOG_CATEGORY(_CommonEditor)

//bool operator==(const FName& rNameLeft, const FName& rNameRight)
//{
//	return Name == *sName;
//}
//FORCEINLINE bool operator==(const FName& Other) const
//{
//#if WITH_CASE_PRESERVING_NAME
//	return GetComparisonIndexFast() == Other.GetComparisonIndexFast() && GetNumber() == Other.GetNumber();
//#else
//	static_assert(sizeof(CompositeComparisonValue) == sizeof(*this), "ComparisonValue does not cover the entire FName state");
//	return CompositeComparisonValue == Other.CompositeComparisonValue;
//#endif
//}
//FORCEINLINE bool operator!=(const FName& Other) const
//{
//	return !(*this == Other);
//}


class FCommonEditorModule : public IModuleInterface
{
private:
	/** List of registered class that we must unregister when the module shuts down */
	TSet<FName> m_RegisteredClassNames;
	TSet<FName> m_RegisteredPropertyTypes;


	void __RegisterCustomClassLayout( FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate )
	{
		check( ClassName != NAME_None );
		m_RegisteredClassNames.Add( ClassName );

		static FName s_PropertyEditor("PropertyEditor");
		FPropertyEditorModule& rPropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(s_PropertyEditor);
		rPropertyModule.RegisterCustomClassLayout( ClassName, DetailLayoutDelegate );
	}

	void __RegisterCustomPropertyTypeLayout( FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate )
	{
		check( PropertyTypeName != NAME_None );
		m_RegisteredPropertyTypes.Add( PropertyTypeName );

		static FName s_PropertyEditor( "PropertyEditor" );
		FPropertyEditorModule& rPropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(s_PropertyEditor);
		rPropertyModule.RegisterCustomPropertyTypeLayout( PropertyTypeName, PropertyTypeLayoutDelegate );
	}



public:
	virtual void StartupModule() override
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
//		__RegisterCustomPropertyTypeLayout("CaptureResolution", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FCaptureResolutionCustomization::MakeInstance));
//		__RegisterCustomPropertyTypeLayout("CaptureProtocolID", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FCaptureTypeCustomization::MakeInstance));
//		__RegisterCustomPropertyTypeLayout("MaterialProxySettings", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FMaterialProxySettingsCustomizations::MakeInstance));

		__RegisterCustomClassLayout("PointReflectionCapture", FOnGetDetailCustomizationInstance::CreateStatic(&PointReflectionCaptureDetails::MakeInstance));

		PropertyModule.NotifyCustomizationModuleChanged();
	}

	virtual void ShutdownModule() override
	{
		if( FModuleManager::Get().IsModuleLoaded("PropertyEditor") )
		{
			FPropertyEditorModule& rPropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

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


//		/**
//		 * Override this to set whether your module is allowed to be unloaded on the fly
//		 * @return	Whether the module supports shutdown separate from the rest of the engine.
//		 */
//		virtual bool SupportsDynamicReloading() override
//		{
//			return true;
//		}

};


IMPLEMENT_GAME_MODULE( FCommonEditorModule, _CommonEditor );
