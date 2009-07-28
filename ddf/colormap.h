//----------------------------------------------------------------------------
//  EDGE Data Definition File Code (Colourmaps)
//----------------------------------------------------------------------------
// 
//  Copyright (c) 1999-2009  The EDGE Team.
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

#ifndef __DDF_COLORMAP_H__
#define __DDF_COLORMAP_H__

#include "epi/utility.h"

#include "main.h"

// -AJA- 1999/07/09: colmap.ddf structures.

typedef enum
{
	// Default value
	COLSP_None     = 0x0000,
	
	// don't apply gun-flash type effects (looks silly for fog)
	COLSP_NoFlash  = 0x0001,
}
colourspecial_e;

typedef struct colmapcache_s
{
	byte *data;
}
colmapcache_t;


class colourmap_c
{
public:
	epi::strent_c name;

	lumpname_c lump_name;

	int start;
	int length;

	colourspecial_e special;

	// colours for GL renderer
	rgbcol_t gl_colour;

	rgbcol_t font_colour;  // (computed only, not in DDF)

	colmapcache_t cache;

	void *analysis;

public:
	 colourmap_c();
	~colourmap_c();

	void Default();
	void CopyDetail(const colourmap_c &src);
	
private:
	// disable copy construct and assignment operator
	explicit colourmap_c(colourmap_c &rhs) { }
	colourmap_c& operator=(colourmap_c &rhs) { return *this; }
};


// Colourmap container
class colourmap_container_c : public epi::array_c
{
public:
	colourmap_container_c();
	~colourmap_container_c();

private:
	void CleanupObject(void *obj);

	int num_disabled;					// Number of disabled 

public:
	// List Management
	int GetSize() {	return array_entries; } 
	int Insert(colourmap_c *c) { return InsertObject((void*)&c); }
	colourmap_c* operator[](int idx) { return *(colourmap_c**)FetchObject(idx); } 

	// Inliners
	int GetDisabledCount() { return num_disabled; }
	void SetDisabledCount(int _num_disabled) { num_disabled = _num_disabled; }

	// Search Functions
	colourmap_c* Lookup(const char* refname);
};

extern colourmap_container_c colourmaps;	// -ACB- 2004/06/10 Implemented


void DDF_ColourmapAddRaw(const char *lump_name, int size);

#endif /* __DDF_COLORMAP_H__ */

//--- editor settings ---
// vi:ts=4:sw=4:noexpandtab
