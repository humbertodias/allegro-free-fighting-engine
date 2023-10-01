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
#ifndef _MAIN_H
#define _MAIN_H

#include "config.h"
#include <string.h>
#include "allegro.h"
#include "macros.h"
#include "loads.h"

struct
{
	struct
	{
		int port;
		int volume;
		char *name;
	}
	output;
}
Options;

int mp3status;
// global variables
DATAFILE *creditflic;
DATAFILE *fonts_dat;
FONT *small_font;
MIDI *music;

BITMAP *virtscreen;             /* virtual screen                   */
BITMAP *zoomscreen;
BITMAP *scanliner;              /* blit screen for scanlines        */
PALLETE Pal;
PALLETE TPal;
BITMAP *Title;
BITMAP *Selecteur1;
BITMAP *Selecteur2;
BITMAP *LBbanner;
BITMAP *LBbanner2;
BITMAP *Bar;
BITMAP *Bar2;
BITMAP *Power;
BITMAP *PBar;
BITMAP *WImpact[ 11 ];
BITMAP *SImpact[ 11 ];
BITMAP *BlockImpact[ 11 ];
BITMAP *B_Temp;
BITMAP *Carton_R1;
BITMAP *Carton_R2;
BITMAP *Carton_R3;
BITMAP *Carton_FT;
BITMAP *Carton_WN;
BITMAP *Carton_KO;
#if TIMEOVER == 1
// TODO - MINE
//BITMAP *Carton_TO;
#endif
BITMAP *novic0;
BITMAP *novic1;
BITMAP *avic0;
BITMAP *avic1;
BITMAP *Face1;
BITMAP *Face2;
BITMAP *Icon1;
BITMAP *Icon2;
BITMAP *Lock;
BITMAP *Bkgd;
BITMAP *Frgd;
/* BITMAP *Blood; */
#if TIMEOVER > 0
// TODO - MINE
//BITMAP *B_clock[ 11 ];
#endif
#if PERFECT == 1
// TODO - MINE
//BITMAP *B_prfct;
#endif 
/******************************************/
char animated;
char foreground;
char bkgd_counter;
char bkgd_delay;
int bkgdlim;
int zoomsup;
int zoomwidth = 320;
unsigned char j;
char gmode , diff;
// story variables
char story;
int sel1, sel2;
// string and key variables
//char charname [ 1000 ][ 30 ];
//char bkgdname [ 1000 ][ 30 ];
//char flcname  [ 1000 ][ 30 ];
int nbchar;
int nbbkgd;
int nbflc;
char defeated[ 1000 ];
char present[ 1000 ];
char secretchar[ 1000 ];
int num;
char tilenum[ 4 ];
char charn1[ 40 ];
char charn2[ 40 ];
char bkgdnum[ 4 ];
char file[ 200 ];
char passeur[ 100 ];
char sectn[ 50 ];
char temp_strg[ 2 ];
char last_char;

char last_status1;
char last_status2;
int fball_h1 , fball_h2;
int fballx_h1 , fballx_h2;
char autho_ht1 , autho_ht2;
char wimpact_nbf;
char simpact_nbf;
char bimpact_nbf;
// control keys variables
int p1_up , p2_up;
int p1_down , p2_down;
int p1_left , p2_left;
int p1_right , p2_right;
int p1_wpunch , p2_wpunch;
int p1_spunch , p2_spunch;
int p1_wkick , p2_wkick;
int p1_skick , p2_skick;
// AI variables
int offense , defense;
char easy_def , easy_off;
char medium_def , medium_off;
char hard_def , hard_off;
int ai_walkf , ai_walkb;
int ai_static;

// misc
int mp3;
int intro_delay;
int midi_vol;
int start_x , start_y;
char gfmode;
int screen_height;
int screen_width;
char autorise;
/* Display: clock, Life bars, Power bars*/
int life1 , life2;
int spower1 , spower2;
char vic_flag;
char vic1 , vic2;
#if TIMEOVER > 0
int t_clock;
#endif

