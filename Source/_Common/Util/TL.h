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


//		template <class T>


//	FStreamableManager

//		m_CharacterBaseTable = ConstructTableLoader("Character_Base_TB");
//		m_MonsterTable = ConstructTableLoader("Monster_TB");
//	    m_SkillTable = ConstructTableLoader("Skill_TB");
//	    m_SkillNttTable = ConstructTableLoader("Skill_Ntt_TB");
//		m_UITextTable = ConstructTableLoader("UI_Text_TB");
//		m_InfoUITable = ConstructTableLoader("Info_UI_TB");
//	    m_BuffNttTable = ConstructTableLoader("Skill_Buff_TB");
//	    m_CCTable = ConstructTableLoader("Skill_Cc_TB");


//	UDataTable* UTableManager::ConstructTableLoader(FString path)
//	{
//		FString tmpPath = "DataTable'/Game/DataTables/" + path + "." + path + "'";
//		ConstructorHelpers::FObjectFinder<UDataTable>
//			GameObjectLookupDataTable_Character(*tmpPath);
//		return GameObjectLookupDataTable_Character.Object;
//	}

}

