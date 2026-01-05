// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystem/StatusAttributeSet.h"

void UTestUserWidget::SetAbilitySystemComponent(UAbilitySystemComponent* ASC)
{
	if (!ASC) return;

	// 1. 초기 값 가져오기 (위젯이 처음 뜰 때 0으로 보이지 않게)
	// GetNumericAttribute는 값을 못 찾으면 0을 반환하지만, 안전을 위해 bFound 체크 가능
	bool bFound = false;
	CurrentHealth = ASC->GetGameplayAttributeValue(UStatusAttributeSet::GetHealthAttribute(), bFound);
	CurrentMaxHealth = ASC->GetGameplayAttributeValue(UStatusAttributeSet::GetMaxHealthAttribute(), bFound);
	CurrentMana = ASC->GetGameplayAttributeValue(UStatusAttributeSet::GetManaAttribute(), bFound);
	CurrentMaxMana = ASC->GetGameplayAttributeValue(UStatusAttributeSet::GetMaxManaAttribute(), bFound);

	// 2. 델리게이트 바인딩 (값이 변할 때 호출될 함수 등록)
	ASC->GetGameplayAttributeValueChangeDelegate(UStatusAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UStatusAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnMaxHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UStatusAttributeSet::GetManaAttribute()).AddUObject(this, &ThisClass::OnManaChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UStatusAttributeSet::GetMaxManaAttribute()).AddUObject(this, &ThisClass::OnMaxManaChanged);

	// 3. UI 최초 업데이트
	UpdateHealthUI();
	UpdateManaUI();
}

void UTestUserWidget::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	CurrentHealth = Data.NewValue;
	UpdateHealthUI();
}

void UTestUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	CurrentMaxHealth = Data.NewValue;
	UpdateHealthUI();
}

void UTestUserWidget::OnManaChanged(const FOnAttributeChangeData& Data)
{
	CurrentMana = Data.NewValue;
	UpdateManaUI();
}

void UTestUserWidget::OnMaxManaChanged(const FOnAttributeChangeData& Data)
{
	CurrentMaxMana = Data.NewValue;
	UpdateManaUI();
}

void UTestUserWidget::UpdateHealthUI()
{
	if (HealthBar && HealthText)
	{
		// 0으로 나누기 방지 및 퍼센트 계산
		const float Percent = (CurrentMaxHealth > 0.0f) ? (CurrentHealth / CurrentMaxHealth) : 0.0f;
		HealthBar->SetPercent(Percent);

		// 텍스트 포맷 예: "50 / 100"
		FString TextStr = FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, CurrentMaxHealth);
		HealthText->SetText(FText::FromString(TextStr));
	}
}

void UTestUserWidget::UpdateManaUI()
{
	if (ManaBar && ManaText)
	{
		const float Percent = (CurrentMaxMana > 0.0f) ? (CurrentMana / CurrentMaxMana) : 0.0f;
		ManaBar->SetPercent(Percent);

		FString TextStr = FString::Printf(TEXT("%.0f / %.0f"), CurrentMana, CurrentMaxMana);
		ManaText->SetText(FText::FromString(TextStr));
	}
}
