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
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "global.h"
#include "macros.h"
#include "config.h"
#include "display.h" 
#include "monitor.h"

/***************************************************************************
          ***** CHECK AND UPDATE LIFEBARS ******
****************************************************************************/
#if GAME_DEBUG > 0
int cicles=0 ;  // cicles
#endif

int slowdown, slowdown_end_round;
BITMAP *disp_up;
BITMAP *disp_down;
int up_x, down_x;

int display_up;
int display_down;
int up_h ;
int down_h ;

int up_x ;
int down_x ;

int p1_name;
int p2_name;

int p1_name_x ;
int p1_name_y ;

int p2_name_x ;
int p2_name_y ;

/*Read border of lifebar parameters*/
int p1_lifebar_border ;
int p2_lifebar_border ;

int p1_lifebar_border_x ;
int p2_lifebar_border_x ;

int p1_lifebar_border_y ;
int p2_lifebar_border_y ;

/*Read border parameters*/
int p1_lifebar ;
int p2_lifebar ;

int p1_lifebar_x ;
int p2_lifebar_x ;

int p1_lifebar_y ;
int p2_lifebar_y ;

/*LIFE FACE*/

int p1_life_face ;
int p1_life_face_x ;
int p1_life_face_y ;

int p2_life_face ;
int p2_life_face_x ;
int p2_life_face_y ;
/* Life Face BORDER*/
int p1_life_face_border ;
int p1_life_face_border_x ;
int p1_life_face_border_y ;
                                                                                
int p2_life_face_border ;
int p2_life_face_border_x ;
int p2_life_face_border_y ;


/*VICTORY FLAGS*/
int p1_victory_flag ;
int p1_victory_flag_x ;
int p1_victory_flag_y ;

int p2_victory_flag ;
int p2_victory_flag_x ;
int p2_victory_flag_y ;

/*BORDER OF POWERBAR*/
int p1_powerbar_border;
int p2_powerbar_border;

int p1_powerbar_border_x;
int p1_powerbar_border_y;

int p2_powerbar_border_x;
int p2_powerbar_border_y;

/* POWERBAR */
int p1_powerbar;
int p2_powerbar;

int p1_powerbar_x;
int p1_powerbar_y;

int p2_powerbar_x;
int p2_powerbar_y;

#if TIMEOVER > 0 
/*BORDER OF POWERBAR*/
int timer_border;

int timer_border_x;
int timer_border_y;

/* TIMER */
int timer_r;
int timer_l;

int timer_r_x;
int timer_r_y;

int timer_l_x;
int timer_l_y;

int timer_c_x;
int timer_c_y;

int timer_center;

int t_clock;
int t_clock_ini;
int t_short;
int t_decrement;
int clock_frame;
char s_cron[] = "99999";
int c_dez, c_uni;
#endif

/*monitor special variables*/

char name_vars[DEF_MAX_MONITOR_VARS][40];

int *addr_vars[DEF_MAX_MONITOR_VARS];

int user_vars[DEF_MAX_USER_VARS];

int current_var;
int current_user_var;

