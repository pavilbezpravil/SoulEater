// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SEAbilityTypes.h"
#include "Abilities/GameplayAbility.h"
#include "SEGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class SOULEATER_API USEGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/** Map of gameplay tags to gameplay effect containers */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
	TMap<FGameplayTag, FSEGameplayEffectContainer> EffectContainerMap;

	/** Make gameplay effect container spec to be applied later, using the passed in container */
	UFUNCTION(BlueprintCallable, Category = Ability, meta=(AutoCreateRefTerm = "EventData"))
    virtual FSEGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FSEGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** Search for and make a gameplay effect container spec to be applied later, from the EffectContainerMap */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
    virtual FSEGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** Applies a gameplay effect container spec that was previously created */
	UFUNCTION(BlueprintCallable, Category = Ability)
    virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FSEGameplayEffectContainerSpec& ContainerSpec);

	/** Applies a gameplay effect container, by creating and then applying the spec */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
    virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);
};
