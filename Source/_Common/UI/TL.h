#pragma once
#include "_Common.h"

namespace TL{

	template<class T>
	struct Widget
	{
		static T* GetByName( UUserWidget* pWidget, FName name )
		{
			type_assert( T, UWidget ); 
			if( UWidgetTree* pWidgetTree = pWidget->WidgetTree )
				return pWidgetTree->FindWidget( name );
			return nullptr;
		}

		template <class TFilter=NullFilter>
		static T* Get( UUserWidget* pWidget, TFilter filter=NullFilter() )
		{
			type_assert( T, UWidget ); 
			UWidgetTree* pWidgetTree = pWidget->WidgetTree;
			if( nullptr == pWidgetTree )
				return nullptr;
			
			pWidgetTree->ForEachWidget( [&]( UWidget* pWidget )
				{ 
					T* pFindWidget = Cast<T>( pWidget );
					if( pFindWidget && filter( pFindWidget ) )
						return pFindWidget;
				});
			return nullptr;
		}


		template <class TFilter=NullFilter>
		static void GetAll( UUserWidget* pWidget, TArray<T*>& rOutArray, TFilter filter=NullFilter() )
		{
			type_assert( T, UWidget ); 
			UWidgetTree* pWidgetTree = pWidget->WidgetTree;
			if( nullptr == pWidgetTree )
				return ;
			
			pWidgetTree->ForEachWidget( [&]( UWidget* pWidget )
				{ 
					T* pFindWidget = Cast<T>( pWidget );
					if( pFindWidget && filter( pFindWidget ) )
						rOutArray.Add( pFindWidget );
				});

		}
	};
}