/******************************************/
void disp_ini_var( void ){
	current_var=0;
	current_user_var=0;
}
/******************************************/
void disp_put_var( char name_variable[40], int *pvar )
{
	sprintf(name_vars[current_var], "%s", name_variable);
	addr_vars[current_var] = pvar;
	current_var++;
#if GAME_DEBUG > 0
	if (current_var > DEF_MAX_MONITOR_VARS)
	{
	   sprintf(log_mess," IN DEF_MAX_MONITOR_VARS:%d change config.h",
	           DEF_MAX_MONITOR_VARS ); 
           ERROR(log_mess); 
	}
#endif	
		
}
/*****************************************/
#if GAME_DEBUG > 0
void show_fpc( void ){
if (cicles++ > DEF_MAX_CICLES) cicles = 0;
    sprintf(log_mess,"cicles: %d ",cicles);
    text_shadow( screen, font, log_mess, 0, 0, makecol( 255,255,255 ) );
}
#endif	
/*****************************************/
void displays( void )
{
	if ( display_up )
		draw_sprite( virtscreen, disp_up, 0, up_x );
	if ( display_down )
		draw_sprite( virtscreen, disp_down, 0, down_x );
}
/*****************************************/
void initdisplay( void )
{
	int up_h, down_h;
	char temp[ 4 ];
        TRON("ak1");
	override_config_file( "game.ini" );
	set_config_file( "game.ini" );
	/*Read heigths of up and down displays*/
	/*****************************************************************/

        TRON("ak2");
	disp_ini_var(); // Initializing monitor variables table...
        TRON("ak3");
	
#if TIMEOVER > 0
//        disp_put_var("t_clock",&t_clock); // put t_clock variable in monitor system
	
	slowdown = xget_config_int( "adjust","slowdown",1);
        disp_put_var("slowdown",&slowdown);
        TRON("ak4");
        slowdown_end_round = xget_config_int( "adjust","slowdown_end_round",1);
        disp_put_var("slowdown_end_round",&slowdown_end_round);
        TRON("AK5");

	t_clock_ini = xget_config_int( "timer", "t_clock", 0 );
	disp_put_var("t_clock_ini",&t_clock_ini);
	 
	t_decrement = xget_config_int( "timer", "t_decrement", 0 );
        disp_put_var("t_decrement",&t_decrement);
	
	t_short = xget_config_int( "timer", "t_short", 0 );
	disp_put_var("t_short",&t_short);

	sprintf( s_cron, "%d", t_clock_ini );
#endif 
	/*--------------------------------------------------------------*/
	sprintf( temp, "%s",
			 xget_config_string( "display", "up", "on" ) );

	if ( strcmp( temp, "off" ) )
		display_up = 1;
	else
		display_up = 0;
	
	disp_put_var("up",&display_up); 
	
	/*--------------------------------------------------------------*/
	sprintf( temp , "%s",
			 xget_config_string( "display", "down", "on" ) );

	if ( strcmp( temp, "off" ) )
		display_down = 1;
	else
		display_down = 0;

	disp_put_var("down",&display_down);
	/*--------------------------------------------------------------*/

	up_h = xget_config_int( "display", "up_h", 50 );
	disp_put_var("up_h",&up_h);
	down_h = xget_config_int( "display", "down_h", 25 );
	disp_put_var("down_h",&down_h);

	up_x = xget_config_int( "display", "up_x", 0 );
        disp_put_var("up_x",&up_x);
		
	down_x = xget_config_int( "display", "down_x", 175 );
        disp_put_var("down_x",&down_x);
		
	/* name parameters
	   p?1_name:
	   0 - none
	   1 - up display
	   2 - down display
	*/

	p1_name = xget_config_int( "display", "p1_name", 1 );
	disp_put_var("p1_name",&p1_name);
	
	p2_name = xget_config_int( "display", "p2_name", 1 );
	disp_put_var("p2_name",&p2_name);

	p1_name_x = xget_config_int( "display", "p1_name_x", 50 );
        disp_put_var("p1_name_x",&p1_name_x);
	
	p1_name_y = xget_config_int( "display", "p1_name_y", 1 );
	disp_put_var("p1_name_y",&p1_name_y);
		
	p2_name_x = xget_config_int( "display", "p2_name_x", 195 );
	disp_put_var("p2_name_x",&p2_name_x);
	
	p2_name_y = xget_config_int( "display", "p2_name_y", 1 );
	disp_put_var("p2_name_y",&p2_name_y);

	/*Read border of lifebar parameters
	   p?_lifebar_border:
	   0 - none
	   1 - up display
	   2 - down display
	*/
	p1_lifebar_border = xget_config_int( "display", "p1_lifebar_border", 1 );
	disp_put_var("p1_lifebar_border",&p1_lifebar_border);

	p2_lifebar_border = xget_config_int( "display", "p2_lifebar_border", 0 );
	disp_put_var("p2_lifebar_border",&p2_lifebar_border);

	p1_lifebar_border_x = xget_config_int( "display", "p1_lifebar_border_x", 0 );
	disp_put_var("p1_lifebar_border_x",&p1_lifebar_border_x);
	
	p2_lifebar_border_x = xget_config_int( "display", "p2_lifebar_border_x", 0 );
	disp_put_var("p2_lifebar_border_x",&p2_lifebar_border_x);

	p1_lifebar_border_y = xget_config_int( "display", "p1_lifebar_border_y", 0 );
	  
	disp_put_var("p1_lifebar_border_y",&p1_lifebar_border_y);
	
	p2_lifebar_border_y = xget_config_int( "display", "p2_lifebar_border_y", 0 );
	disp_put_var("p2_lifebar_border_y",&p2_lifebar_border_y);

	/*Read border parameters
	   p?_lifebar:
	   0 - none
	   1 - up display
	   2 - down display
	*/

	p1_lifebar = xget_config_int( "display", "p1_lifebar", 1 );
	disp_put_var("p1_lifebar",&p1_lifebar);
	p2_lifebar = xget_config_int( "display", "p2_lifebar", 1 );
        disp_put_var("p2_lifebar",&p2_lifebar);
	
	/*20 + ( 125 - ( life1/4 ) ) = 20 */
	p1_lifebar_x = xget_config_int( "display", "p1_lifebar_x", 20 );
	disp_put_var("p1_lifebar_x",&p1_lifebar_x);
	p1_lifebar_y = xget_config_int( "display", "p1_lifebar_y", 12 );
        disp_put_var("p1_lifebar_y",&p1_lifebar_y);
	
	p2_lifebar_x = xget_config_int( "display", "p2_lifebar_x", 175 );
	disp_put_var("p2_lifebar_x",&p1_lifebar_x);
	
	p2_lifebar_y = xget_config_int( "display", "p2_lifebar_y", 12 );
        disp_put_var("p2_lifebar_y",&p2_lifebar_y);

	/* BORDER OF POWERBAR */
	p1_powerbar_border = xget_config_int( "display", "p1_powerbar_border", 2 );
	disp_put_var("p1_powerbar_border",&p1_powerbar_border);
	
	p2_powerbar_border = xget_config_int( "display", "p2_powerbar_border", 2 );
	disp_put_var("p2_powerbar_border",&p2_powerbar_border);

	p1_powerbar_border_y = xget_config_int( "display", "p1_powerbar_border_y", 0 );
	disp_put_var("p1_powerbar_border_y",&p1_powerbar_border_y);
	
	p1_powerbar_border_x = xget_config_int( "display", "p1_powerbar_border_x", 0 );
	disp_put_var("p1_powerbar_border_x",&p1_powerbar_border_x);

	p2_powerbar_border_y = xget_config_int( "display", "p2_powerbar_border_y", 0 );
	disp_put_var("p2_powerbar_border_y",&p2_powerbar_border_y);
	
	p2_powerbar_border_x = xget_config_int( "display", "p2_powerbar_border_x", 253 );
	disp_put_var("p2_powerbar_border_x",&p2_powerbar_border_x);

	/*LIFE FACE*/
	p1_life_face=xget_config_int( "display", "p1_life_face", 1 );
        disp_put_var("p1_life_face",&p1_life_face);
	p1_life_face_x=xget_config_int( "display", "p1_life_face_x", 1);
        disp_put_var("p1_life_face_x",&p1_life_face_x);
	p1_life_face_y=xget_config_int( "display", "p1_life_face_y", 24 );
        disp_put_var("p1_life_face_y",&p1_life_face_y);
       
        p2_life_face=xget_config_int( "display", "p2_life_face", 1 );
        disp_put_var("p2_life_face",&p2_life_face);
        p2_life_face_x=xget_config_int( "display", "p2_life_face_x", 295 );
        disp_put_var("p2_life_face_x",&p2_life_face_x);
        p2_life_face_y=xget_config_int( "display", "p2_life_face_y", 24 );
        disp_put_var("p2_life_face_y",&p2_life_face_y);

        /* LIFE FACE BORDER */
        p1_life_face_border=xget_config_int( "display", "p1_life_face_border", 0 );
        disp_put_var("p1_life_face_border",&p1_life_face_border);
        p1_life_face_border_x=xget_config_int( "display", "p1_life_face_border_x", 1);
        disp_put_var("p1_life_face_border_x",&p1_life_face_border_x);
        p1_life_face_border_y=xget_config_int( "display", "p1_life_face_border_y", 25);
        disp_put_var("p1_life_face_border_y",&p1_life_face_border_y);

        p2_life_face_border=xget_config_int( "display", "p2_life_face_border", 0 );
        disp_put_var("p2_life_face_border",&p2_life_face_border);
        p2_life_face_border_x=xget_config_int( "display", "p2_life_face_border_x", 290);
        disp_put_var("p2_life_face_border_x",&p2_life_face_border_x);
        p2_life_face_border_y=xget_config_int( "display", "p2_life_face_border_y", 25);
        disp_put_var("p2_life_face_border_y",&p2_life_face_border_y);
                                                                                
	/*VICTORY FLAGS*/

	p1_victory_flag=xget_config_int( "display", "p1_victory_flag", 1 );
	disp_put_var("p1_victory_flag",&p1_victory_flag);
	p1_victory_flag_x=xget_config_int( "display", "p1_victory_flag_x", 26 );
	disp_put_var("p1_victory_flag_x",&p1_victory_flag_x);
	p1_victory_flag_y=xget_config_int( "display", "p1_victory_flag_y", 34 );
	disp_put_var("p1_victory_flag_y",&p1_victory_flag_y);

	
	p2_victory_flag=xget_config_int( "display", "p2_victory_flag", 1 );
	disp_put_var("p2_victory_flag",&p2_victory_flag);
	p2_victory_flag_x=xget_config_int( "display", "p2_victory_flag_x", 279);
	disp_put_var("p2_victory_flag_x",&p2_victory_flag_x);
	p2_victory_flag_y=xget_config_int( "display", "p2_victory_flag_y", 34 );
	disp_put_var("p2_victory_flag_y",&p2_victory_flag_y);
	
	/* POWERBAR */
	p1_powerbar = xget_config_int( "display", "p1_powerbar", 2 );
	disp_put_var("p1_powerbar",&p1_powerbar);
	
	p2_powerbar = xget_config_int( "display", "p2_powerbar", 2 );
	disp_put_var("p2_powerbar",&p2_powerbar);
	
	p1_powerbar_x = xget_config_int( "display", "p1_powerbar_x", 3 );
	disp_put_var("p1_powerbar_x",&p1_powerbar_x);
	
	p1_powerbar_y = xget_config_int( "display", "p1_powerbar_y", 18 );
	disp_put_var("p1_powerbar_y",&p1_powerbar_y);

	p2_powerbar_x = xget_config_int( "display", "p2_powerbar_x", 256 );
	disp_put_var("p2_powerbar_x",&p2_powerbar_x);
	
	p2_powerbar_y = xget_config_int( "display", "p2_powerbar_y", 18 );
        disp_put_var("p2_powerbar_y",&p2_powerbar_y);
      

#if TIMEOVER > 0
	/* BORDER OF TIMER*/
	timer_border = xget_config_int( "display", "timer_border", 1 );
	disp_put_var( "timer_border",&timer_border );

	timer_border_x = xget_config_int( "display", "timer_border_x", 147 );
        disp_put_var( "timer_border_x",&timer_border_x );
	
	timer_border_y = xget_config_int( "display", "timer_border_y", 22 );
	disp_put_var( "timer_border_y",&timer_border_y );

	/* TIMER */
	timer_r = xget_config_int( "display", "timer_r", 1 );
        disp_put_var( "timer_r",&timer_r );
	
	timer_l = xget_config_int( "display", "timer_l", 1 );
	disp_put_var( "timer_l",&timer_l );

	timer_r_x = xget_config_int( "display", "timer_r_x", 162 );
        disp_put_var( "timer_r_x",&timer_r_x );
	
	timer_r_y = xget_config_int( "display", "timer_r_y", 22 );
	disp_put_var( "timer_r_y",&timer_r_y);

	timer_l_x = xget_config_int( "display", "timer_l_x", 147 );
        disp_put_var( "timer_l_x",&timer_l_x);
	
	timer_l_y = xget_config_int( "display", "timer_l_y", 22 );
        disp_put_var( "timer_l_y",&timer_l_y);

	sprintf( temp, "%s",
           xget_config_string( "display", "timer_center", "on" ) );

	if ( strcmp( temp, "off" ) )
		timer_center = 1;
	else
		timer_center = 0;
	disp_put_var( "timer_center",&timer_center);

	timer_c_x = timer_l_x + ( ( timer_r_x - timer_l_x ) / 2 );
	timer_c_y = timer_l_y + ( ( timer_r_y - timer_l_y ) / 2 );
#endif

	/***************************/
	disp_up = create_bitmap( 320, up_h );
	disp_down = create_bitmap( 320, down_h );
	fill_sprite( disp_up );
	fill_sprite( disp_down );

#if GAME_DEBUG > 0
	rect( disp_up, 0, 0, 319 , disp_up->h-1, makecol( 255, 0, 0 ) );
	rect( disp_down, 0, 0, 319 , disp_down->h-1, makecol( 0, 0, 255 ) );
#endif

	if ( p1_name > 0 ){
	      text_shadow( ( p1_name == 1 ? disp_up : disp_down ),
                     small_font, charn1, p1_name_x, p1_name_y,
					 sel_player1 );
 
              #if GAME_DEBUG > 0
                 rect(p1_name == 1 ? disp_up : disp_down,p1_name_x,
                     p1_name_y,p1_name_x+8,p1_name_y+8,
                        makecol(255,255,255) );
             #endif

        }
	if ( p2_name > 0 ){
		text_shadow( ( p2_name == 1 ? disp_up : disp_down ),
                     small_font,  charn2, p2_name_x, p2_name_y,
					 sel_player2 );

        }
	refresh_lifebar();
	refresh_powerbar();
        TRON("KO");
	// claudemir
	refresh_life_faces();
        TRON("k2");

#if TIMEOVER > 0
	refresh_timer();
#endif
        TRON("K");

	displays();
        TRON("1");
#if CHEAT == 1	
	disp_put_var( "p1_power",&spower1);
        TRON("1");

	disp_put_var( "p2_power",&spower2);
        TRON("1");

        disp_put_var( "p1_life",&life1);
        TRON("1");
 
       disp_put_var( "p2_life",&life2);
        TRON("1");

        disp_put_var("t_clock",&t_clock);	
        TRON("1");

	disp_put_var("actual",&actual);
        TRON("1");

	disp_put_var("actual2",&actual2);
        TRON("1");

#endif
       	run_script( 0, fight_round, charname[sel1], charname[sel2] );
        TRON("1");

}

