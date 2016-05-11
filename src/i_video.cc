//----------------------------------------------------------------------------
//  EDGE2 SDL Video Code
//----------------------------------------------------------------------------
// 
//  Copyright (c) 1999-2009  The EDGE2 Team.
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

#include "i_defs.h"
#include "i_sdlinc.h"
#include "i_defs_gl.h"

#ifdef WIN32
#include "GL/wglew.h"
#else
#include <GL/glew.h>
#endif

#ifdef MACOSX
#include <SDL2/SDL_opengl.h>
#else
#include <SDL_opengl.h>
#endif

#include <signal.h>

#include "m_argv.h"
#include "m_misc.h"
#include "r_modes.h"

extern cvar_c r_width, r_height, r_depth, r_fullscreen;

//The window we'll be rendering to
SDL_Window *my_vis;

int graphics_shutdown = 0;

cvar_c in_grab;

static bool grab_state;

static int display_W, display_H;

SDL_GLContext   glContext;


// Possible Screen Modes
static struct { int w, h; } possible_modes[] =
{
	{  320, 200, },
	{  320, 240, },
	{  400, 300, },
	{  512, 384, },
	{  640, 400, },
	{  640, 480, },
	{  800, 600, },
	{ 1024, 768, },
	{ 1280,1024, },
	{ 1600,1200, },

	{  -1,  -1, }
};


void I_GrabCursor(bool enable)
{
	if (! my_vis || graphics_shutdown)
		return;

	grab_state = enable;

	if (grab_state && in_grab.d)
	{
		SDL_ShowCursor(0);
//		SDL_WM_GrabInput(SDL_GRAB_ON);
		SDL_SetWindowGrab(my_vis, SDL_TRUE);
	}
	else
	{
		SDL_ShowCursor(1);
//		SDL_WM_GrabInput(SDL_GRAB_OFF);
		SDL_SetWindowGrab(my_vis, SDL_FALSE);
	}
}


void I_StartupGraphics(void)
{
	
	uint32_t  flags = 0;
    char    title[256];
	
	if (M_CheckParm("-directx"))
		force_directx = true;

	if (M_CheckParm("-gdi") || M_CheckParm("-nodirectx"))
		force_directx = false;

	const char *driver = M_GetParm("-videodriver");

	if (! driver)
		driver = SDL_getenv("SDL_VIDEODRIVER");

	if (! driver)
	{
		driver = "default";

#ifdef WIN32
		if (force_directx)
			driver = "directx";
#endif
	}

	if (stricmp(driver, "default") != 0)
	{
		char nameBuffer[200];
		char valueBuffer[200];
		bool overWrite = true;
		snprintf(nameBuffer, sizeof(nameBuffer), "SDL_VIDEODRIVER");
		snprintf(valueBuffer, sizeof(valueBuffer), "%s", driver);
		SDL_setenv(nameBuffer, valueBuffer, overWrite);
	}

	I_Printf("SDL_Video_Driver: %s\n", driver);


	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
		I_Error("Couldn't init SDL VIDEO!\n%s\n", SDL_GetError());

	if (M_CheckParm("-nograb"))
		in_grab = 0;
	
#ifdef OPENGL_2
SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
#endif	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   16);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	///SDL_GL_SetAttribute(SDL_GL_S, 1);
	SDL_GL_SetSwapInterval(-1); 
	
	// ~CA 5.7.2016:
	
/* 	Some systems allow specifying -1 for the interval, 
	to enable late swap tearing. Late swap tearing works 
	the same as vsync, but if you've already missed the 
	vertical retrace for a given frame, it swaps buffers 
	immediately, which might be less jarring for the user 
	during occasional framerate drops. If application 
	requests late swap tearing and the system does not support 
	it, this function will fail and return -1. In such a case, 
	you should probably retry the call with 1 for the interval. */
	
	flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS;
	
	sprintf(title, "3DGE");
    my_vis = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              display_W,
                              display_H,
                              flags);
	
	    if(my_vis == NULL) 
	{
        I_Error("I_InitScreen: Failed to create window");
        return;
    }

	I_Printf("I_StartupGraphics: initialisation OK\n");
}


bool I_SetScreenSize(scrmode_c *mode)
{
	///I_Printf("I_SetScreenSize = reached");
	I_GrabCursor(false);

	I_Printf("I_SetScreenSize: trying %dx%d %dbpp (%s)\n",
			 mode->width, mode->height, mode->depth,
			 mode->full ? "fullscreen" : "windowed");

//	my_vis = SDL_SetVideoMode(mode->width, mode->height, mode->depth, 
//					SDL_OPENGL | SDL_DOUBLEBUF |
//					(mode->full ? SDL_FULLSCREEN : 0));
	my_vis = SDL_CreateWindow("Hyper3DGE",
					SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    mode->width, mode->height,
                    SDL_WINDOW_OPENGL | //SDL2 is double-buffered by default
                    (mode->full ? SDL_WINDOW_FULLSCREEN :0));
	SDL_GLContext context = SDL_GL_CreateContext( my_vis );
    SDL_GL_MakeCurrent( my_vis, context );

                    
	if (my_vis == NULL)
	{
		I_Printf("I_SetScreenSize: (mode not possible)\n");
		return false;
	}

/* 	stupid shit we really do NOT need
if (my_vis->format->BytesPerPixel <= 1)
	{
		I_Printf("I_SetScreenSize: 8-bit mode set (not suitable)\n");
		return false;
	}

	 I_Printf("I_SetScreenSize: mode now %dx%d %dbpp flags:0x%x\n",
			 my_vis->w, my_vis->h,
			 my_vis->format->BitsPerPixel, my_vis->flags); 
			*/

	// -AJA- turn off cursor -- BIG performance increase.
	//       Plus, the combination of no-cursor + grab gives 
	//       continuous relative mouse motion.
	I_GrabCursor(true);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//function sets the active rendering area, it doesn't enforce any orthagonal matrices
	///glViewport( 0, 0, display_W, display_H );

#ifdef MACOSX
	//TODO: On Mac OS X make sure to bind 0 to the draw framebuffer before swapping the window, otherwise nothing will happen.
#endif
//	SDL_GL_SwapBuffers()
	SDL_GL_SwapWindow(my_vis);
	
	///I_Printf("Screen Size Set!/n");
	
	return true;
}


void I_StartFrame(void)
{
	///I_Printf("STARTING FRAME");
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}


void I_FinishFrame(void)
{
	extern cvar_c r_vsync;

#ifdef WIN32
	if (SDL_GL_GetSwapInterval)
	{
		if (r_vsync.d > 0)
			glFinish();
		SDL_GL_SetSwapInterval(r_vsync.d != 0);
	}
#endif

	SDL_GL_SwapWindow(my_vis);
	if (r_vsync.d > 0)
			glFinish();
	if (in_grab.CheckModified())
		I_GrabCursor(grab_state);
}


void I_PutTitle(const char *title)
{
	;
}

void I_SetGamma(float gamma)
{
	if (SDL_SetWindowBrightness(my_vis, gamma) < 0)
		I_Printf("Failed to change gamma.\n");
}


void I_ShutdownGraphics(void)
{
	if (graphics_shutdown)
		return;

	graphics_shutdown = 1;

	if (SDL_WasInit(SDL_INIT_EVERYTHING))
	{
        // reset gamma to default
        I_SetGamma(1.0f);

		SDL_Quit ();
	}
}


void I_GetDesktopSize(int *width, int *height)
{
	*width  = display_W;
	*height = display_H;
}

//--- editor settings ---
// vi:ts=4:sw=4:noexpandtab
