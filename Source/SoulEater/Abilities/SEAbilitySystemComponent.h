// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SEAbilitySystemComponent.generated.h"

class USEGameplayAbility;
/**
 * 
 */
UCLASS()
class SOULEATER_API USEAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/** Returns a list of currently active ability instances that match the tags */
    void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<USEGameplayAbility*>& ActiveAbilities);

	/** Returns the default level used for ability activations, derived from the character */
	int32 GetDefaultAbilityLevel() const;

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static USEAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);
};
