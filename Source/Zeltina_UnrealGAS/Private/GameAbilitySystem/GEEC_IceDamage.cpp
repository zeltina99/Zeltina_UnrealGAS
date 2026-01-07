// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/GEEC_IceDamage.h"
#include "GameAbilitySystem/ResourceAttributeSet.h"

// UGEEC_FireDamage 계산에 필요한 어트리뷰트를 캡처하기 위한 구조체(이 계산이 어디에 영향을 줄것인가)
struct FIceDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);	// Health 어트리뷰트를 캡처할 것이라고 정의

	FIceDamageStatics()
	{
		// UResourceAttributeSet의 Health를 캡처하는데, Target으로 부터 캡처, 스냅샷은 사용하지 않음
		DEFINE_ATTRIBUTE_CAPTUREDEF(UResourceAttributeSet, Health, Target, false);
	}
};

// FIceDamageStatics을 싱글톤 인스턴스를 반환하는 함수
static FIceDamageStatics& IceDamageStatics()
{
	static FIceDamageStatics Statics;
	return Statics;
}

UGEEC_IceDamage::UGEEC_IceDamage()
{
	RelevantAttributesToCapture.Add(IceDamageStatics().HealthDef);	// 캡처할 어트리뷰트 목록에 추가

	Tag_DebuffFreez = FGameplayTag::RequestGameplayTag(FName("State.Debuff.Freez"));
	Tag_ElementIce = FGameplayTag::RequestGameplayTag(FName("Element.Ice"));
}

void UGEEC_IceDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 이펙트를 주고 받는 ASC를 찾아 놓기
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (TargetASC && SourceASC)
	{
		const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

		const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();	// 소스의 태그 가져오기
		const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();	// 타겟의 태그 가져오기

		float Damage = MinimumDamage;

		// 커브테이블에서 값 가져오기
		if (DamageTable)
		{
			float EffectLevel = Spec.GetLevel();	// 레벨 가져오기
			FRealCurve* DamageCurve = DamageTable->FindCurve(FName("Damage"), TEXT("UGEEC_IceDamage"));	// 커브 테이블에서 커브가져오기
			if (DamageCurve)
			{
				Damage = DamageCurve->Eval(EffectLevel);	// 커브에서 레벨에 해당하는 값 가져오기
			}
		}

		if (SourceTags && SourceTags->HasTag(Tag_ElementIce)		// 공격 데미지가 불속성이고
			&& TargetTags && TargetTags->HasTag(Tag_DebuffFreez))	// 피격자가 화상 디버프를 가지고 있으면
		{
			Damage *= 2.0f;	// 그러면 데미지 두배
		}

		if (Damage > 0.0f)
		{
			OutExecutionOutput.AddOutputModifier(
				FGameplayModifierEvaluatedData(
					IceDamageStatics().HealthProperty,	// Health 어트리뷰트를 변경한다.
					EGameplayModOp::Additive,			// 더해 줄거다.
					-Damage								// -Damage만큼 처리
				)
			);
		}
	}
}
