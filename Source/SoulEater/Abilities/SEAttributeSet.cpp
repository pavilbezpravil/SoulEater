// Fill out your copyright notice in the Description page of Project Settings.


#include "SEAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "../SECharacterBase.h"

void USEAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
    Super::PostGameplayEffectExecute(Data);

    FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

    // Compute the delta between old and new, if it is available
    float DeltaValue = 0;
    if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
    {
        // If this was additive, store the raw delta value to be passed along later
        DeltaValue = Data.EvaluatedData.Magnitude;
    }

    // Get the Target actor, which should be our owner
    AActor* TargetActor = nullptr;
    AController* TargetController = nullptr;
    ASECharacterBase* TargetCharacter = nullptr;
    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        TargetCharacter = Cast<ASECharacterBase>(TargetActor);
    }

    if (Data.EvaluatedData.Attribute == GetDamageAttribute())
    {
        // Get the Source actor
        AActor* SourceActor = nullptr;
        AController* SourceController = nullptr;
        ASECharacterBase* SourceCharacter = nullptr;
        if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
        {
            SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
            SourceController = Source->AbilityActorInfo->PlayerController.Get();
            if (SourceController == nullptr && SourceActor != nullptr)
            {
                if (APawn* Pawn = Cast<APawn>(SourceActor))
                {
                    SourceController = Pawn->GetController();
                }
            }

            // Use the controller to find the source pawn
            if (SourceController)
            {
                SourceCharacter = Cast<ASECharacterBase>(SourceController->GetPawn());
            }
            else
            {
                SourceCharacter = Cast<ASECharacterBase>(SourceActor);
            }

            // Set the causer actor based on context if it's set
            if (Context.GetEffectCauser())
            {
                SourceActor = Context.GetEffectCauser();
            }
        }

        // Try to extract a hit result
        FHitResult HitResult;
        if (Context.GetHitResult())
        {
            HitResult = *Context.GetHitResult();
        }

        // Store a local copy of the amount of damage done and clear the damage attribute
        const float LocalDamageDone = GetDamage();
        SetDamage(0.f);

        if (LocalDamageDone > 0)
        {
            // Apply the health change and then clamp it
            const float OldHealth = GetHealth();
            SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

            if (TargetCharacter)
            {
                // This is proper damage
                TargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);

                // Call for all health changes
                TargetCharacter->HandleHealthChanged(-LocalDamageDone, SourceTags);
            }
        }
    } else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        // Handle other health changes such as from healing or direct modifiers
        // First clamp it
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

        if (TargetCharacter)
        {
            // Call for all health changes
            TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
        }
    }
    else if (Data.EvaluatedData.Attribute == GetSoulsAttribute())
    {
        if (TargetCharacter)
        {
            TargetCharacter->HandleSoulsChanged(DeltaValue, SourceTags);
        }
    }
    else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
    {
        SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
        if (TargetCharacter)
        {
            TargetCharacter->HandleStanimaChanged(DeltaValue, SourceTags);
        }
    }
}
