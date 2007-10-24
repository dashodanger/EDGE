//----------------------------------------------------------------------------
//  EDGE Music handling Code
//----------------------------------------------------------------------------
// 
//  Copyright (c) 1999-2007  The EDGE Team.
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
// -ACB- 1999/11/13 Written
//

#include "i_defs.h"

#include <stdlib.h>

#include "epi/file.h"
#include "epi/filesystem.h"

#include "ddf/main.h"

#include "dm_state.h"
#include "s_sound.h"
#include "s_music.h"
#include "m_misc.h"
#include "w_wad.h"

// Current music handle
static int musichandle = -1;

// music slider value
int mus_volume;

bool nomusic = false;
bool nocdmusic = false;

// ================ END OF INTERNALS =================

//
// S_ChangeMusic
//
void S_ChangeMusic(int entrynum, bool looping)
{
	const pl_entry_c *play;
	byte *data;
	int datlength;
	int datnum;
	i_music_info_t musdat;

	if (nomusic)
		return;
	
	// -AJA- playlist number 0 reserved to mean "no music"
	if (entrynum <= 0)
	{
		S_StopMusic();
		return;
	}

	play = playlist.Find(entrynum);
	if (!play)
		return;

	S_StopMusic();

	if (play->type == MUS_MP3)
	{
		I_Warning("S_ChangeMusic: MP3 music no longer supported.\n");
		return;
	}

	float volume = slider_to_gain[mus_volume];

	// -ACB- 2000/06/06 This is not system specific
	if (play->infotype == MUSINF_FILE)
	{
		data = NULL;

		//
		// -ACB- 2004/08/18 Something of a hack until we revamp this to be
		//                  a little less platform dependent and a little
		//                  more object orientated
		//
		if (play->type != MUS_OGG)
		{
			// -AJA- 2005/01/15: filenames in DDF relative to GAMEDIR
			std::string fn = M_ComposeFileName(game_dir.c_str(), play->info.c_str());

			data = M_GetFileData(fn.c_str(), &datlength);

			if (!data)
			{
				I_Warning("S_ChangeMusic: Can't Load File '%s'\n", fn.c_str());
				return;
			}

			musdat.format = IMUSSF_DATA;
			musdat.info.data.ptr = data;
			musdat.info.data.size = datlength;
		}
		else
		{
			// -AJA- 2005/01/15: filenames in DDF relative to GAMEDIR
			std::string fn = M_ComposeFileName(game_dir.c_str(), play->info.c_str());

			if (! epi::FS_Access(fn.c_str(), epi::file_c::ACCESS_READ))
			{
				I_Warning("S_ChangeMusic: Can't Load OGG '%s'\n", fn.c_str());
				return;
			}

			musdat.format = IMUSSF_FILE;
			musdat.info.file.name = fn.c_str();
		}

		musichandle = I_MusicPlayback(&musdat, play->type, looping, volume);

		if (data)
			delete [] data;
	}

	if (play->infotype == MUSINF_LUMP)
	{
		datnum = W_CheckNumForName(play->info);
		if (datnum != -1)
		{
			datlength = W_LumpLength(datnum);
			data = (byte*)W_CacheLumpNum(datnum);

			musdat.format = IMUSSF_DATA;
			musdat.info.data.ptr = data;
			musdat.info.data.size = datlength;

			musichandle = I_MusicPlayback(&musdat, play->type, looping, volume);
			W_DoneWithLump(data);
		}
		else
		{
			I_Warning("S_ChangeMusic: LUMP '%s' not found.\n", play->info.c_str()); 
			return;
		}
	}

	if (play->infotype == MUSINF_TRACK)
	{
		musdat.format = IMUSSF_CD;
		musdat.info.cd.track = atoi(play->info);

		musichandle = I_MusicPlayback(&musdat, play->type, looping, volume);
	}

	if (musichandle == -1)
		I_Printf("%s\n", I_MusicReturnError());
}

//
// S_ResumeMusic
//
void S_ResumeMusic(void)
{
	if (nomusic || musichandle == -1)
		return;

	I_MusicResume(&musichandle);
	return;
}

//
// S_PauseMusic
//
void S_PauseMusic(void)
{
	if (nomusic || musichandle == -1)
		return;

	I_MusicPause(&musichandle);
	return;
}

//
// S_StopMusic
//
void S_StopMusic(void)
{
	if (nomusic || musichandle == -1)
		return;

	I_MusicKill(&musichandle);
	return;
}

//
// S_MusicTicker
//
void S_MusicTicker(void)
{
	I_MusicTicker(&musichandle);
	return;
}

///---//
///---// S_GetMusicVolume
///---//
///---int S_GetMusicVolume(void)
///---{
///---	return musicvolume;
///---}

void S_ChangeMusicVolume(void)
{
///---	SYS_ASSERT(volume >= 0 && volume < SND_SLIDER_NUM);
///---
///---	musicvolume = volume;

	if (nomusic || musichandle == -1)
		return;

	I_SetMusicVolume(&musichandle, slider_to_gain[mus_volume]);
}


//--- editor settings ---
// vi:ts=4:sw=4:noexpandtab
