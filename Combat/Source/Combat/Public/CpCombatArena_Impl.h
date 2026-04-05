#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CpCombatSubsystem.h"

#include "CpCombatArena_Impl.generated.h"

UCLASS()
class COMBAT_API ACpCombatArena : public AActor
{
    GENERATED_BODY()

public:
    ACpCombatArena();

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

    UCpCombatSubsystem* CombatSubsystem = nullptr;

protected:

    virtual void BeginPlay() override;
    virtual void Reset() override;

    void SpawnActors();
    void BindToCombatSubsystem();
    void UnbindFromCombatSubsystem();

    UFUNCTION()
    void OnCombatStateChanged(ECpCombatState NewState);

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