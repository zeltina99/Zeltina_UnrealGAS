// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttMacro.h"
#include "AbilitySystemComponent.h"
#include "StatAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class ZELTINA_UNREALGAS_API UStatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UStatAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UStatAttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UStatAttributeSet, Dexterity)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UStatAttributeSet, Intelligence)
};