/**************************************/
void refresh_lifebar( void )
{

	/*CLEAR PRINT AREA OF LIFEBAR*/
/*	if ( p1_lifebar > 0 )
	{
		rectfill( ( p1_lifebar == 1 ? disp_up : disp_down ),
				  p1_lifebar_x,
				  p1_lifebar_y,
				  p1_lifebar_x + 127,
				  p1_lifebar_y + 7, makecol( 255, 0, 255 ) );
	}
	if ( p2_lifebar > 0 )
	{
		rectfill( ( p2_lifebar == 1 ? disp_up : disp_down ),
				  p2_lifebar_x,
				  p2_lifebar_y,
				  p2_lifebar_x + 127,
				  p2_lifebar_y + 7, makecol( 255, 0, 255 ) );
	}
*/
	/*PRINT BORDERS OF LIFEBARS*/
	if ( p1_lifebar_border > 0 )
	{
		draw_sprite( ( p1_lifebar_border == 1 ? disp_up : disp_down ),
					 LBbanner,
					 p1_lifebar_border_x,
					 p1_lifebar_border_y );
	}

	if ( p2_lifebar_border > 0 )
	{
		draw_sprite( ( p2_lifebar_border == 1 ? disp_up : disp_down ),
					 LBbanner2,
					 p2_lifebar_border_x ,
					 p2_lifebar_border_y );
	}


	/*PRINT LIFEBARS*/
	if ( p1_lifebar > 0 )
		masked_blit( Bar, ( p1_lifebar == 1 ? disp_up : disp_down ),
			  ( 125 - ( life1 / 4 ) ),
			  0,
			  p1_lifebar_x + ( 125 - ( life1 / 4 ) ),
			  p1_lifebar_y,
			  ( life1 / 4 ) + 2,
			  Bar->h );

	if ( p2_lifebar > 0 )
        {
		masked_blit( Bar2, ( p2_lifebar == 1 ? disp_up : disp_down ),
			  0,
			  0,
			  p2_lifebar_x,
			  p2_lifebar_y,
			  ( life2 / 4 ) + 2,
			  Bar2->h );
                
        }
}

