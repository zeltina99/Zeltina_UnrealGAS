// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/Ability/GA_ChargingJump.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

UGA_ChargingJump::UGA_ChargingJump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ChargingJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);	// 쿨타임이나 코스트 문제
		return;
	}

	CurrentChargingTime = 0.0f;	// 차징 시간 초기화
	GetWorld()->GetTimerManager().SetTimer(
		ChargingTimerHandle,
		this,
		&UGA_ChargingJump::UpdateChargingTime,
		0.01f,
		true
	);	// 타이머를 이용해 차징 시간 업데이트 처리

	// 차징시 적용될 이펙트가 있으면 적용
	if (ChargingEffectClass && ActorInfo->AbilitySystemComponent.IsValid())
	{
		FGameplayEffectContextHandle Context = ActorInfo->AbilitySystemComponent->MakeEffectContext();
		Context.AddSourceObject(this);
		FGameplayEffectSpecHandle Spec = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
			ChargingEffectClass,
			GetAbilityLevel(),
			Context);

		if (Spec.IsValid())
		{
			ChargingEffectHandle = ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}
	UE_LOG(LogTemp, Log, TEXT("차징 시작"));
}

void UGA_ChargingJump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ExecuteSuperJump(ActorInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_ChargingJump::UpdateChargingTime()
{
	CurrentChargingTime += 0.01f;
	if (CurrentChargingTime >= MaxChargingTime)
	{
		CurrentChargingTime = MaxChargingTime;
		GetWorld()->GetTimerManager().ClearTimer(ChargingTimerHandle);
		UE_LOG(LogTemp, Log, TEXT("UGA_ChargingJump : 차징 최대치 도달"));
	}
}

void UGA_ChargingJump::ExecuteSuperJump(const FGameplayAbilityActorInfo* ActorInfo)
{
	if (ActorInfo && ActorInfo->AvatarActor.IsValid())	// ActorInfo와 AvatarActor확인
	{
		if (ChargingEffectHandle.IsValid() && ActorInfo->AbilitySystemComponent.IsValid())	// 이펙트가 실행중이면
		{
			ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ChargingEffectHandle);	// 이펙트 정지
			ChargingEffectHandle.Invalidate();	// 핸들 무효화
		}

		ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
		if (Character)
		{
			float ChargeRatio = FMath::Clamp(CurrentChargingTime / MaxChargingTime, 0.0f, 1.0f);	// 차지된 %
			float JumpPower = FMath::Lerp(MinJumpPower, MaxJumpPower, ChargeRatio);	// 보간으로 중간값 구하기

			FVector Direction(1, 0, 1);
			Direction.Normalize();	// 앞쪽 위 45도 방향
			Direction = Character->GetActorRotation().RotateVector(Direction);
			Character->LaunchCharacter(Direction * JumpPower, false, false); // 캐릭터 날리기
		}
	}
}
