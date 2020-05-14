// Fill out your copyright notice in the Description page of Project Settings.


#include "SEAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "../SECharacterBase.h"

void USEAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
    Super::PostGameplayEffectExecute(Data);

    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

    // Compute the delta between old and new, if it is available
    float DeltaValue = 0;
    if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
    {
        // If this was additive, store the raw delta value to be passed along later
        DeltaValue = Data.EvaluatedData.Magnitude;
    }
    
    AActor* TargetActor = nullptr;
    AController* TargetController = nullptr;
    ASECharacterBase* TargetCharacter = nullptr;
    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        TargetCharacter = Cast<ASECharacterBase>(TargetActor);
    }

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
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
