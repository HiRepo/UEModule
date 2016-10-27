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
					if( pFindWidget ) return;
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


		template <class TFilter=NullFilter>
		static T* GetRecursive( UUserWidget* pRootWidget, TFilter filter=NullFilter() )
		{
			type_assert( T, UWidget ); 

			TArray<UUserWidget*> arrUserWidget;
			arrUserWidget.Add( pRootWidget );
			while( arrUserWidget.Num() )
			{
				TArray<UUserWidget*>  arrSubUserWidget;
				for( UUserWidget* pUserWidget : arrUserWidget )
				{
					UWidgetTree* pWidgetTree = pUserWidget->WidgetTree;
					T* pFindWidget = nullptr;
					pWidgetTree->ForEachWidget( [&]( UWidget* pWidget )
						{ 
							if( pFindWidget ) return;
							T* 	pCastWidget = Cast<T>( pWidget );
							if( pCastWidget && filter( pCastWidget ) )
							{
								pFindWidget = pCastWidget;
								return ;
							}
							if( UUserWidget* pSubWidget = Cast<UUserWidget>( pWidget ) )
								arrSubUserWidget.Add( pSubWidget );
						});
					if( pFindWidget ) 
						return pFindWidget;
				}
				arrUserWidget =	arrSubUserWidget;
			}
			return nullptr;
		}


		template <class TFilter=NullFilter>
		static void GetAllRecursive( UUserWidget* pRootWidget, TArray<T*>& rOutArray, TFilter filter=NullFilter() )
		{
			type_assert( T, UWidget ); 

			TArray<UUserWidget*> arrUserWidget;
			arrUserWidget.Add( pRootWidget );
			while( arrUserWidget.Num() )
			{
				TArray<UUserWidget*> arrSubUserWidget;
				for( UUserWidget* pUserWidget : arrUserWidget )
				{
					UWidgetTree* pWidgetTree = pUserWidget->WidgetTree;
					pWidgetTree->ForEachWidget( [&]( UWidget* pWidget )
						{
							T* pCastWidget = Cast<T>( pWidget );
							if( pCastWidget && filter( pCastWidget ) )
								rOutArray.Add( pCastWidget );
							if( UUserWidget* pSubWidget = Cast<UUserWidget>( pWidget ) )
								arrSubUserWidget.Add( pSubWidget );
						});
				}
				arrUserWidget =	arrSubUserWidget;
			}
		}

		static T* GetRecursiveByName( UUserWidget* pRootWidget, FName name )
		{
			return GetRecurcive( pRootWidget, [=]( T* pWidget )
				{
					return pWidget->GetFName() == name;
				});
		}

	};
}

