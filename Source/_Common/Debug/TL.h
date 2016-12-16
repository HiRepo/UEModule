#pragma once

namespace TL{

	int GetReferenceCount( UObject* pObject, TArray<UObject*>* pRefObjects=nullptr )
	{
		if( nullptr == pObject || !pObject->IsValidLowLevelFast() )
			return -1;
	 
		TArray<UObject*> arrRefObject;				//req outer, ignore archetype, recursive, ignore transient
		FReferenceFinder ObjectReferenceCollector( arrRefObject, pObject, false, true, true, false );
		ObjectReferenceCollector.FindReferences( pObject );
	 
		if( pRefObjects )
			pRefObjects->Append(ReferredToObjects);
		return arrRefObject.Num();
	}

};

