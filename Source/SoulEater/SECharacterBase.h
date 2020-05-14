// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "SECharacterBase.generated.h"

class USEGameplayAbility;
class USEAbilitySystemComponent;
class USEAttributeSet;

UCLASS()
class SOULEATER_API ASECharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASECharacterBase();

	void PossessedBy(AController* NewController) override;
	void UnPossessed() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	void AddAbility(TSubclassOf<USEGameplayAbility> Ability);

	UFUNCTION(BlueprintCallable)
	int32 GetCharacterLevel() const;
	
	UFUNCTION(BlueprintCallable)
	bool SetCharacterLevel(int32 NewLevel);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable)
	float GetSouls() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Abilities to grant to this character on creation. These will be activated by tag or event and are not bound to specific inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
    TArray<TSubclassOf<USEGameplayAbility>> GameplayAbilities;
	
	/** Passive gameplay effects applied on creation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
    TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;
	
	UPROPERTY(EditAnywhere, Category = Abilities)
    int32 CharacterLevel;

	UPROPERTY(BlueprintReadOnly)
	USEAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY()
	USEAttributeSet* AttributeSet;

	void AddStartupGameplayAbilities();
	void RemoveStartupGameplayAbilities();
	
	UPROPERTY()
    int32 bAbilitiesInitialized;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	
	void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& SourceTags);

	friend USEAttributeSet;
};
