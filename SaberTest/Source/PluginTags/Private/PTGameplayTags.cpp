#include "PTGameplayTags.h"

namespace PluginTags
{
	UE_DEFINE_GAMEPLAY_TAG(TAG_Combat_Damage,			"Combat.Damage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Combat_Crit,				"Combat.Crit");
	
	UE_DEFINE_GAMEPLAY_TAG(TAG_Combat_StateChanged,		"Combat.StateChanged");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Combat_State_None,		"Combat.State.None");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Combat_State_Init,		"Combat.State.Init");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Combat_State_Active,		"Combat.State.Active");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Combat_State_Finished,	"Combat.State.Finished");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Combat_State_Resulting,	"Combat.State.Resulting");
	
	UE_DEFINE_GAMEPLAY_TAG(TAG_Health_Changed,			"Health.Changed");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Health_Death,			"Health.Death");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Attack,			"Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Dash,			"Ability.Dash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_HitReaction,		"Ability.HitReaction");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Stat_DamageProduced,		"Stat.DamageProduced");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Stat_DamageRecieved,		"Stat.DamageReceived");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Stat_CritCount,			"Stat.CritCount");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Stat_ExperiencePoints,	"Stat.ExperiencePoints");
}