/*******************************************************/
void refresh_powerbar( void )
{
         fill_sprite( disp_down );
#if GAME_DEBUG > 0
        rect( disp_down, 0, 0, 319 , disp_down->h-1, makecol( 0, 0, 255 ) );
#endif

	/*PRINT BORDERS OF POWERBAR*/
	if ( p1_powerbar_border > 0 )
		draw_sprite( ( p1_powerbar_border == 1 ? disp_up : disp_down ), Power ,
					 p1_powerbar_border_x, p1_powerbar_border_y );

	if ( p2_powerbar_border > 0 )
		draw_sprite( ( p2_powerbar_border == 1 ? disp_up : disp_down ), Power ,
					 p2_powerbar_border_x, p2_powerbar_border_y );

	/*PRINT POWERBARS*/
	if ( p1_powerbar > 0 )
		blit ( PBar, ( p1_powerbar == 1 ? disp_up : disp_down ),
			   0, 0, p1_powerbar_x, p1_powerbar_y, spower1, 6 );

	if ( p2_powerbar > 0 )
		blit ( PBar, ( p2_powerbar == 1 ? disp_up : disp_down ),
			   0, 0, p2_powerbar_x, p2_powerbar_y, spower2, 6 );
}

/*************************************************************/
#if TIMEOVER > 0
void refresh_timer( void )
{
     timer_border = xget_config_int("display","timer_border",1);
	 timer_border_x = xget_config_int("display","timer_border_x",147);
	 timer_border_y = xget_config_int("display","timer_border_y",22);


#if GAME_DEBUG >= 2	
	TRON( "CHECK CLOCK" );
#endif	
	if ( clock_frame++ >= t_decrement )
	{
        	if ( timer_border > 0 )
	           draw_sprite( ( timer_border == 1 ? disp_up : disp_down ), B_clock[ 10 ] ,
			 timer_border_x, timer_border_y );
		
		clock_frame = 0;

		if ( t_clock-- <= 1 )
		{
			if ( life1 < life2 )
				life1 = 0;
			else
				life2 = 0;
		}

		sprintf( s_cron, "%d", t_clock );

		c_dez = s_cron[ 0 ] - 48;
		c_uni = s_cron[ 1 ] - 48;

		if ( ( c_dez < 0 ) || ( c_dez > 9 ) )
			c_dez = 0;
		if ( ( c_uni < 0 ) || ( c_uni > 9 ) )
			c_uni = 0;
	

#if GAME_DEBUG >= 2
            sprintf( log_mess, "t_clock: %d | {%d}{%d}", t_clock, c_dez, c_uni );
	    TRON( log_mess );
#endif
	
	    if ( t_clock <= t_short )
	    {
	  	set_trans_blender( 255, 0, 0, 100 );
	    }else{
		set_trans_blender( 255, 255, 255, 0 );
            }
	    if ( timer_center && ( t_clock < 10 ) )
	    {
		if ( timer_r > 0 )
			draw_lit_sprite( ( timer_r == 1 ? disp_up : disp_down ), B_clock[ c_dez ],
							 timer_c_x, timer_c_y, 100 );
	    }else{
		if ( timer_l > 0 )
			draw_lit_sprite( ( timer_l == 1 ? disp_up : disp_down ), B_clock[ c_dez ],
							 timer_l_x, timer_l_y, 100 );
		if ( timer_r > 0 )
			draw_lit_sprite( ( timer_r == 1 ? disp_up : disp_down ), B_clock[ c_uni ],
							 timer_r_x, timer_r_y, 100 );
	    }
	}
}

