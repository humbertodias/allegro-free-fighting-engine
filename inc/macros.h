/*
    KOF2003 - A Engine Fight Game

    Maintainers:
    Copyright (c) 2003 by Claudemir Jr <claudemir.daluz@virtuallink.com.br>
    Copyright (c) 2003 by Rick Leite   <ric@asbyte.com.br>

    Based on KOF91 code ( http://kof91.com )
    Copyright (c) 2003 by Nicolas Sevez , aka Moah (moah@kof91.com)

    Linux Ported Copyright (c) Duncan M. Vicar <duncan@kde.org>

    Contributors:
    TDAM code Copyright (c) 2003 by Ivan Toledo <birdie@alumnos.utem.cl>
    
    *************************************************************************
    *                                                                       *
    * This program is free software; you can redistribute it and/or modify  *
    * it under the terms of the GNU General Public License as published by  *
    * the Free Software Foundation; either version 2 of the License, or     *
    * (at your option) any later version.                                   *
    * Please for more information see license.txt                           *
    *                                                                       *
    *************************************************************************
*/
#ifndef _MACROS_H
#define _MACROS_H

#include "config.h"
#define limd            (zoomwidth - (4*p2.width/5))
#define limg            (2 - (p1.width/5))
#define ZMRATIO			(int)(zoomwidth/1.6)
#define JOY1_WP		(joy[0].button[0].b)
#define JOY1_SP		(joy[0].button[1].b)
#define JOY1_WK		(joy[0].button[2].b)
#define JOY1_SK		(joy[0].button[3].b)

#define JOY1_UP		(joy[0].stick[0].axis[1].d1)
#define JOY1_DN		(joy[0].stick[0].axis[1].d2)
#define JOY1_LT		(joy[0].stick[0].axis[0].d1)
#define JOY1_RT		(joy[0].stick[0].axis[0].d2)

#define P1_UP		( key[p1_up] || JOY1_UP )
#define P1_DN		( key[p1_down] || JOY1_DN )

#if TEMP > 0
#define P1_LT		( key[(inverted?p1_right:p1_left)] || JOY1_LT )
#define P1_RT		( key[(inverted?p1_left:p1_right)] || JOY1_RT )
#else
#define P1_LT		( key[p1_left] || JOY1_LT )
#define P1_RT		( key[p1_right] || JOY1_RT )
#endif

#define P1_WP		( key[p1_wpunch] || JOY1_WP )
#define P1_SP		( key[p1_spunch] || JOY1_SP )
#define P1_WK		( key[p1_wkick] || JOY1_WK )
#define P1_SK		( key[p1_skick] || JOY1_SK )

#define P2_UP		( key[p2_up] )
#define P2_DN		( key[p2_down] )

#if TEMP > 0
#define P2_LT		( key[(inverted?p2_right:p2_left)] )
#define P2_RT		( key[(inverted?p2_left:p2_right)] )
#else
#define P2_LT		( key[p2_left] )
#define P2_RT		( key[p2_right] )
#endif

#define P2_WP		( key[p2_wpunch] )
#define P2_SP		( key[p2_spunch] )

#define P2_WK		( key[p2_wkick] )
#define P2_SK		( key[p2_skick] )

/*************************/
/* SELECT SYSTEM */

/*********/
#if (SO == LINUX) || (SO == FREEBSD)
#define DIR_BAR "/"
#define DEFAULT_GFX 1 
/* *NIX Video modes (see alunix.h):
    GFX_XWINDOWS, GFX_XDGA, GFX_XDFS, GFX_VGA, GFX_MODEX, 
    GFX_FBCON, GFX_VBEAF, GFX_GGI, GFX_SVGALIB,
    GFX_AUTODETECT 
    DEFAULT_GFX
*/
#define BR_GFX GFX_XWINDOWS
#endif 
/*********/

/*********/
#if SO == DOS
#define DIR_BAR "\\"
#define DEFAULT_GFX 1 
/* DJGPP Video Modes (See aldos.h):
   GFX_VGA, GFX_MODEX, GFX_VESA1, GFX_VESA2B, GFX_VESA2L, GFX_VESA3, 
   GFX_VBEAF, GFX_XTENDED, 
   GFX_DRIVER_VGA, GFX_DRIVER_MODEX, GFX_DRIVER_VBEAF,
   GFX_DRIVER_VESA3, GFX_DRIVER_VESA2L, GFX_DRIVER_VESA2B,
   GFX_DRIVER_XTENDED, GFX_DRIVER_VESA1,
   GFX_AUTODETECT,
   DEFAULT_GFX
*/
#define BR_GFX DEFAULT_GFX
#endif 
/*********/

