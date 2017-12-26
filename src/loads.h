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
#ifndef _LOADS_H
#define _LOADS_H

#include "config.h"
#if GAME_DEBUG > 0
BITMAP *__load_sprite__( const char *file, PALLETE Pal, char *n_file , int n_line );
BITMAP *__xload_pcx__( const char *file, PALLETE Pal, char *n_file , int n_line );
//char *check_file_or_similar( const char * file, char * n_file , int n_line );
char check_file( const char * file, char * n_file , int n_line );
SAMPLE *__xload_sample__( const char *file , char * n_file , int n_line );
MIDI *__xload_midi__( const char *file, char * n_file , int n_line );
#else
BITMAP *__load_sprite__( const char *file, PALLETE Pal );
BITMAP *__xload_pcx__( const char *file, PALLETE Pal );
//char *check_file_or_similar( const char * file );
char check_file( const char * file );
SAMPLE *__xload_sample__( const char *file );
MIDI *__xload_midi__( const char *file );
#endif


char file_missing( const char * file );
char midi_ok( void );
// Isto tem que sair ...
BITMAP * create_shadow(BITMAP * src, int w, int h, char t_shadow);


// Sprites loading functions
// char max_nbf1, max_nbf2 ;

void load_sprites1 ( void );
void load_sprites2 ( void );
void load_kick1 ( void );
void load_kick2 ( void );
void load_win1 ( void );
void load_win2 ( void );
//void load_lose1 ( void );
//void load_lose2 ( void );


#endif

// EOF
