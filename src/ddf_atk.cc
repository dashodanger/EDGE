//----------------------------------------------------------------------------
//  EDGE Data Definition File Code (Attack Types)
//----------------------------------------------------------------------------
// 
//  Copyright (c) 1999-2003  The EDGE Team.
// 
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//----------------------------------------------------------------------------
//
// Attacks Setup and Parser Code
//
// 1998/10/29 -KM- Finalisation of sound code.  SmartProjectile.
//

#include "i_defs.h"

#include "ddf_locl.h"
#include "ddf_main.h"
#include "dm_state.h"
#include "m_fixed.h"
#include "m_math.h"
#include "p_mobj.h"
#include "z_zone.h"

#undef  DF
#define DF  DDF_CMD

static attacktype_t buffer_atk;
static attacktype_t *dynamic_atk;

// this (and buffer_mobj) logically belongs with buffer_atk:
static bool attack_has_mobj;
static float a_damage_range;
static float a_damage_multi;

static const attacktype_t template_atk =
{
	DDF_BASE_NIL,  // ddf

	ATK_NONE, // attackstyle
	AF_None,  // flags
	NULL,     // initsound
	NULL,     // sound
	0,        // accuracy slope
	0,        // accuracy angle
	0,        // xoffset
	0,        // yoffset
	0,        // angle_offset
	0,        // slope_offset
	0,        // assault_speed
	0,        // height
	0,        // range
	0,        // count
	0,        // tooclose

	// damage info
	{
		0,      // nominal
		-1,     // linear_max
		-1,     // error
		0,      // delay
		NULL_LABEL, NULL_LABEL, NULL_LABEL,  // override labels
		false   // no_armour
	},

	BITSET_EMPTY, // attack_class
	0,        // objinitstate
	NULL,     // objinitstate_ref
	PERCENT_MAKE(0), // notracechance
	PERCENT_MAKE(0), // keepfirechance
	NULL,     // atk_mobj
	NULL,     // spawnedobj
	NULL,     // spawnedobj_ref
	NULL,     // puff
	NULL      // puff_ref
};

attacktype_t ** ddf_attacks = NULL;
int num_ddf_attacks = 0;
int num_disabled_attacks = 0;

static stack_array_t ddf_attacks_a;


static void DDF_AtkGetType(const char *info, void *storage);
static void DDF_AtkGetSpecial(const char *info, void *storage);
static void DDF_AtkGetLabel(const char *info, void *storage);

damage_t dummy_damage;

#undef  DDF_CMD_BASE
#define DDF_CMD_BASE  dummy_damage

const commandlist_t damage_commands[] =
{
  DF("VAL", nominal,    DDF_MainGetFloat),
  DF("MAX", linear_max, DDF_MainGetFloat),
  DF("ERROR", error, DDF_MainGetFloat),
  DF("DELAY", delay, DDF_MainGetTime),

  DF("PAIN STATE", pain, DDF_AtkGetLabel),
  DF("DEATH STATE", death, DDF_AtkGetLabel),
  DF("OVERKILL STATE", overkill, DDF_AtkGetLabel),

	DDF_CMD_END
};

// -KM- 1998/09/27 Major changes to sound handling
// -KM- 1998/11/25 Accuracy + Translucency are now fraction.  Added a spare attack for BFG.
// -KM- 1999/01/29 Merged in thing commands, so there is one list of
//  thing commands for all types of things (scenery, items, creatures + projectiles)

#undef  DDF_CMD_BASE
#define DDF_CMD_BASE  buffer_atk

static const commandlist_t attack_commands[] =
{
	// sub-commands
  DDF_SUB_LIST("DAMAGE", damage, damage_commands, dummy_damage),

	DF("ATTACKTYPE", ddf, DDF_AtkGetType),
	DF("ATTACK SPECIAL", ddf, DDF_AtkGetSpecial),
	DF("ACCURACY SLOPE", accuracy_slope, DDF_MainGetSlope),
	DF("ACCURACY ANGLE", accuracy_angle, DDF_MainGetAngle),
	DF("ATTACK HEIGHT", height, DDF_MainGetFloat),
	DF("SHOTCOUNT", count, DDF_MainGetNumeric),
	DF("X OFFSET", xoffset, DDF_MainGetFloat),
	DF("Y OFFSET", yoffset, DDF_MainGetFloat),
	DF("ANGLE OFFSET", angle_offset, DDF_MainGetAngle),
	DF("SLOPE OFFSET", slope_offset, DDF_MainGetSlope),
	DF("ATTACKRANGE", range, DDF_MainGetFloat),
	DF("TOO CLOSE RANGE", tooclose, DDF_MainGetNumeric),
	DF("NO TRACE CHANCE", notracechance, DDF_MainGetPercent),
	DF("KEEP FIRING CHANCE", keepfirechance, DDF_MainGetPercent),
	DF("ASSAULT SPEED", assault_speed, DDF_MainGetFloat),
	DF("ATTEMPT SOUND", initsound, DDF_MainLookupSound),
	DF("ENGAGED SOUND", sound, DDF_MainLookupSound),
	DF("SPAWNED OBJECT", spawnedobj_ref, DDF_MainGetString),
	DF("SPAWN OBJECT STATE", objinitstate_ref, DDF_MainGetString),
	DF("PUFF", puff_ref, DDF_MainGetString),
	DF("ATTACK CLASS", attack_class, DDF_MainGetBitSet),

	// -AJA- backward compatibility cruft...
	DF("!DAMAGE", damage.nominal, DDF_MainGetFloat),
	{"!DAMAGE RANGE", DDF_MainGetFloat, &a_damage_range, NULL},
	{"!DAMAGE MULTI", DDF_MainGetFloat, &a_damage_multi, NULL},

	DDF_CMD_END
};


