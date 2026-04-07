#pragma once

#include "NativeGameplayTags.h"

/**
 * Centralized namespace containing all gameplay tags used throughout the plugin.
 * Provides a single location for managing tag definitions and ensures consistency across systems.
 * 
 * Tag Categories:
 * - Combat: Damage, critical hits, and state management
 * - Health: Health changes and death events
 * - Ability: Ability identification and activation
 * - Stats: Player statistics tracking
 */

namespace PluginTags
{
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Damage);
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Crit);
	
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_StateChanged);
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_State_None);
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_State_Init);
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_State_Active);
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_State_Finished);
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_State_Resulting);
	
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Health_Changed);
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Health_Death);

	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack);
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Dash);
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_HitReaction);

	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Stat_DamageProduced);
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Stat_DamageRecieved);
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Stat_CritCount);
	PLUGINTAGSMODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Stat_ExperiencePoints);
}