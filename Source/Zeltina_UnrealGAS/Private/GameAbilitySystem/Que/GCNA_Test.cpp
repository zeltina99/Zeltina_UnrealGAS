// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/Que/GCNA_Test.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AGCNA_Test::AGCNA_Test()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.TestActor"));
	bAutoDestroyOnRemove = true;	// 재사용시 파이클 재생이 끝나지 않는 문제 해결을 위해 필요
}

bool AGCNA_Test::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (SpawnedVFX.IsValid())	// 안전장치(혹시 만들어진게 있으면 제거하고 진행하라)
	{
		SpawnedVFX->Deactivate();
		SpawnedVFX = nullptr;
	}

	if (MyTarget)
	{

		//Parameters.EffectContext.GetHitResult : 히트 정보를 가져올 수 있다(=부딪친 위치나 노멀 벡터를 구할 수 있다.)
		// 중요 : GetHitResult는 값을 설정해 줬어야 쓸 수 있다.
		SpawnedVFX = UNiagaraFunctionLibrary::SpawnSystemAttached(	// 파티클 만들어서 저장해 놓기
			TestVFX,                        // 1. 스폰할 나이아가라 시스템 (변수)
			MyTarget->GetRootComponent(),   // 2. 어디에 붙일지 (컴포넌트)
			FName("NAME_None"),             // 3. 소켓 이름 (없으면 NAME_None)
			FVector(0, 0, -90.0f),          // 4. 위치 오프셋 (소켓 기준 얼마나 떨어질지)
			FRotator::ZeroRotator,          // 5. 회전 오프셋
			EAttachLocation::SnapToTarget,  // 6. 붙이는 규칙 (딱 달라붙기)
			true                            // 7. 효과가 끝나면 컴포넌트 자동 삭제 여부
		);

		//UNiagaraFunctionLibrary::SpawnSystemAttached()
		// 붙인채로 움직이고 싶을 때
		return true;
	}
	return false;
}

bool AGCNA_Test::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (SpawnedVFX.IsValid())	// 끝날 때 제거하기
	{
		SpawnedVFX->Deactivate();
		SpawnedVFX = nullptr;
	}
	return true;
}
