// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "TestUserWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class ZELTINA_UNREALGAS_API UTestUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetAbilitySystemComponent(UAbilitySystemComponent* ASC);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ManaBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ManaText;

protected:
	// --- 어트리뷰트 변경 콜백 함수들 ---

	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	void OnManaChanged(const FOnAttributeChangeData& Data);
	void OnMaxManaChanged(const FOnAttributeChangeData& Data);

	// UI 업데이트 헬퍼 함수
	void UpdateHealthUI();
	void UpdateManaUI();

private:
	// 현재 값 저장을 위한 캐싱 변수 (계산을 위해 필요)
	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 1.0f; // 0으로 나누기 방지
	float CurrentMana = 0.0f;
	float CurrentMaxMana = 1.0f;
};
