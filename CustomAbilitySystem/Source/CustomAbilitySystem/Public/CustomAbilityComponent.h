#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomAbility.h"
#include "CustomModifier.h"
#include "CustomAbilityExecutionContext.h"
#include "CustomAbilityComponent.generated.h"

UCLASS(ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class CUSTOMABILITYSYSTEM_API UCustomAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCustomAbilityComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	TArray<UCustomAbility*> Abilities;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="Abilities")
	TArray<UCustomAbility*> ActiveAbilities;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="Modifiers")
	TArray<FCustomModifier> PersistentModifiers;
	
	UPROPERTY(Transient, BlueprintReadWrite, Category="Tags")
	TSet<FName> OwnerTags;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="Cooldown")
	TMap<UCustomAbility*, float> Cooldowns;

protected:
	// active context during execution
	TMap<UCustomAbility*, FCustomAbilityExecutionContext> ExecutionContexts;

public:

	UFUNCTION(BlueprintCallable, Category="Abilities")
	bool TryActivateAbility(UCustomAbility* Ability);
	
	UFUNCTION(BlueprintCallable, Category="Abilities")
	void CancelAbility(UCustomAbility* Ability);
	
	UFUNCTION(BlueprintCallable, Category="Abilities")
	bool IsAbilityAvailable(UCustomAbility* Ability) const;
	
	FCustomAbilityExecutionContext* GetExecutionContext(UCustomAbility* Ability);
};