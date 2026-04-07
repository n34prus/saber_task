#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CSCombatSubsystem.h"
#include "GICombatArena.generated.h"

/**
 * Combat arena actor that manages enemy spawning and combat initiation.
 * Triggers combat encounters when the player enters the arena area.
 * 
 * Key Features:
 * - Trigger-based combat initiation
 * - Dynamic enemy spawning based on difficulty
 * - AI controller assignment to spawned enemies
 * - Combat state monitoring
 * - Automatic ground placement for spawned enemies
 * - Combat subsystem integration
 * - Post-combat UI display
 */

UCLASS()
class GAMEPLAYIMPLEMENTATIONMODULE_API AGICombatArena : public AActor
{
    GENERATED_BODY()

public:
    AGICombatArena();

protected:
    
    UPROPERTY(EditAnywhere, Category="Arena")
    TSoftClassPtr<APawn> EnemyPawnClass;

    UPROPERTY(EditAnywhere, Category="Arena")
    TSoftClassPtr<AAIController> AiControllerClass;

    UPROPERTY(EditAnywhere, Category="Arena")
    TSoftClassPtr<UUserWidget> WidgetClass;
    
    //UPROPERTY(EditAnywhere, Category="Arena")
    //int32 SpawnCount;

    UPROPERTY(EditAnywhere, Category="Arena")
    float SpawnSpacing;
    
    UPROPERTY()
    UBoxComponent* Trigger;

    UPROPERTY()
    TArray<APawn*> SpawnedPawns;

    UCSCombatSubsystem* CombatSubsystem = nullptr;

protected:

    virtual void BeginPlay() override;
    virtual void Reset() override;

    void SpawnActors();
    void BindToBusEvents();
    void UnbindFromBusEvents();

    UFUNCTION()
    void HandleCombatStateChanged(const FEBEventData& Event);

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp,
                   AActor* OtherActor,
                   UPrimitiveComponent* OtherComp,
                   int32 OtherBodyIndex,
                   bool bFromSweep,
                   const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable)
    void OnTriggered(AActor* ByActor);
};