/*********/
#if SO == MSCPP
#define DIR_BAR "\\" 
/* rWin Video Modes (see alwin.h):
   GFX_DIRECTX, GFX_DIRECTX_ACCEL, 
   GFX_DIRECTX_SAFE, GFX_DIRECTX_SOFT,
   GFX_DIRECTX_WIN, GFX_DIRECTX_OVL,
   GFX_GDI, GFX_DRIVER_DIRECTX,
   GFX_AUTODETECT
*/
#define BR_GFX GFX_DIRECTX
#endif 
/*********/
#if SO == BEOS
#define DIR_BAR "/" 
/* BEOS Video Modes (see albeos.h):
   GFX_BEOS, GFX_BEOS_FULLSCREEN, GFX_BEOS_FULLSCREEN_SAFE,
   GFX_BEOS_WINDOWED, GFX_BEOS_WINDOWED_SAFE, 
   GFX_DRIVER_BEOS, 
*/
#define BR_GFX GFX_BEOS_FULLSCREEN_SAFE
#endif

/*************************/
#define fill_sprite(a) rectfill(a, 0, 0, a->w, a->h, makecol(255,0,255));

#if GAME_DEBUG > 0
#define TRONSTART(a) __tronstart__( a, __FILE__, __LINE__ )
#define TRON(a) __tron__( a, __FILE__, __LINE__ )
#endif

#if GAME_DEBUG == 0
#define TRONSTART(a) /* a */
#define TRON(a)  /* a */
#endif

#define ERROR(a) __error__( a, __FILE__, __LINE__ )

#if GAME_DEBUG > 0
#define load_sprite(a,b) __load_sprite__( a, b, __FILE__, __LINE__ )
#define xload_pcx(a,b) __xload_pcx__( a, b, __FILE__, __LINE__ )
#define xload_sample(a) __xload_sample__( a, __FILE__, __LINE__ )
#define xload_midi(a) __xload_midi__( a, __FILE__, __LINE__ )
#else
#define load_sprite(a,b) __load_sprite__( a, b )
#define xload_pcx(a,b) __xload_pcx__( a, b )
#define xload_sample(a) __xload_sample__( a )
#define xload_midi(a) __xload_midi__( a )
#endif

#define F_SLOWDOWN 0
#define F_SLOWDOWN_END_ROUND 1

#define K_aacute 'á'
#define K_eacute 'é'
#define K_iacute 'í'
#define K_oacute 'ó'
#define K_uacute 'ú'

#define K_agrave 'à'
#define K_egrave 'è'
#define K_igrave 'ì'
#define K_ograve 'ò'
#define K_ugrave 'ù'

#define K_atilde 'ã'
#define K_otilde 'õ'

#define K_ccedil 'ç'

#define K_ntilde 'ñ'

#define K_auml   'ä'
#define K_euml   'ë'
#define K_iuml   'ï'
#define K_ouml   'ö'
#define K_uuml   'ü'

#define K_acirc  'â' 
#define K_ecirc  'ê'
#define K_icirc  'î'
#define K_ocirc  'ô'
#define K_ucirc  'û'

#define K_Aacute 'Á'
#define K_Eacute 'É'
#define K_Iacute 'Í'
#define K_Oacute 'Ó'
#define K_Uacute 'Ú'

#define K_Agrave 'À'
#define K_Egrave 'È'
#define K_Igrave 'Ì'
#define K_Ograve 'Ò'
#define K_Ugrave 'Ù'

#define K_Atilde 'Ã'
#define K_Otilde 'Õ'

#define K_Ccedil 'Ç'

#define K_Ntilde 'Ñ'

#define K_Auml   'Ä'
#define K_Euml   'Ë'
#define K_Iuml   'ï'
#define K_Ouml   'Ö'
#define K_Uuml   'Ü'

#define K_Acirc  'Â' 
#define K_Ecirc  'Ê'
#define K_Icirc  'Î'
#define K_Ocirc  'Ô'
#define K_Ucirc  'Û'

#endif

// EOF
