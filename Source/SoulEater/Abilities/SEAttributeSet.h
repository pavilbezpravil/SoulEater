// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "SEAttributeSet.generated.h"


// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class SOULEATER_API USEAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USEAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "MaxHealth")
    FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USEAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Souls")
    FGameplayAttributeData Souls;
	ATTRIBUTE_ACCESSORS(USEAttributeSet, Souls)
};
