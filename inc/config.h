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
#ifndef _CONFIG_H
#define _CONFIG_H

#include "defs.h"

#define VERSION "1.3.5refat1"

/* SO xxx
   xxx = DOS      // for DOS/DJGPP  
   xxx = LINUX    // for LINUX and gcc
   xxx = CYGWIN   // for WINDOWS with CYGWIN compiler (http://cygwin.com)
   xxx = MSVCPP   // for M$ Visual C++
 //   #define SO DOS

*/

#include "so.inc"

/*   GAME_DEBUG
     0 = NOT debug mode 
     1 = debug mode level 1
     2 = debug mode level 2 (verbose)
     
     *Motive of change: binary small and faster
*/

#define GAME_DEBUG 0

/*
    CHEAT
    0 = NOT allow change in monitor life and time and power bar
    1 = allow change...
 */

#define CHEAT 1

/*
  TIMEOVER
   0 = disable
   1 = enable
   *Exibe cronometer and interrupt figth if time is ZERO.
   * Exibe Time Up message (timeup.pcx) if time over.
*/
#define TIMEOVER 1


/*
    PERFECT
     0 - disable 
     1 - enable
     * If player wins and get zero damage then
      Exibe sprite perfect.pcx and play perfect.wav 
*/
#define PERFECT 1

/*
  SCREENSHOT
  0 - disable
  1 - enable
  * press <F2> and save a screenshot *
*/
#define SCREENSHOT 1


/*
  TEMP
  0 - disable
  1 - enable
  * activate of features in development *
*/
#define TEMP 1



#define DEF_MAX_MONITOR_VARS 150
#define DEF_MAX_USER_VARS 50 
#define DEF_MAX_CICLES 10000
#define DEF_MAX_CLONE_PIXELS 25

/* 
  SOUND CONSTANTS

*/

#define DEF_PAN 128
#define DEF_PITCH 1000


#define DIR_SPRITES "data" DIR_BAR "sprites" DIR_BAR
#define DIR_WAV "data" DIR_BAR "wav" DIR_BAR
#define DIR_MIDI "data" DIR_BAR "midi" DIR_BAR
#define DIR_BKGDS "data" DIR_BAR "bkgds" DIR_BAR

/* END OF CONFIG FLAGS */

/*******************************/
/* This redefinitions is necessary for TRON() ...*/
#define BRDATE  __DATE__
#define BRTIME  __TIME__

#endif

// EOF
