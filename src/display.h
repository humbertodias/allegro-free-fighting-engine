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
#ifndef _DISPLAY_H
#define _DIAPLAY_H

void disp_ini_var( void );
void disp_put_var(char name_variable[40], int *pvar);
#if GAME_DEBUG > 0
void show_fpc( void );
#endif
void displays( void );
void initdisplay( void );
void refresh_lifebar( void );
void refresh_powerbar( void );
void refresh_timer( void );
void start_timer( void );
void refresh_vflag( void );
// claudemir
void refresh_life_faces( void );

void text_centre_shadow( BITMAP *outbm, FONT *font, char *text, int x, int y, int color );
void text_shadow( BITMAP *outbm, FONT *font, char *text, int x, int y, int color );
void refresh_vflags( void );	
int xget_config_int( char *section, char *var_name, int val_defa );
char *xget_config_string( char *section, char *var_name, char *val_defa );
void save_display( void );
void PUT_t_clock( int param );
int GET_t_clock( void );
void K_listvars( char param[40] ); // monitor command 
void K_defvar( char namevar[40] );
void K_setvar( char namevar[40], char param[40], char param2[40] ); // monitor command 
void adjusts( const char f_s );

#endif

// EOF
