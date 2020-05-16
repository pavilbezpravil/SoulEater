// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "SETargetType.generated.h"

class ASECharacterBase;
/**
 * 
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class SOULEATER_API USETargetType : public UObject
{
	GENERATED_BODY()

	public:
    // Constructor and overrides
    USETargetType() {}

	/** Called to determine targets to apply gameplay effects to */
	UFUNCTION(BlueprintNativeEvent)
    void GetTargets(ASECharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class SOULEATER_API USETargetType_UseOwner : public USETargetType
{
	GENERATED_BODY()

public:
    // Constructor and overrides
    USETargetType_UseOwner() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ASECharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class SOULEATER_API USETargetType_UseEventData : public USETargetType
{
	GENERATED_BODY()

public:
    // Constructor and overrides
    USETargetType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ASECharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