int clone = 0;
char inverted = 0;
char log_mess[ 80 ];
char c;
int cx, cy;                         // Current X and Y position in the bkgd + misc
int x, y;                           // Used in for loops  + misc
int fx, fy;
int ok = 1;                         // Loop until ok==0
int s = 2;                          // Scroll speed 2 pixels per frame
int misc;
//long t1, t2,
long n;				// For calculating FPS
int flag = 0;                     // sprite cycling flags
int flag2 = 0;
char hflag = 0, hflag2 = 0;
char frameskip = 0;
char skp_flag = 0;
int sens1 = 0;                     // jump flags
int sens2 = 0;
int i;
int g , h , nb;	       // Miscellaneous variables
char com_block = 0;
char com_gblock = 0;
char com_walkf = 0;
char com_walkb = 0;
int spr1x = 60 , spr1y = 77;       // Sprites coordinates
int spr2x = 210, spr2y = 77;
char oldsel1 , oldsel2;
char last_sel1 = 1, last_sel2 = 1; // INITIALIZING Selection of chars
char p2_sel;
int angle;
char bgd;
int sel_player1 , sel_player2;
int alt_color , tcolor , bcolor;
int sel_bkgd;
int text_color;
int deb;
char fight_round;
int actual = 0;			// actual sprite status
int actual2 = 0;
char ak , ak2 = 0;          // flag air-kick
char apch , apch2 = 0;      // flag air-punch
char limak , limak2 = 1;
signed char limp1 , limp2 ;
char nbpar = 0;
char vsynch = 0;
int delayer = 2;
/* character size info */
int sol1 = 0 , sol2 = 0; // INITIALIZING
int tol1 , tol2;
int jump_how1 , jump_how2;
int hitfram1 = 0, hitfram2 = 0; // INITIALIZING HIT FRAME
char female1 , female2;
// character animation info
int rush_stop1 , rush_stop2;
int smove_stop1 , smove_stop2;
int rushx_stop1 , rushx_stop2;
int smovex_stop1 , smovex_stop2;
char damag1 , damag2;
char fball1 , fball2;
int fb1x = 0 , fb2x = 0; // INITIALIZING FIREBALL X,Y COORDENATES
int fb1y = 0 , fb2y = 0; // INITIALIZING FIREBALL X,Y COORDENATES
int fball_cx1 = 0 , fball_cx2 = 0; // INITIALIZING FIREBALL X,Y COORDENATES
int fball_cy1 = 0 , fball_cy2 = 0; // INITIALIZING FIREBALL X,Y COORDENATES
char fball_flg1 = 0 , fball_flg2 = 0; // INITIALIZING FIREBALL X,Y COORDENATES
char fballx1 , fballx2;
int fbx1x = 0 , fbx2x = 0; // INITIALIZING FIREBALL X,Y COORDENATES
int fbx1y = 0 , fbx2y = 0; // INITIALIZING FIREBALL X,Y COORDENATES
int fballx_cx1 = 0 , fballx_cx2 = 0; // INITIALIZING FIREBALL X,Y COORDENATES
int fballx_cy1 = 0 , fballx_cy2 = 0; // INITIALIZING FIREBALL X,Y COORDENATES
char fballx_flg1 = 0 , fballx_flg2 = 0; // INITIALIZING FIREBALL FLAGS
char accel_flag , gfx_flag;
char snd_flag , midi_flag;
char wind_flag;
char tnt2 = 0;
char open1[ 40 ];
char open2[ 40 ];
char open3[ 40 ];
char open4[ 40 ];
char open5[ 40 ];
char title[ 20 ];
extern char say1[500] ;
extern char say2[500] ;

// END OF GLOBAL DEFINITIONS

// declara��es de variaveis para o KOF2003
BITMAP *tface1;				//p1 face pequena ao lado da barra
BITMAP *tface2;				//p2 face pequena ao lado da barra

BITMAP *tface_border1;                  // 
BITMAP *tface_border2;


char file2[200];			//var para carregar pcx's
//char buffer[200];			//var tb para carregar pcx's

int opt;				//int para realizar compara��es
int lineprint;

void man( void );
char get_kstatus ( char strg[] );
char check_kstatus1 ( char ctrl_key );
char check_kstatus2 ( char ctrl_key );
void Intro( void );

#endif

// EOF