//
//  DDF PARSE ROUTINES
//

static bool AttackStartEntry(const char *name)
{
	int i;
	bool replaces = false;

	if (name && name[0])
	{
		for (i=num_disabled_attacks; i < num_ddf_attacks; i++)
		{
			if (DDF_CompareName(ddf_attacks[i]->ddf.name, name) == 0)
			{
				dynamic_atk = ddf_attacks[i];
				replaces = true;
				break;
			}
		}
    
		// if found, adjust pointer array to keep newest entries at end
		if (replaces && i < (num_ddf_attacks-1))
		{
			Z_MoveData(ddf_attacks + i, ddf_attacks + i + 1, attacktype_t *,
					   num_ddf_attacks - i);
			ddf_attacks[num_ddf_attacks - 1] = dynamic_atk;
		}
	}

	// not found, create a new one
	if (! replaces)
	{
		Z_SetArraySize(&ddf_attacks_a, ++num_ddf_attacks);
    
		dynamic_atk = ddf_attacks[num_ddf_attacks - 1];
		dynamic_atk->ddf.name = (name && name[0]) ? Z_StrDup(name) :
			DDF_MainCreateUniqueName("UNNAMED_ATTACK", num_ddf_attacks);
	}

	dynamic_atk->ddf.number = 0;

	attack_has_mobj = false;
	a_damage_range = -1;
	a_damage_multi = -1;

	// instantiate the static entries
	buffer_atk  = template_atk;
	buffer_mobj = template_mobj;

	return replaces;
}

static void AttackParseField(const char *field, const char *contents,
							 int index, bool is_last)
{
#if (DEBUG_DDF)  
	L_WriteDebug("ATTACK_PARSE: %s = %s;\n", field, contents);
#endif

	// first, check attack commands
	if (DDF_MainParseField(attack_commands, field, contents))
		return;

	// we need to create an MOBJ for this attack
	attack_has_mobj = true;

	ThingParseField(field, contents, index, is_last);
}

static void AttackFinishEntry(void)
{
	ddf_base_t base;
  
	// FIXME: check stuff...

	// handle attacks that have mobjs
	if (attack_has_mobj)
	{
		if (buffer_mobj.first_state)
			DDF_StateFinishStates(buffer_mobj.first_state, 
								  buffer_mobj.last_state);

		buffer_atk.atk_mobj = DDF_MobjMakeAttackObj(&buffer_mobj,
													dynamic_atk->ddf.name);
	}
	else
		buffer_atk.atk_mobj = NULL;
  
	// compute an attack class, if none specified
	if (buffer_atk.attack_class == BITSET_EMPTY)
	{
		buffer_atk.attack_class = attack_has_mobj ? BITSET_MAKE('M') : 
			(buffer_atk.attackstyle == ATK_CLOSECOMBAT ||
			 buffer_atk.attackstyle == ATK_SKULLFLY) ? 
			BITSET_MAKE('C') : BITSET_MAKE('B');
	}

	// -AJA- 2001/01/27: Backwards compatibility
	if (a_damage_range > 0)
	{
		buffer_atk.damage.nominal = a_damage_range;

		if (a_damage_multi > 0)
			buffer_atk.damage.linear_max = a_damage_range * a_damage_multi;
	}

	// transfer static entry to dynamic entry
  
	base = dynamic_atk->ddf;
	dynamic_atk[0] = buffer_atk;
	dynamic_atk->ddf = base;

	// compute CRC...
	CRC32_Init(&dynamic_atk->ddf.crc);

	// FIXME: add more stuff...

	CRC32_Done(&dynamic_atk->ddf.crc);
}

static void AttackClearAll(void)
{
	// not safe to delete attacks -- mark as disabled
  
	num_disabled_attacks = num_ddf_attacks;
}


void DDF_ReadAtks(void *data, int size)
{
	readinfo_t attacks;

	attacks.memfile = (char*)data;
	attacks.memsize = size;
	attacks.tag = "ATTACKS";
	attacks.entries_per_dot = 2;

	if (attacks.memfile)
	{
		attacks.message = NULL;
		attacks.filename = NULL;
		attacks.lumpname = "DDFATK";
	}
	else
	{
		attacks.message = "DDF_InitAttacks";
		attacks.filename = "attacks.ddf";
		attacks.lumpname = NULL;
	}

	attacks.start_entry  = AttackStartEntry;
	attacks.parse_field  = AttackParseField;
	attacks.finish_entry = AttackFinishEntry;
	attacks.clear_all    = AttackClearAll;

	DDF_MainReadFile(&attacks);
}