#endif 
/*************************************************************/
void start_timer(void){
                sprintf( s_cron, "%d", t_clock );
                                                                                
                c_dez = s_cron[ 0 ] - 48;
                c_uni = s_cron[ 1 ] - 48;
                                                                                
                if ( ( c_dez < 0 ) || ( c_dez > 9 ) )
                        c_dez = 0;
                if ( ( c_uni < 0 ) || ( c_uni > 9 ) )
                        c_uni = 0;

                if ( timer_border > 0 )
                   draw_sprite( ( timer_border == 1 ? disp_up : disp_down ), B_clock[ 10 ] ,
                         timer_border_x, timer_border_y );

            if ( timer_center && ( t_clock < 10 ) )
            {
                if ( timer_r > 0 )
                        draw_lit_sprite( ( timer_r == 1 ? disp_up : disp_down ), B_clock[ c_dez ],
                                                         timer_c_x, timer_c_y, 100 );
            }else{
                if ( timer_l > 0 )
                        draw_lit_sprite( ( timer_l == 1 ? disp_up : disp_down ), B_clock[ c_dez ],
                                                         timer_l_x, timer_l_y, 100 );
                if ( timer_r > 0 )
                        draw_lit_sprite( ( timer_r == 1 ? disp_up : disp_down ), B_clock[ c_uni ],
                                                         timer_r_x, timer_r_y, 100 );
            }
}


