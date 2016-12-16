#include "_Common.h"
#include "BlueprintNodeHelpers.h"
#include "BTTask_BP.h"


static bool __CanUsePropertyType(const UProperty* pProperty)
{
	if (pProperty->IsA(UNumericProperty::StaticClass()) ||
		pProperty->IsA(UBoolProperty::StaticClass()) ||
		pProperty->IsA(UNameProperty::StaticClass()))
	{
		return true;
	}

	const UStructProperty* StructProp = Cast<const UStructProperty>(pProperty);
	if (StructProp)
	{
		FString CPPType = StructProp->GetCPPType(NULL, CPPF_None);
		if( CPPType.Contains("FVector") || CPPType.Contains("FRotator") )
		{
			return true;
		}
	}

	return false;
}


static void __CollectPropertyData( const UObject* pObject, const UClass* StopAtClass, TArray<UProperty*>& PropertyData )
{
	UE_LOG(LogBehaviorTree, Verbose, TEXT("Looking for runtime properties of class: %s"), *GetNameSafe(pObject->GetClass()));

	PropertyData.Reset();

	for( UProperty* pProperty = pObject->GetClass()->PropertyLink; pProperty; pProperty = pProperty->PropertyLinkNext )
	{
		// stop when reaching base class
		if (pProperty->GetOuter() == StopAtClass)
			break;
		//
		// skip properties without any setup data
		if (pProperty->HasAnyPropertyFlags(CPF_Transient) ||
			pProperty->HasAnyPropertyFlags(CPF_DisableEditOnInstance) == false)
		{
			continue;
		}
		//
		// serialize only simple types
		if( __CanUsePropertyType(pProperty) )
		{
			UE_LOG(LogBehaviorTree, Verbose, TEXT("> name: '%s'"), *GetNameSafe(pProperty));
			PropertyData.Add(pProperty);
		}
	}
}



static FString __CollectPropertyDescription( const UObject* pObject, const UClass* pStopAtClass, const TArray<UProperty*>& CDO_PropertyData )
{
	FString RetString;
	for( UProperty* pProperty = pObject->GetClass()->PropertyLink; pProperty; pProperty = pProperty->PropertyLinkNext )
	{
		// stop when reaching base class
		if( pProperty->GetOuter() == pStopAtClass )
			break;

		// skip properties without any setup data
		if( pProperty->HasAnyPropertyFlags(CPF_Transient) ||
			pProperty->HasAnyPropertyFlags(CPF_DisableEditOnInstance) ||
			CDO_PropertyData.Contains( pProperty ))
		{
			continue;
		}

		if( pProperty->IsA(UClassProperty::StaticClass()) ||
			pProperty->IsA(UStructProperty::StaticClass()) ||
			__CanUsePropertyType(pProperty))
		{
			if( RetString.Len() )
			{
				RetString.AppendChar(TEXT('\n'));
			}

			const uint8* PropData = pProperty->ContainerPtrToValuePtr<uint8>( pObject );
			RetString += BlueprintNodeHelpers::DescribeProperty(pProperty, PropData);
		}
	}

	return RetString;
}


void __DescribeRuntimeValues(const UObject* pObject, const TArray<UProperty*>& PropertyData, TArray<FString>& RuntimeValues )
{
	for (int32 PropertyIndex = 0; PropertyIndex < PropertyData.Num(); PropertyIndex++)
	{
		UProperty* pProperty = PropertyData[PropertyIndex];
		const uint8* _pPropAddr = pProperty->ContainerPtrToValuePtr<uint8>(pObject);
		RuntimeValues.Add( BlueprintNodeHelpers::DescribeProperty( pProperty, _pPropAddr ));
	}
}





struct __PropertyInfo
{
	TArray<UProperty*> PropertyData;
	__PropertyInfo( const UObject* pObject )
	{
	//	this Functions not export in AIModule
	//	BlueprintNodeHelpers::__CollectPropertyData( pObject, pClass, PropertyData ); 		//	this Functions not export in AIModule
		UClass* pClass = pObject->GetClass();
		__CollectPropertyData( pClass->GetDefaultObject(), pClass, PropertyData );
	}
};



FString UBTTask_BP::GetStaticDescription() const
{
	static __PropertyInfo s_CDO_Property( this ); 

	FString sReturn = Super::GetStaticDescription();

	if( IsShowPropertyDetails )
	{
//		this Functions not export in AIModule
//		FString sPropertyDesc = BlueprintNodeHelpers::CollectPropertyDescription( this, GetClass(), s_PropertyInfo.PropertyData );	
		FString sPropertyDesc = __CollectPropertyDescription( this, GetClass(), s_CDO_Property.PropertyData );
		if( sPropertyDesc.Len() )
		{
			sReturn += TEXT(":\n\n");
			sReturn += sPropertyDesc;
		}
	}

	return _NodeDescription( sReturn );
}


void UBTTask_BP::DescribeRuntimeValues(const UBehaviorTreeComponent& rBTComponent, uint8* pNodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	static __PropertyInfo s_CDO_Property( this ); 

	__DescribeRuntimeValues( this, s_CDO_Property.PropertyData, Values);
//	BlueprintNodeHelpers::DescribeRuntimeValues(this, CDO->PropertyData, Values);
}

