#pragma once

#include "Engine/DataTable.h"

namespace TL
{
	template<class T>
	struct Loader
	{
		static T* Sync( const TCHAR* psName, uint32 flag=LOAD_None, UPackageMap* pSandbox=nullptr )
		{
			UObject* pOuter = nullptr;
			return Cast<T>( StaticLoadObject( T::StaticClass(), pOuter, psName, nullptr, flag, pSandbox ));
		}

		static UClass* Class( FString sName )
		{
			int index = INDEX_NONE; 
			sName.FindLastChar( TCHAR('\''), index );
			check( INDEX_NONE != index );
			sName.InsertAt( index, "_C" );
			UClass* pClass = Loader<UClass>::Sync( *sName );
			check( pClass->IsChildOf( T::StaticClass() ) ); 
			return pClass;
		}
	};

	// Example
	/*
		auto* s_tex = TL::Loader<UTexture2D>::Sync( TEXT("Texture2D'/Game/UI/Resource/Icon_Item/Ic_It_PVPPoint.Ic_It_PVPPoint'") );
		auto* s_particle = TL::Loader<UParticleSystem>::Sync( TEXT("ParticleSystem'/Game/Effect/Common/Particles/P_Decal_My_01.P_Decal_My_01'") );
	*/

	template<class T>
	struct FTable
	{
	private :
		FTable() = delete;
		FTable( const FTable& ) = delete;
		FTable& operator=( const FTable& ) = delete;

		FString  m_sFullPath;

	public :
		FTable( const TCHAR* psPath, const TCHAR* psFile )
		: m_sFullPath( FString::Printf( TEXT("DataTable'/Game%s/%s.%s'"), psPath, psFile, psFile ) )
		{}
	
		UDataTable& Get()
		{
			static UDataTable* s_pTable = nullptr;
			if( nullptr == s_pTable )
			{
				s_pTable = Loader<UDataTable>::Sync( *m_sFullPath );
				s_pTable->AddToRoot();
			}
			return *s_pTable;
		}

		T* GetData( const TCHAR* psKey )
		{
			return Get().template FindRow<T>( psKey, FString(psKey) );
		}
	};
}



