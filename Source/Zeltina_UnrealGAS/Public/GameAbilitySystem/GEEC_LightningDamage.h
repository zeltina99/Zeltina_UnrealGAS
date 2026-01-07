// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_LightningDamage.generated.h"

/**
 * 
 */
UCLASS()
class ZELTINA_UNREALGAS_API UGEEC_LightningDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGEEC_LightningDamage();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CustomData")
	float MinimumDamage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CustomData")
	TObjectPtr<UCurveTable> DataCurveTable = nullptr;

private:
	FGameplayTag Tag_DebuffShock;		// 추가 데미지를 받을 태그
	FGameplayTag Tag_ElementLightning;	// 이 데미지의 속성 태그
};
