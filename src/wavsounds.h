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

#ifndef _WAVSOUNDS_H
#define _WAVSOUNDS_H

void set_snd_vol( int xpar );
int get_snd_vol( void );
void loadsounds( void );
void freesounds( void );
void playwav( int wavindex );
void loadwav (int wavindex, char *filename);
void Buzz ( void );
void Bing ( void );
void Done ( void );
void Block ( void );
void Paf ( char gana );
void Hurt ( char gana );
void Round ( char which );
void Fight( void );

/*************************************************************************/
#define DEF_rd1        0
#define DEF_rd2        1
#define DEF_rd3        2
#define DEF_fight      3 
#define DEF_wp_alwa    6
#define DEF_wk_alwa    7
#define DEF_sp_alwa    8
#define DEF_sk_alwa    9
#define DEF_ko         28
#define DEF_argh       29
#define DEF_female     30
#define DEF_done       25
#define DEF_bing       26
#define DEF_buzz       27
#define DEF_hit1       10
#define DEF_hit2       11
#define DEF_block      12

#define DEF_hadoken1   13
#define DEF_hadoken2   14
#define DEF_copter1    15
#define DEF_copter2    16
#define DEF_dragon1    17
#define DEF_dragon2    18
#define DEF_hadokenx1  19
#define DEF_hadokenx2  20
#define DEF_copterx1   21
#define DEF_copterx2   22
#define DEF_dragonx1   23
#define DEF_dragonx2   24

#if TIMEOVER == 1
  #define DEF_to       32
#endif

#if PERFECT == 1
  #define DEF_prfct    31
#endif
#define DEF_MAX_WAV_SOUNDS 33

#endif

// EOF


