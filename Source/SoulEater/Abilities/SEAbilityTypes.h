// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SETargetType.h"
#include "SEAbilityTypes.generated.h"

/**
* Struct defining a list of gameplay effects, a tag, and targeting info
* These containers are defined statically in blueprints or assets and then turn into Specs at runtime
*/
USTRUCT(BlueprintType)
struct FSEGameplayEffectContainer
{
	GENERATED_BODY()

    FSEGameplayEffectContainer() {}

	/** Sets the way that targeting happens */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    TSubclassOf<USETargetType> TargetType;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;
};

/** A "processed" version of RPGGameplayEffectContainer that can be passed around and eventually applied */
USTRUCT(BlueprintType)
struct FSEGameplayEffectContainerSpec
{
	GENERATED_BODY()

    FSEGameplayEffectContainerSpec() {}

	/** Computed target data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    FGameplayAbilityTargetDataHandle TargetData;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
    TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

	/** Returns true if this has any valid effect specs */
	bool HasValidEffects() const;

	/** Returns true if this has any valid targets */
	bool HasValidTargets() const;

	/** Adds new targets to target data */
	void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};
