#pragma once
#include "Json.h" 
#include "JsonBP.generated.h"


UCLASS(Blueprintable, MinimalAPI, ClassGroup=(Common,Util))
class UJsonValue : public UObject
{
	GENERATED_BODY()

private :
	TSharedPtr<FJsonValue> m_jValue;

public :
	UJsonValue() {}

	void InitFromJson( TSharedPtr<FJsonValue> jValue ) { m_jValue = jValue; }

	virtual bool TryGetObject(const TSharedPtr<FJsonObject>*& Object) const { return false; }

};


UCLASS(Blueprintable, MinimalAPI, ClassGroup=(Common,Util))
class UJsonObject : public UObject
{
	GENERATED_BODY()
	typedef TJsonWriterFactory< TCHAR, TPrettyJsonPrintPolicy<TCHAR> > FPrettyJsonStringWriterFactory;
	typedef TJsonWriter< TCHAR, TPrettyJsonPrintPolicy<TCHAR> > FPrettyJsonStringWriter;

private :
	TSharedPtr<FJsonObject> m_jObject;

public :
	UJsonObject() {}

	TSharedPtr<FJsonObject> GetJObject(){ return m_jObject; }

	void InitFromJson( TSharedPtr<FJsonObject> jObject ) { m_jObject = jObject; }

	UFUNCTION(BlueprintCallable, Category="Util|Json")
	void InitFormString( FString sContent )
	{
		m_jObject = MakeShareable(new FJsonObject());

		TSharedRef<TJsonReader<TCHAR>> jReader = TJsonReaderFactory<TCHAR>::Create( sContent );

		FJsonSerializer::Deserialize( jReader, m_jObject );
	}

	UFUNCTION(BlueprintPure, Category="Util|Json")
	FString ToString()
	{
		FString sOut;
		TSharedRef<TJsonWriter<TCHAR>> jWriter = TJsonWriterFactory<>::Create( &sOut );
		FJsonSerializer::Serialize( m_jObject.ToSharedRef(), jWriter );
		return sOut;
	}

	UFUNCTION(BlueprintPure, Category="Util|Json")
	FString ToStringPretty()
	{
		FString sOut;
		TSharedRef<FPrettyJsonStringWriter> jWriter = FPrettyJsonStringWriterFactory::Create( &sOut );
		FJsonSerializer::Serialize( m_jObject.ToSharedRef(), jWriter );
		return sOut;
	}

	UFUNCTION(BlueprintCallable, Category="Util|Json")
	bool GetBool( const FString& sField, bool& outBool )
	{
		return m_jObject->TryGetBoolField( sField, outBool );
	}

	UFUNCTION(BlueprintCallable, Category="Util|Json")
	bool GetInt( const FString& sField, int32& outNumber )
	{
		return m_jObject->TryGetNumberField( sField, outNumber );
	}

	UFUNCTION(BlueprintCallable, Category="Util|Json")
	bool GetString( const FString& sField, FString& outString )
	{
		return m_jObject->TryGetStringField( sField, outString );
	}

	UFUNCTION(BlueprintCallable, Category="Util|Json")
	bool GetStringArray( const FString& sField, TArray<FString>& outArray )
	{
		return m_jObject->TryGetStringArrayField( sField, outArray );
	}

	UFUNCTION(BlueprintCallable, Category="Util|Json")
	bool GetFloat( const FString& sField, float& outNumber )
	{
		double outDouble;
		bool bResult = m_jObject->TryGetNumberField( sField, outDouble );
		outNumber = outDouble;
		return bResult;
	}

	UFUNCTION(BlueprintCallable, Category="Util|Json")
	bool GetObject( const FString& sField, UJsonObject*& outObject )
	{
		const TSharedPtr<FJsonObject>* pJsonObject = nullptr;
		if( false == m_jObject->TryGetObjectField( sField, pJsonObject ) )
			return false;
		
		outObject = NewObject<UJsonObject>();
		outObject->InitFromJson( *pJsonObject );
		return true;
	}

	UFUNCTION(BlueprintCallable, Category="Util|Json")
	bool GetObjectArray( const FString& sField, TArray<UJsonObject*>& outArray )
	{
		const TArray<TSharedPtr<FJsonValue>>* pFieldArray = nullptr;
		if( false == m_jObject->TryGetArrayField( sField, pFieldArray ) )
			return false;

		for( const TSharedPtr<FJsonValue>& _jField : *pFieldArray )
		{
			const TSharedPtr<FJsonObject>* pJObject = nullptr;
			if( false == _jField->TryGetObject( pJObject ) )
				continue;
			UJsonObject* pJSonObject = NewObject<UJsonObject>();
			pJSonObject->InitFromJson( *pJObject );
			outArray.Add( pJSonObject );
		}
 		return true;
	}

	UFUNCTION(BlueprintCallable, Category="Util|Json")
	bool GetField( const FString& sField, UJsonValue*& outField )
	{
		TSharedPtr<FJsonValue> jField = m_jObject->TryGetField( sField );
		if( false == jField.IsValid() )
			return false;
		outField = NewObject<UJsonValue>();
		outField->InitFromJson( jField );
 		return true;
	}

	UFUNCTION(BlueprintCallable, Category="Util|Json")
	bool GetFieldArray( const FString& sField, TArray<UJsonValue*>& outArray )
	{
		const TArray<TSharedPtr<FJsonValue>>* pFieldArray = nullptr;
		if( false == m_jObject->TryGetArrayField( sField, pFieldArray ) )
			return false;

		for( const TSharedPtr<FJsonValue>& _jField : *pFieldArray )
		{
			UJsonValue* pJsonValue = NewObject<UJsonValue>();
			pJsonValue->InitFromJson( _jField );
			outArray.Add( pJsonValue );
		}
 		return true;
	}

};
