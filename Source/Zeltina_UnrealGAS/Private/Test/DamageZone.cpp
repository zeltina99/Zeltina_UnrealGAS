// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/DamageZone.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
ADamageZone::ADamageZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADamageZone::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ADamageZone::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ADamageZone::OnEndOverlap);
}

void ADamageZone::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (ASC)
	{
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddInstigator(this, this);

		//FGameplayEffectContextHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, ContextHandle);

		/*if (SpecHandle.IsValid())
		{
			
		}*/
	}
}

void ADamageZone::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}
