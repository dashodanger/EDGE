//----------------------------------------------------------------------------
//  Radius Trigger header file
//----------------------------------------------------------------------------
// 
//  Copyright (c) 1999-2000  The EDGE Team.
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

#ifndef __RAD_TRIG__
#define __RAD_TRIG__

#include "dm_type.h"
#include "e_player.h"
#include "rad_main.h"

// Radius Trigger Parser Version
#define PARSERV       11
#define PARSERVFIX    10

#define MAXSTRLEN     512

extern rad_script_t *r_scripts;
extern rad_trigger_t *r_triggers;
extern int rad_itemsread;

// Tip Prottypes
void RAD_ResetTips(void);
void TIP_SendTip(s_tip_t * tip);
void TIP_DisplayTips(int y);

// RadiusTrigger & Scripting Prototypes
boolean_t RAD_Init(void);
void RAD_LoadScript(const char *name, int lump, boolean_t dots);
void RAD_SpawnTriggers(char *map_name);
void RAD_ClearTriggers(void);

void RAD_DoRadiTrigger(player_t * p);
void RAD_Ticker(void);
boolean_t RAD_WithinRadius(mobj_t * mo, rad_script_t * r);
rad_script_t *RAD_FindScriptByName(char *name);
rad_trigger_t *RAD_FindTriggerByName(char *name);
rts_state_t *RAD_FindStateByLabel(rad_script_t *scr, char *label);
void RAD_EnableByTag(int tag, boolean_t disable);

// Path support
boolean_t RAD_CheckReachedTrigger(mobj_t * thing);

//
//  PARSING
//
void RAD_ParserBegin(void);
void RAD_ParserDone(void);
void RAD_ParseLine(char *s);
void RAD_Error(const char *err, ...);
int RAD_StringHashFunc(const char *s);

extern int rad_cur_linenum;
extern char *rad_cur_filename;

//
//  ACTIONS
//
void RAD_ActNOP(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActTip(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActSpawnThing(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActPlaySound(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActSectorMove(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActSectorLight(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActEnableScript(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActActivateLinetype(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActJump(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActSleep(rad_trigger_t *R, mobj_t *actor, void *param);

void RAD_ActDamagePlayers(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActHealPlayers(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActArmourPlayers(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActDamageMonsters(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActSkill(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActGotoMap(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActExitLevel(rad_trigger_t *R, mobj_t *actor, void *param);
void RAD_ActSaveGame(rad_trigger_t *R, mobj_t *actor, void *param);

#endif
