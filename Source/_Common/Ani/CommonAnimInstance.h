#pragma once
#include "Animation/AnimInstance.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Animation/AnimNode_AssetPlayerBase.h"
#include "CommonAnimInstance.generated.h"

UCLASS()
class UCommonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	float __GetElapsePercent( FAnimNode_StateMachine* pSM, int iMachineIndex )
	{
		int iStateIndex = pSM->GetCurrentState();
		if( 0 <= iStateIndex )
			return GetRelevantAnimTimeFraction( iMachineIndex, iStateIndex );
		return -1.f;
	}

	float __GetElapsedTime( FAnimNode_StateMachine* pSM, int iMachineIndex  )
	{
		int iStateIndex = pSM->GetCurrentState();
		if( 0 <= iStateIndex )
		{
			if( FAnimNode_AssetPlayerBase* pAssetPlayer = GetRelevantAssetPlayerFromState( iMachineIndex, iStateIndex ) )
			{
				if( pAssetPlayer->GetAnimAsset() )
						return pAssetPlayer->GetCurrentAssetTime();
//						return GetInstanceCurrentStateElapsedTime( iMachineIndex );
			}
		}
		return -1.f;
	}


public :
	int GetMachineIndex( FName MachineName )
	{
		return GetStateMachineIndex( MachineName );
	}


//		int GetActiveMachine( FAnimNode_StateMachine*& pOutSM )
//		{
//			static int s_Count = 0;
//			++s_Count;
//			
//			int index = INDEX_NONE;
//			int i = 0;
//			UE_LOG( _Common, Warning, TEXT("%d : AttackStateMachine is %d"), s_Count, GetStateMachineIndex("AttackStateMachine") );
//	
//			while( FAnimNode_StateMachine* pSM = GetStateMachineInstance(i) )
//			{
//				int iStateIndex = pSM->GetCurrentState();
//				if( iStateIndex < 0 )
//					return INDEX_NONE;
//				const float _fWeight = pSM->GetStateWeight( iStateIndex );
//				if( 0.f < _fWeight )
//				{
//					pOutSM = pSM;
//					index = i;
//					UE_LOG( _Common, Warning, TEXT("%d : index : %d,  Weight is %f"), s_Count, i, _fWeight );
//				}
//				++i;
//			}
//			return index;
//		}


	float GetElapsePercent( FName MachineName )
	{
		if( FAnimNode_StateMachine*	pSM = GetStateMachineInstanceFromName(MachineName) )
			return __GetElapsePercent( pSM, GetStateMachineIndex(MachineName) );
		return -1.f;
	}


	int GetAnimFrame( FName MachineName )
	{
		float fElapseTime = GetElapsedTime( MachineName );
		return fElapseTime < 0.f ? -1 : fElapseTime / 0.033f;
	}



	float GetElapsedTime( FName MachineName )
	{
		if( FAnimNode_StateMachine*	pSM = GetStateMachineInstanceFromName(MachineName) )
			return __GetElapsedTime( pSM, GetStateMachineIndex(MachineName) );
		return -1.f;
	}


	bool IsLastFrame( FName MachineName )
	{
		if( FAnimNode_StateMachine*	pSM = GetStateMachineInstanceFromName(MachineName) )
		{
			int iStateIndex   = pSM->GetCurrentState();
			int iMachineIndex = GetStateMachineIndex(MachineName);
			if( 0 <= iStateIndex )
			{
				if( FAnimNode_AssetPlayerBase* pAssetPlayer = GetRelevantAssetPlayerFromState( iMachineIndex, iStateIndex ) )
					return pAssetPlayer->GetCurrentAssetLength() < GetInstanceCurrentStateElapsedTime( iMachineIndex );
			}
		}
		return false;
	}

};

