#pragma once
#include "ModuleInterface.h"
#include "PropertyEditorModule.h"

class _COMMONEDITOR_API FCommonModule : public IModuleInterface
{
protected :
	/** List of registered class that we must unregister when the module shuts down */
	TSet<FName> m_RegisteredClassNames;
	TSet<FName> m_RegisteredPropertyTypes;

	void _RegisterCustomClassLayout( FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate );
	void _RegisterCustomPropertyTypeLayout( FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate );

public :
	virtual void ShutdownModule() override;
};