/* Print centred text with shadow */

void text_centre_shadow( BITMAP *outbm, FONT *font,
		 char *text, int x, int y, int color )
{
	char dc[255];
        char outtext[255];
	char ch;
	int i;
        sprintf(dc,"%s",text);
        sprintf(outtext,"%s",text);
 
	for(i=0;i<strlen(text)-2;i++){
            ch=text[i];
                        
            dc[i]=' '; // Put a space..
            
            
            if(ch==K_aacute){outtext[i]='a'; dc[i]='\''; }
	    if(ch==K_eacute){outtext[i]='e'; dc[i]='\''; }
	    if(ch==K_iacute){outtext[i]='i'; dc[i]='\''; }
	    if(ch==K_oacute){outtext[i]='o'; dc[i]='\''; }
	    if(ch==K_uacute){outtext[i]='u'; dc[i]='\''; }
	    
	    if(ch==K_atilde){outtext[i]='a'; dc[i]='~'; }
	    if(ch==K_otilde){outtext[i]='o'; dc[i]='~'; }
	    
	    if(ch==K_agrave){outtext[i]='a'; dc[i]='`'; }
	    if(ch==K_egrave){outtext[i]='e'; dc[i]='`'; }
	    if(ch==K_igrave){outtext[i]='i'; dc[i]='`'; }
	    if(ch==K_ograve){outtext[i]='o'; dc[i]='`'; }
	    if(ch==K_ugrave){outtext[i]='u'; dc[i]='`'; }

	    if(ch==K_auml){outtext[i]='a'; dc[i]='"'; }
	    if(ch==K_euml){outtext[i]='e'; dc[i]='"'; }
	    if(ch==K_iuml){outtext[i]='i'; dc[i]='"'; }
	    if(ch==K_ouml){outtext[i]='o'; dc[i]='"'; }
	    if(ch==K_uuml){outtext[i]='u'; dc[i]='"'; }
	    
            if(ch==K_acirc){outtext[i]='a'; dc[i]='^'; }
	    
	    if(ch==K_ecirc){outtext[i]='e'; dc[i]='^'; }
            if(ch==K_icirc){outtext[i]='i'; dc[i]='^'; }
	    if(ch==K_ocirc){outtext[i]='o'; dc[i]='^'; }
            if(ch==K_ucirc){outtext[i]='u'; dc[i]='^'; }
    	    if(ch==K_ccedil){outtext[i]='c'; dc[i]=','; }
	    if(ch==K_ntilde){outtext[i]='n'; dc[i]='~'; }

	    if(ch==K_Aacute){outtext[i]='A'; dc[i]='\''; }
	    if(ch==K_Eacute){outtext[i]='E'; dc[i]='\''; }
	    if(ch==K_Iacute){outtext[i]='I'; dc[i]='\''; }
	    if(ch==K_Oacute){outtext[i]='O'; dc[i]='\''; }
	    if(ch==K_Uacute){outtext[i]='U'; dc[i]='\''; }
	    
	    if(ch==K_Atilde){outtext[i]='A'; dc[i]='~'; }
	    if(ch==K_Otilde){outtext[i]='O'; dc[i]='~'; }
	    
	    if(ch==K_Agrave){outtext[i]='A'; dc[i]='`'; }
	    if(ch==K_Egrave){outtext[i]='E'; dc[i]='`'; }
	    if(ch==K_Igrave){outtext[i]='I'; dc[i]='`'; }
	    if(ch==K_Ograve){outtext[i]='O'; dc[i]='`'; }
	    if(ch==K_Ugrave){outtext[i]='U'; dc[i]='`'; }

	    
	    if(ch==K_Auml){outtext[i]='A'; dc[i]='"'; }
	    if(ch==K_Euml){outtext[i]='E'; dc[i]='"'; }
	    if(ch==K_Iuml){outtext[i]='I'; dc[i]='"'; }
	    if(ch==K_Ouml){outtext[i]='O'; dc[i]='"'; }
	    if(ch==K_Uuml){outtext[i]='U'; dc[i]='"'; }

	    if(ch==K_Acirc){outtext[i]='A'; dc[i]='^'; }
	    if(ch==K_Ecirc){outtext[i]='E'; dc[i]='^'; }
            if(ch==K_Icirc){outtext[i]='I'; dc[i]='^'; }
	    if(ch==K_Ocirc){outtext[i]='O'; dc[i]='^'; }
            if(ch==K_Ucirc){outtext[i]='U'; dc[i]='^'; }
	    
	    if(ch==K_Ccedil){outtext[i]='C'; dc[i]=',';}

	    if(ch==K_Ntilde){outtext[i]='N'; dc[i]='~';}
            
//	    sprintf(log_mess,"outtext[i]=%c i=%d",outtext[i],i);
//            TRON(log_mess);
	}

	textout_centre( outbm, font, outtext, x + 1, y + 1, 0 );
        textout_centre( outbm, font, dc,   x + 1, y + 1, 0 );
	textout_centre( outbm, font, outtext, x, y, color );
        textout_centre( outbm, font, dc, x, y, color );
         
}
/********************************************/
/* Print centred text shadow */

