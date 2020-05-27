// Fill out your copyright notice in the Description page of Project Settings.


#include "SECharacterBase.h"
#include "Abilities/SEAbilitySystemComponent.h"
#include "Abilities/SEGameplayAbility.h"
#include "Abilities/SEAttributeSet.h"
#include "GameplayTagContainer.h"


// Sets default values
ASECharacterBase::ASECharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterLevel = 1;
	
	AbilitySystemComponent = CreateDefaultSubobject<USEAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<USEAttributeSet>(TEXT("AttributeSet"));

	bAbilitiesInitialized = false;
}

void ASECharacterBase::PossessedBy(AController* NewController) {
	if (AbilitySystemComponent) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGameplayAbilities();
	}
}

void ASECharacterBase::UnPossessed() {}

// Called every frame
void ASECharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASECharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ASECharacterBase::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

int32 ASECharacterBase::GetCharacterLevel() const {
	return CharacterLevel;
}

bool ASECharacterBase::SetCharacterLevel(int32 NewLevel) {
	if (CharacterLevel != NewLevel && NewLevel > 0)
	{
		// Our level changed so we need to refresh abilities
		RemoveStartupGameplayAbilities();
		CharacterLevel = NewLevel;
		AddStartupGameplayAbilities();

		return true;
	}
	return false;
}

float ASECharacterBase::GetHealth() const {
	return AttributeSet ? AttributeSet->GetHealth() : 0;
}

float ASECharacterBase::GetMaxHealth() const {
	return AttributeSet ? AttributeSet->GetMaxHealth() : 0;
}

float ASECharacterBase::GetStamina() const {
	return AttributeSet ? AttributeSet->GetStamina() : 0;
}

float ASECharacterBase::GetMaxStamina() const {
	return AttributeSet ? AttributeSet->GetMaxStamina() : 0;
}

float ASECharacterBase::GetMoveSpeed() const {
	return AttributeSet ? AttributeSet->GetMoveSpeed() : 0;
}

float ASECharacterBase::GetSouls() const {
	return AttributeSet ? AttributeSet->GetSouls() : 0;
}

bool ASECharacterBase::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}

	return false;
}

void ASECharacterBase::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<USEGameplayAbility*>& ActiveAbilities)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	}
}

bool ASECharacterBase::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration)
{
	if (AbilitySystemComponent && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}
	return false;
}

bool ASECharacterBase::HasTag(FGameplayTag Tag) {
	if (AbilitySystemComponent) {
		return AbilitySystemComponent->GetTagCount(Tag) > 0;		
	}
	return false;
}

void ASECharacterBase::AddAbility(TSubclassOf<USEGameplayAbility> Ability) {
	if (AbilitySystemComponent) {
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, GetCharacterLevel(), INDEX_NONE, this));
	}
}

// Called when the game starts or when spawned
void ASECharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASECharacterBase::AddStartupGameplayAbilities() {
	check(AbilitySystemComponent);
	
	if (GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
		// Grant abilities, but only on the server	
		for (TSubclassOf<USEGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetCharacterLevel(), INDEX_NONE, this));
		}

		// Now apply passives
		for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}

		// AddSlottedGameplayAbilities();

		bAbilitiesInitialized = true;
	}
}
void ASECharacterBase::RemoveStartupGameplayAbilities() {
	check(AbilitySystemComponent);

	if (GetLocalRole() == ROLE_Authority && bAbilitiesInitialized)
	{
		// Remove any abilities added from a previous call
		TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
		for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
		{
			if ((Spec.SourceObject == this) && GameplayAbilities.Contains(Spec.Ability->GetClass()))
			{
				AbilitiesToRemove.Add(Spec.Handle);
			}
		}

		// Do in two passes so the removal happens after we have the full list
		for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
		{
			AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
		}

		// Remove all of the passive gameplay effects that were applied by this character
		FGameplayEffectQuery Query;
		Query.EffectSource = this;
		AbilitySystemComponent->RemoveActiveEffects(Query);

		// RemoveSlottedGameplayAbilities(true);

		bAbilitiesInitialized = false;
	}
}

void ASECharacterBase::HandleDamage(float DamageAmount, const FHitResult& HitInfo,
	const FGameplayTagContainer& DamageTags, ASECharacterBase* InstigatorCharacter, AActor* DamageCauser) {
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ASECharacterBase::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& SourceTags) {
	if (bAbilitiesInitialized) {
		OnHealthChanged(DeltaValue, SourceTags);
	}
}

void ASECharacterBase::HandleSoulsChanged(float DeltaValue, const FGameplayTagContainer& SourceTags) {
	if (bAbilitiesInitialized) {
		OnSoulsChanged(DeltaValue, SourceTags);
	}
}

void ASECharacterBase::HandleStanimaChanged(float DeltaValue, const FGameplayTagContainer& SourceTags) {
	if (bAbilitiesInitialized) {
		OnStaminaChanged(DeltaValue, SourceTags);
	}
}
