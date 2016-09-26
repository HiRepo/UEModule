#pragma once
#include "_Common.h"

namespace TL{

	template<class T>
	struct Widget
	{
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
					if( pFindWidget && filter() )
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
					if( pFindWidget && filter() )
						rOutArray.Add( pFindWidget );
				});

		}
	};
}