void text_shadow( BITMAP *outbm, FONT *font,
				  char *text, int x, int y, int color )
{
	textout( outbm, font, text, x + 1, y + 1, 0 );
	textout( outbm, font, text, x, y, color );
}
/********************************************************/
void refresh_vflags( void )
{
#if GAME_DEBUG >= 2
	TRON( "VICTORY FLAGS" );
#endif
	
   if (vic_flag>20){
      draw_sprite( p1_victory_flag == 1?disp_up:disp_down, vic1?avic1:novic1 ,p1_victory_flag_x ,p1_victory_flag_y ) ;	
      draw_sprite( p2_victory_flag == 1?disp_up:disp_down, vic2?avic1:novic1 ,p2_victory_flag_x ,p2_victory_flag_y ) ;
   }else{
      draw_sprite( p1_victory_flag == 1?disp_up:disp_down, vic1?avic0:novic0 ,p1_victory_flag_x,p1_victory_flag_y ) ;
      draw_sprite( p2_victory_flag == 1?disp_up:disp_down, vic2?avic0:novic0 ,p2_victory_flag_x ,p2_victory_flag_y ) ;
   }
}
/********************************************************/
// Claudemir
// função para desenhar o rosto no lado da barra de life
void refresh_life_faces (void)
{
       TRON("q"); 
       if (p1_life_face_border>0){
          draw_sprite(p1_life_face_border == 1?disp_up:disp_down, tface_border1, p1_life_face_border_x,p1_life_face_border_y);
       }
              
       TRON("A");                                                                  
       if (p2_life_face_border>0){
         draw_sprite(p2_life_face_border == 1?disp_up:disp_down, tface_border2, p2_life_face_border_x,p2_life_face_border_y);
       }
       TRON("z");
       if (p1_life_face>0){	
	  draw_sprite(p1_life_face == 1?disp_up:disp_down, tface1, p1_life_face_x,p1_life_face_y);
       }

       TRON("w");
       if (p2_life_face>0){
	 draw_sprite(p2_life_face == 1?disp_up:disp_down, tface2, p2_life_face_x,p2_life_face_y); 
       }
       TRON("s");
}

