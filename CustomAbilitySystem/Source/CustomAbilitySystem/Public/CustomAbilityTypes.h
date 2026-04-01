#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECustomAbilityState : uint8
{
	CaState_Idle UMETA(DisplayName = "Idle"),
	CaState_Casting UMETA(DisplayName = "Casting"),
	CaState_Targeting UMETA(DisplayName = "Targeting"),
	CaState_Executing UMETA(DisplayName = "Executing"),
	CaState_Cooldown UMETA(DisplayName = "Cooldown"),
	CaState_Interrupted UMETA(DisplayName = "Interrupted")
};

UENUM(BlueprintType)
enum class ECustomModifierOp : uint8
{
	CmOp_Add UMETA(DisplayName = "Add"),
	CmOp_Multiply UMETA(DisplayName = "Multiply")
};

UENUM(BlueprintType)
enum class ECustomAbilityFailReason : uint8
{
	CaFail_None UMETA(DisplayName = "None"),
	CaFail_NotEnoughResource UMETA(DisplayName = "Not Enough Resource"),
	CaFail_BlockedByTags UMETA(DisplayName = "Blocked By Tags"),
	CaFail_OnCooldown UMETA(DisplayName = "On Cooldown"),
	CaFail_InvalidTarget UMETA(DisplayName = "Invalid Target"),
	CaFail_Busy UMETA(DisplayName = "Busy")
};