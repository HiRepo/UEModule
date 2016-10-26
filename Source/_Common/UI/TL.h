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
				return Cast<T>( pWidgetTree->FindWidget( name ) );
			return nullptr;
		}

		template <class TFilter=NullFilter>
		static T* Get( UUserWidget* pRootWidget, TFilter filter=NullFilter() )
		{
			type_assert( T, UWidget ); 
			UWidgetTree* pWidgetTree = pRootWidget->WidgetTree;
			if( nullptr == pWidgetTree )
				return nullptr;
			
			T* pFindWidget = nullptr;
			pWidgetTree->ForEachWidget( [&]( UWidget* pWidget )
				{ 
					T* 	pCastWidget = Cast<T>( pWidget );
					if( pCastWidget && filter( pCastWidget ) )
						pFindWidget = pCastWidget;
				});
			return pFindWidget;
		}


		template <class TFilter=NullFilter>
		static void GetAll( UUserWidget* pRootWidget, TArray<T*>& rOutArray, TFilter filter=NullFilter() )
		{
			type_assert( T, UWidget ); 
			UWidgetTree* pWidgetTree = pRootWidget->WidgetTree;
			if( nullptr == pWidgetTree )
				return ;
			
			pWidgetTree->ForEachWidget( [&]( UWidget* pWidget )
				{ 
					T* pCastWidget = Cast<T>( pWidget );
					if( pCastWidget && filter( pCastWidget ) )
						rOutArray.Add( pCastWidget );
				});

		}
	};
}