/********************************************************/
int xget_config_int( char *section, char *var_name, int val_defa )
{
	int val_ret = 0;
	char exist[6] = "xxx";

    const char * str = get_config_string( section, var_name, "xxx" );
    sprintf( exist, "%s", str );
	if ( !strcmp( exist, "xxx" ) )
	{
		val_ret = val_defa;
		set_config_int( section, var_name, val_ret );
	} else{
	        val_ret = get_config_int( section, var_name, 0 );
	}
	
	set_config_file( "game.ini" );
	return val_ret;
}
/********************************************************/
char *xget_config_string( char *section, char *var_name, char *val_defa )
{
	char sval_ret[ 6 ] = "xxx";

	/* sprintf( sval_ret,"%s", "xxx" ); */
	sprintf( sval_ret, "%s", get_config_string( section, var_name, "xxx" ) );
	if ( !strcmp( sval_ret, "xxx" ) )
	{
		sprintf( sval_ret, "%s", val_defa );
		set_config_string( section, var_name, sval_ret );
	}
	set_config_file( "game.ini" );
	return sval_ret;
}
/****************************************************************************/
void save_display( void )
{
	char temp[ 4 ];
	if ( display_up == 1 )
		sprintf( temp, "on" );
	else
		sprintf( temp, "off" );
	set_config_string ( "display", "up" , temp );

	if ( display_down == 1 )
		sprintf( temp, "on" );
	else
		sprintf( temp, "off" );
	set_config_string ( "display", "down" , temp );

	set_config_int ( "display", "up_h" , up_h );
	set_config_int ( "display", "down_h" , down_h );
	set_config_int ( "display", "up_x", up_x );
	set_config_int ( "display", "down_x", down_x );

	set_config_int ( "display", "p1_name", p1_name );
	set_config_int ( "display", "p2_name", p2_name );

	set_config_int ( "display", "p1_name_x", p1_name_x );
	set_config_int ( "display", "p2_name_x", p2_name_x );

	set_config_int ( "display", "p1_name_y", p1_name_x );
	set_config_int ( "display", "p2_name_y", p2_name_x );

	set_config_int ( "display", "p1_lifebar_border", p1_lifebar_border );
	set_config_int ( "display", "p2_lifebar_border", p2_lifebar_border );

	set_config_int ( "display", "p1_lifebar_border_x", p1_lifebar_border_x );
	set_config_int ( "display", "p2_lifebar_border_y", p2_lifebar_border_y );

	set_config_int ( "display", "p1_lifebar", p1_lifebar );
	set_config_int ( "display", "p2_lifebar", p2_lifebar );

	set_config_int ( "display", "p1_lifebar_x", p1_lifebar_x );
	set_config_int ( "display", "p2_lifebar_x", p2_lifebar_x );

	set_config_int ( "display", "p1_lifebar_y", p1_lifebar_y );
	set_config_int ( "display", "p2_lifebar_y", p2_lifebar_y );

	set_config_int ( "display", "p1_lifebar_border", p1_lifebar_border );
	set_config_int ( "display", "p2_lifebar_border", p2_lifebar_border );

	set_config_int ( "display", "p1_lifebar_border_x", p1_lifebar_border_x );
	set_config_int ( "display", "p2_lifebar_border_x", p2_lifebar_border_x );

	set_config_int ( "display", "p1_lifebar_border_y", p1_lifebar_border_y );
	set_config_int ( "display", "p2_lifebar_border_y", p2_lifebar_border_y );

	set_config_int ( "display", "p1_powerbar", p1_powerbar );
	set_config_int ( "display", "p2_powerbar", p2_powerbar );

	set_config_int ( "display", "p1_powerbar_x", p1_powerbar_x );
	set_config_int ( "display", "p1_powerbar_y", p1_powerbar_y );

	set_config_int ( "display", "p2_powerbar_x", p2_powerbar_x );
	set_config_int ( "display", "p2_powerbar_y", p2_powerbar_y );

	set_config_int ( "display", "timer_r", timer_r );
	set_config_int ( "display", "timer_l", timer_l );

	set_config_int ( "display", "timer_r_x", timer_r_x );
	set_config_int ( "display", "timer_r_y", timer_r_y );

	set_config_int ( "display", "timer_l_x", timer_l_x );
	set_config_int ( "display", "timer_l_y", timer_l_y );

	set_config_int ( "display", "timer_l_y", timer_l_y );

	if ( timer_center == 1 )
		sprintf( temp, "on" );
	else
		sprintf( temp, "off" );
	set_config_string ( "display", "timer_center" , temp );

	set_config_int ( "display", "timer_border" , timer_border );

	set_config_int ( "display", "timer_border_x" , timer_border_x );
	set_config_int ( "display", "timer_border_y" , timer_border_y );

	set_config_file( "game.ini" );
}
/***************************************************************************/
void PUT_t_clock( int param )
{
  t_clock=param;
}

int GET_t_clock( void ) 
{

  return t_clock;
}
/***************************************************************************/
void K_listvars( char param[40] )
{
	int i;
	char linha[80];
	console_print( "Table of variables:" );
	for(i=0;i<current_var;i++)
	{
              if ( ( strlen(param)==0 ) || !strncmp( param, name_vars[i], strlen( param ) ) )
	      {
	 	sprintf(linha, "%s=%d", name_vars[i], *addr_vars[i]);
		console_print( linha );
	      }
	}
}

/******************************************/
void K_setvar( char namevar[40], char param[40], char param2[40] )
{
	int i;
	int i_param=0;
	     

	if( isdigit(param[0]) )
	{
         i_param = atoi( param );  	
	}else{
          for( i=0; i < current_var; i++)
	  {
             if ( !strncmp( param, name_vars[i], strlen( param )) 
	         && name_vars[i][ strlen( param )+1 ] == '\0') 
	     {
               i_param = *addr_vars[i];
	       break;
	     }
	  }
	}

 	for( i=0; i < current_var; i++)
	{
          if ( !strncmp( namevar, name_vars[i], strlen( namevar )) 
	      && name_vars[i][ strlen( namevar )+1 ] == '\0') 
	  {
             *addr_vars[i] = i_param;
	     break;
	  }
	}
}
/******************************************/
void K_defvar( char namevar[40] )
{
	int i, ok=1;

        for(i=0; i < current_var ;i++)
	   if( !strcmp( namevar, name_vars[i] ) ) ok=0;
		   
	if (ok==1)
	{
	   disp_put_var( namevar, &user_vars[current_user_var++] );
	   console_print( "Variable added OK." );
	}else{
	   sprintf( log_mess,"Error (%s) - Vaiable already exist !", namevar );
	   console_print( log_mess );
	   ERROR( log_mess );
	}
}
/**********************************************************************/
void adjusts( const char f_s )
{
  if(f_s==0)
   rest( slowdown );
  else if(f_s==1)
   rest(slowdown_end_round);
}
/**********************************************************************/

// EOF