void DDF_AttackInit(void)
{
	Z_InitStackArray(&ddf_attacks_a, (void ***)&ddf_attacks, sizeof(attacktype_t), 0);
}

void DDF_AttackCleanUp(void)
{
	int i;
	attacktype_t *atk;

	for (i=num_disabled_attacks; i < num_ddf_attacks; i++)
	{
		atk = ddf_attacks[i];
    
		DDF_ErrorSetEntryName("[%s]  (attacks.ddf)", atk->ddf.name);

		// lookup thing references

		atk->puff = atk->puff_ref ? DDF_MobjLookup(atk->puff_ref) : NULL;

		atk->spawnedobj = atk->spawnedobj_ref ? 
			DDF_MobjLookup(atk->spawnedobj_ref) : NULL;
      
		if (atk->spawnedobj)
		{
			if (atk->objinitstate_ref)
				atk->objinitstate = DDF_MainLookupDirector(atk->spawnedobj, atk->objinitstate_ref);
			else
				atk->objinitstate = atk->spawnedobj->spawn_state;
		}

		DDF_ErrorClearEntryName();
	}
}

static const specflags_t attack_specials[] =
{
    {"SMOKING TRACER", AF_TraceSmoke, 0},
    {"KILL FAILED SPAWN", AF_KillFailedSpawn, 0},
    {"REMOVE FAILED SPAWN", AF_KillFailedSpawn, 1},
    {"PRESTEP SPAWN", AF_PrestepSpawn, 0},
    {"SPAWN TELEFRAGS", AF_SpawnTelefrags, 0},
    {"NEED SIGHT", AF_NeedSight, 0},
    {"FACE TARGET", AF_FaceTarget, 0},

    {"FORCE AIM", AF_ForceAim, 0},
    {"ANGLED SPAWN", AF_AngledSpawn, 0},
    {"PLAYER ATTACK", AF_Player, 0},
    {"TRIGGER LINES", AF_NoTriggerLines, 1},

    // -AJA- backwards compatibility cruft...
    {"!NOAMMO", AF_None, 0},
    {NULL, AF_None, 0}
};

static void DDF_AtkGetSpecial(const char *info, void *storage)
{
	int flag_value;

	switch (DDF_MainCheckSpecialFlag(info, attack_specials, &flag_value, true, false))
	{
		case CHKF_Positive:
			buffer_atk.flags = (attackflags_e)(buffer_atk.flags | flag_value);
			break;
    
		case CHKF_Negative:
			buffer_atk.flags = (attackflags_e)(buffer_atk.flags & ~flag_value);
			break;

		case CHKF_User:
		case CHKF_Unknown:
			DDF_WarnError("DDF_AtkGetSpecials: Unknown Attack Special: %s", info);
			break;
	}
}

// -KM- 1998/11/25 Added new attack type for BFG: Spray
static const char *attack_class[NUMATKCLASS] =
{
    "NONE",
    "PROJECTILE",
    "SPAWNER",
    "TRIPLE SPAWNER",
    "FIXED SPREADER",
    "RANDOM SPREADER",
    "SHOT",
    "TRACKER",
    "CLOSECOMBAT",
    "SHOOTTOSPOT",
    "SKULLFLY",
    "SMARTPROJECTILE",
    "SPRAY"
};

static void DDF_AtkGetType(const char *info, void *storage)
{
	int i;

	i = 0;

	while (i != NUMATKCLASS && DDF_CompareName(info, attack_class[i]))
		i++;

	if (i == NUMATKCLASS)
	{
		DDF_WarnError("DDF_AtkGetType: No such attack type '%s'\n", info);
		buffer_atk.attackstyle = ATK_SHOT;
		return;
	}
  
	buffer_atk.attackstyle = (attackstyle_e)i;
}

//
// DDF_AtkGetLabel
//
static void DDF_AtkGetLabel(const char *info, void *storage)
{
	label_offset_t *lab = (label_offset_t *)storage;

	// check for `:' in string
	const char *div = strchr(info, DIVIDE);

	int i = div ? (div - info) : (int)strlen(info);

	if (i <= 0)
		DDF_Error("Bad State `%s'.\n", info);

	lab->label = Z_New(const char, i + 1);
	Z_StrNCpy((char *)lab->label, info, i);

	lab->offset = div ? MAX(0, atoi(div+1) - 1) : 0;
}

//
// DDF_AttackLookup
//
attacktype_t *DDF_AttackLookup(const char *name)
{
	int i;

	if (!name || !name[0])
		DDF_Error("Missing attack name !\n");

	for (i=num_disabled_attacks; i < num_ddf_attacks; i++)
	{
		if (DDF_CompareName(ddf_attacks[i]->ddf.name, name) == 0)
			return ddf_attacks[i];
	}

	DDF_WarnError("Unknown Attack: %s\n", name);
	return NULL;
}
