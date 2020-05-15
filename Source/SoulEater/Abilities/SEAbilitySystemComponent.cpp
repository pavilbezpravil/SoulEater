// Fill out your copyright notice in the Description page of Project Settings.


#include "SEAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "SEGameplayAbility.h"
#include "SoulEater/SECharacterBase.h"


void USEAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<USEGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<USEGameplayAbility>(ActiveAbility));
		}
	}
}

int32 USEAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	ASECharacterBase* OwningCharacter = Cast<ASECharacterBase>(OwnerActor);

	if (OwningCharacter)
	{
		return OwningCharacter->GetCharacterLevel();
	}
	return 1;
}

USEAbilitySystemComponent* USEAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	return Cast<USEAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}