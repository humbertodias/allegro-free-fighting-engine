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
#include "allegro.h"
#include "config.h"
#include "logger.h"
#include "macros.h"
#include "wavsounds.h"
#include "initfiles.h"
#include "display.h"
#include "inits.h"
#include "refresh.h"
#include "startmenu.h"
#include "mp3.h"
#include "batle.h"
#include "global.h"
#include "main.h"

int main(int, char **)
{
	
	int loop = 0;
	story = 0;
	lineprint = 0 ;
	//   srand(time(NULL));
	TRONSTART( "\n  **WARNING: Initializing KOF2003..." );
	allegro_init(); // inicia a lib Allegro
	/*   set_window_title ("initialising engine ..."); */
	set_config_file ( "cfgdata.stp" );
	accel_flag = get_config_int( 0, "accel", 1 );
#if DEBUG_MODE == 2

	debug = get_config_int( 0, "dbug", 0 );
#endif

	mp3 = get_config_int( 0, "mp3", 1 );
	gfx_flag = get_config_int( 0, "gfmode", 0 );
	wind_flag = get_config_int( 0, "win", 0 );
	switch ( gfx_flag )
	{
			case 2 :
			// modo de video 320x240
			screen_height = 240;
			screen_width = 320;
			start_y = 20;
			break;
			case 0 :
			// modo padr�o de video 230x200
			screen_height = 200;
			screen_width = 320;
			start_y = 0;
			break;
			case 1 :
			// modo de video 320x400
			screen_height = 400;
			screen_width = 320;
			start_y = 0;
			break;
			case 4 :
			// ?
			case 3 :
			// modo de video 640x400
			screen_height = 400;
			screen_width = 640;
			start_y = 0;
			break;
			// adicionando o modo 800x600 !!
			// Bem... tem que ter uma m�quina poderosa e uma
			// boa placa de video. Pot�nciaaaaaa...
			case 5 :
			screen_height = 600;
			screen_width = 800;
			start_y = 0;
			break;
	} /* switch (gfx_flag) */
	gfmode = gfx_flag;
	snd_flag = get_config_int( 0, "snd_driver", 0 );
	midi_flag = get_config_int( 0, "midi_driver", 0 );
	midi_vol = ( 3 * get_config_int( 0, "midi_vol", 70 ) );

	set_snd_vol (( 3 * get_config_int( 0, "snd_vol", 40 ) ));

	override_config_file( "game.ini" );
	TRON( "title...1" );
	sprintf( title , "%s", get_config_string( 0, "title", " " ) );
	/*    title = get_config_string( 0, "title", " " );*/
	TRON( "title...2" );
	TRON( title );

	set_window_title ( title );

	sprintf( open1 , "%s",
			 get_config_string ( 0 , "open1" , " " ) );
	sprintf( open2 , "%s",
			 get_config_string ( 0 , "open2" , " " ) );
	sprintf( open3 , "%s",
			 get_config_string ( 0 , "open3" , " " ) );
	sprintf( open4 , "%s",
			 get_config_string ( 0 , "open4" , " " ) );
	sprintf( open5 , "%s",
			 get_config_string ( 0 , "open5" , " " ) );

	/*************************************/

	install_timer(); //habilita rotinas para timer
	install_keyboard(); //habilita as rotinas de teclado
	fonts_dat = load_datafile ( "fonts.dat" );
	if ( get_config_int( 0, "joy1", 0 ) )
	{
		// JOYSTICK INSTALL
		if ( install_joystick( JOY_TYPE_AUTODETECT ) != 0 )
		{
			set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
			sprintf( log_mess, "Error: initialising joystick\n%s\n",
					 allegro_error );
			ERROR( log_mess );

			return 1;
		}
		/*
		// make sure that we have a joystick
		if (joy_type == JOY_TYPE_NONE) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		              ERROR("Error: joystick not found\n");
		return 1;
		}
		*/
	}

	// bem, o planao � tirar esses cr�dito, horrivel no inicio da engine !
	// bem... tirado :)
	/*
	 allegro_message(
		"%s\n\n%s\n%s\n%s\n%s\n%s\n\n"
		"Basead in KOF91 engine v.1.2 by Moah (moah@online.fr)\n"
		"http://moah.online.fr/kof91\n\n"
		"kof2003 by Claudemir P. da Luz Jr.\n"
		"http://www.virtuallink.com.br/kof2003\n\n"
		"KOF2003 is FREEWARE and CANNOT BE SOLD\n"
		, title, open1, open2, open3, open4, open5 );
	*/

	/*
	//Ningu�m precisa ficar sabendo,certo !?
	if ( accel_flag )
		allegro_message( "\nUsing Accel.\n" );
	*/

#if GAME_DEBUG > 0
	sprintf( log_mess, "GFX: %d ...", BR_GFX );
#endif
	TRON( log_mess );
#if SO==MSVCPP
	set_color_depth( 16 );
	if ( wind_flag )
	{
		if ( set_gfx_mode( GFX_DIRECTX_WIN, screen_width, screen_height, 0, 0 ) != 0 )
		{
			set_color_depth( 15 ) ;
			if ( set_gfx_mode( GFX_DIRECTX_WIN, screen_width, screen_height, 0, 0 ) != 0 )
			{
				set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
				allegro_message( "Error setting graphics mode\n%s\nTry a different video mode !\n", allegro_error );
				return 1;
			}
		}
	}
	else
		if ( accel_flag )
		{
			if ( set_gfx_mode( GFX_DIRECTX_ACCEL, screen_width, screen_height, 0, 0 ) != 0 )
			{
				set_color_depth( 15 );
				if ( set_gfx_mode( GFX_DIRECTX_ACCEL, screen_width, screen_height, 0, 0 ) != 0 )
				{
					set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
					allegro_message( "Error setting graphics mode\n%s\nTry a different video mode !\n", allegro_error );
					return 1;
				}
			}
		}
		else
		{
			if ( set_gfx_mode( GFX_DIRECTX_SOFT, screen_width, screen_height, 0, 0 ) != 0 )
			{
				set_color_depth ( 15 ) ;
				if ( set_gfx_mode( GFX_DIRECTX_SOFT, screen_width, screen_height, 0, 0 ) != 0 )
				{
					set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
					allegro_message( "Error setting graphics mode\n%s\nTry a different video mode !\n", allegro_error );
					return 1;
				}
			}
		}

#else
	TRON( "set_color_depth(16)..." );
	set_color_depth( 16 );
	//set_gfx_mode(GFX_SAFE, 320, 200, 0, 0); // claudemir 320x200
	if ( set_gfx_mode( GFX_AUTODETECT, screen_width, screen_height, 0, 0 ) != 0 )
	{
		TRON( "set_color_depth(15)..." );
		set_color_depth ( 15 );
		if ( set_gfx_mode( BR_GFX, screen_width, screen_height, 0, 0 ) != 0 )
		{
			set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
			sprintf( log_mess, "\n\nError setting graphics mode\n%s\nTry a"
			         " different video mode !\n", allegro_error );
			ERROR( log_mess );
			return 1;
		}
	}
#endif
	start_x = 0;
	cx = 0;
	n = 0;
	/*** end of GFX mode setting ***/
	
	text_mode ( -1 ); // est� fun��o faz os texto ficarem transparentes :)
	small_font = font;
	virtscreen = create_bitmap( 325, 205 );
	if ( ( gfmode == 1 ) || ( gfmode > 2 ) )
		scanliner = create_bitmap( screen_width, 400 );
	else
		scanliner = create_bitmap( 10, 10 );

	/*************************************/
	// Get INFO from GAME.INI FILE
#if GAME_DEBUG > 0
	sprintf( log_mess, "sreen_width:%d screen_height:%d\n", screen_width, screen_height );
	TRON( log_mess );
	sprintf( log_mess, "screen->w:%d screen->h:%d\n", screen->w, screen->h );
	TRON( log_mess );
#endif

	loadsounds();
	set_volume ( get_snd_vol() , midi_vol );

        nbchar=listfiles( charname, DIR_SPRITES, "" );
	nbbkgd=listfiles( bkgdname, DIR_BKGDS, "pcx" );
        nbflc=listfiles( flcname, DIR_BKGDS, "flc" );

	for ( cx = 0; cx < nbflc; cx++ )
	{
		strcpy ( bkgdname[ nbbkgd + cx ], flcname[ cx ] );
	}
	nbbkgd = nbbkgd + nbflc;
	
	printf( "%d characters\n%d backgrounds\nfound .", nbchar, nbbkgd );
#if GAME_DEBUG > 0
	sprintf( log_mess, "%d characters\n%d backgrounds\nfound .", nbchar, nbbkgd );
#endif
	TRON( log_mess );

#if TIMEOVER > 0

	PUT_t_clock(  xget_config_int( "timer", "t_clock", 0 ) );
#endif

	// criar as cores para textos na engine
	sel_player1 = makecol( 255, 0, 0 );	// verde ?
	sel_player2 = makecol( 0, 255, 0 );	// vermelho ?
	alt_color = makecol( 120, 0, 0 );
	tcolor = makecol( 255, 0, 255 );
	bcolor = makecol( 0, 0, 0 );		// preto
	sel_bkgd = makecol( 0, 255, 0 );
	text_color = -1;
	intro_delay = get_config_int ( 0 , "intro_delay" , 125 );
	wimpact_nbf = get_config_int ( 0 , "wimpact_nbf" , 1 );
	simpact_nbf = get_config_int ( 0 , "simpact_nbf" , 1 );
	bimpact_nbf = get_config_int ( 0 , "bimpact_nbf" , 1 );
	if ( ( Lock = xload_pcx ( DIR_SPRITES "lock.pcx" , 0 ) ) == NULL )
	{
		ERROR( "lock.pcx missing" );
		//exit( 0 );
	}
	for ( num = 1; num < ( wimpact_nbf + 1 ); ++num )
	{
		sprintf( file, DIR_SPRITES "wimpact%d.pcx", num );
		if ( ( WImpact[ num ] = xload_pcx ( file , 0 ) ) == NULL )
		{
			sprintf( log_mess, "Can't open %s file", file );
			ERROR( log_mess );
			//exit( 0 );
		}
	}
	for ( num = 1; num < ( simpact_nbf + 1 ); ++num )
	{
		sprintf( file, DIR_SPRITES "simpact%d.pcx", num );
		SImpact[ num ] = xload_pcx ( file , 0 );
	}
	for ( num = 1; num < ( bimpact_nbf + 1 ); ++num )
	{
		sprintf( file, DIR_SPRITES "bimpact%d.pcx", num );
		BlockImpact[ num ] = xload_pcx ( file , 0 );
	}
	// Get Control Keys
	p1_up = get_config_int ( "controls" , "p1_up" , 6 );
	p1_down = get_config_int ( "controls" , "p1_down" , 22 );
	p1_left = get_config_int ( "controls" , "p1_left" , 3 );
	p1_right = get_config_int ( "controls" , "p1_right" , 2 );
	p1_wpunch = get_config_int ( "controls" , "p1_wpunch" , 17 );
	p1_spunch = get_config_int ( "controls" , "p1_spunch" , 1 );
	p1_wkick = get_config_int ( "controls" , "p1_wkick" , 26 );
	p1_skick = get_config_int ( "controls" , "p1_skick" , 23 );
	p2_up = get_config_int ( "controls" , "p2_up" , 84 );
	p2_down = get_config_int ( "controls" , "p2_down" , 85 );
	p2_left = get_config_int ( "controls" , "p2_left" , 82 );
	p2_right = get_config_int ( "controls" , "p2_right" , 83 );
	p2_wpunch = get_config_int ( "controls" , "p2_wpunch" , 15 );
	p2_spunch = get_config_int ( "controls" , "p2_spunch" , 16 );
	p2_wkick = get_config_int ( "controls" , "p2_wkick" , 11 );
	p2_skick = get_config_int ( "controls" , "p2_skick" , 12 );
	//get AI parameters
	easy_def = get_config_int ( 0, "easy_def" , 60 );
	easy_off = get_config_int ( 0, "easy_off" , 30 );
	medium_def = get_config_int ( 0, "medium_def" , 70 );
	medium_off = get_config_int ( 0, "medium_off" , 50 );
	hard_def = get_config_int ( 0, "hard_def" , 60 );
	hard_off = get_config_int ( 0, "hard_off" , 80 );
	TRON( "load font" );
	if ( !( font = fonts_dat[ 0 ].dat ) )
	{
		ERROR( "failed loading font" );
		//allegro_exit();
		//exit( 0 );
	}
	// SCAN CHARS DIRECTORY
	for ( int cx = 0; cx < nbchar; cx++ )
	{
		sprintf( passeur, DIR_SPRITES "%s" DIR_BAR "static1.pcx", charname[ cx ] );
        present[ cx ] = ( xload_pcx( passeur, Pal ) != NULL );
		sprintf( passeur, DIR_SPRITES "%s" DIR_BAR "char.ini", charname[ cx ] );
		override_config_file( passeur );
		secretchar[ cx ] = get_config_int ( charname[ cx ], "locked" , 0 );
	}
	TRON( "INTRO..." );
	clear_keybuf();
	Intro();
	TRON( "INSTALL SOUND..." );
	
	// Habilitando o SOM :)
	if ( install_sound( snd_flag ? DIGI_NONE : DIGI_AUTODETECT,
						midi_flag ? MIDI_NONE : MIDI_AUTODETECT, 0 ) != 0 )
	{
		ERROR( "Error initialising sound system\n" );
		return 1;
	}
	// fim da rotina de SOM

	//if ( ( music = xload_midi ( DIR_MIDI "select.mid" ) ) == NULL )
	//{
        //	ERROR( "error in midi file format" );
	//	allegro_exit();
	//	exit( 0 );
	//}
	
	music = xload_midi ( DIR_MIDI "select.mid" );
	TRON( "play midi" );
	if( midi_ok() ) play_midi ( music , 1 );
	TRON( "OK." );


	/*
	-------- PROGRAM MAIN LOOPS ---------
	   inits() <-------\
	                   |
	   startmenu()  ---/  <--\
	                         |
	 K_batle()  -------------/
	--------------------------------------
	--------- PROGRAM STRUCTURE -----------
	main.c
	     |
	     |---inits.c
	     |---startmenu.c
	     |---K_batle.c
	     |   \---moves.c
	     |
	     \---mp3.c

	-------------------------------
	*/
	while ( 1 )
	{
		TRON( " /* inits() */ " );
		inits();
		
		while ( 1 )
		{
			/* StartMenu*/
			TRON( "startmenu()" );
			loop = startmenu();
			if ( loop )
				break;
			/* K_batle() */
			TRON( "K_batle()" );
			if ( !K_batle() )
				break;
		}

		if ( loop )
			continue;

		/******** SMALL ENDING STUFF *****************/
		fade_out( 2 );
		TRON( "destroy_bitmap()" );
		destroy_bitmap ( zoomscreen );
		TRON( "OK." );
		if ( animated )
			close_fli();
		/* goto Demoniac; */
	} /* while inits */
	TRON( "\n  **WARNING: END OF PROGRAM\n" );
} /*eof main() */
END_OF_MAIN();
/******************************************************/
void Intro( void )
{
	char x , y , bab; // dumb variables
	int mvb = 20;
	bab = 1;
	Title = xload_pcx ( "title.pcx" , Pal );
	clear ( virtscreen );
	clear ( screen );

	// newmessages !!
	// bom lugar pra colocar a Intro .flc e message de inicializa��o !

	textout_centre(virtscreen, font, 
			"KOF2003 - A Engine Fight Game"
			, virtscreen->w/2, lineprint, makecol(255,0,0));
	textout_centre(virtscreen, font, 
			"Maintainers:"
			, virtscreen->w/2, lineprint+=15, makecol(255,255,255));
	
	textout_centre(virtscreen, font,
		        "Claudemir Jr" 
		        , virtscreen->w/2, lineprint+=15, makecol(255,255,255));

	textout_centre(virtscreen, font,
                        "<claudemir.daluz@virtuallink.com.br>"			                        , virtscreen->w/2, lineprint+=11, makecol(255,255,255));
	
	textout_centre(virtscreen, font,
			"Rick Leite <ric@asbyte.com.br>"
			, virtscreen->w/2, lineprint+=11, makecol(255,255,255));
	
	textout_centre(virtscreen, font,
			"Based on KOF91 code http://kof91.com by:"
			, virtscreen->w/2, lineprint+=15, makecol(0,255,0));
	
	textout_centre(virtscreen, font,
			"Nicolas Sevez, (Moah) moah@kof91.com"
			, virtscreen->w/2, lineprint+=11, makecol(255,255,255));
		
	textout_centre(virtscreen, font,
			"Linux Ported by:"
			, virtscreen->w/2, lineprint+=15, makecol(0,255,0));
textout_centre(virtscreen, font,
                         "Duncan M. Vicar <duncan@kde.org>"
			 , virtscreen->w/2, lineprint+=11, makecol(255,255,255));
	textout_centre(virtscreen, font,
			"Contributors:"
    			, virtscreen->w/2, lineprint+=15, makecol(0,255,0));
	
        textout_centre(virtscreen, font,
		        "Ivan Toledo <birdie@alumnos.utem.cl>"
                        , virtscreen->w/2, lineprint+=11, makecol(255,255,255));

        textout_centre(virtscreen, font,
			"Special tanks"
			, virtscreen->w/2, lineprint+=15, makecol(0,255,0));

	textout_centre(virtscreen, font,
                        "NeoFix aka"
                        , virtscreen->w/2, lineprint+=11, makecol(255,255,255));
	textout_centre(virtscreen, font,
	"Daniel L.A. Moura <neofix2k@hotmail.com>"
	, virtscreen->w/2, lineprint+=11, makecol(255,255,255));
        text_centre_shadow(virtscreen, font,
	"GPL Ver. 2, June 1991, see licence.txt"
//        "��������� ���������" //test diacritcs ...
       , virtscreen->w/2, lineprint+=15, makecol(0,0,255));


	
        blit_KOF91 ();
	TRON("\n\nfalne mano..\n\n");

//	rest(9000);
	
       while ( !keypressed() ) ; 

//	if ( open_fli ( "open.flc" ) != FLI_OK )
//
//ERROR( "open.flc is corrupted or missing\n" );
		//allegro_exit();
		//exit( 0 );
//
	if ( !MP3_Init() )
		ERROR( "Error initialising MP3 player\n" );

	y = MP3_Open ( "title.mp3" );
	rest ( 400 );
	if ( y )
		MP3_Play();
	retrace_count = 0;
	if( open_fli ( "open.flc" ) != FLI_OK ){
		ERROR( "open.flc is corrupted or missing\n" );
	}else{
	do
	{
		if ( mvb++ > 1 )
			mvb = 0;
		if ( y )
			MP3_Play();
		if ( !mvb )
		{
			retrace_count -= 2;
			reset_fli_variables();
			if ( next_fli_frame ( 0 ) != FLI_OK )
				bab = 0;
			if ( keypressed() )
				bab = 0;
			set_palette ( fli_palette );
			blit ( fli_bitmap , virtscreen , 0 , 0 , 0 , 0 , 320 , 200 );
			blit_KOF91();
			if ( !mp3 )
				rest( 15 );
			while ( !retrace_count )
				;
			;
		}
	}
	while ( bab );
	close_fli ();
	//rest (900);
        }
	clear_keybuf ();
	clear ( virtscreen );
	//   set_palette ( black_palette );
	mvb = 20;
	retrace_count = 0;

	if ( open_fli ( "intro.flc" ) == FLI_OK )
		do
		{
			if ( mvb++ > 0 )
				mvb = 0;
			if ( mvb )
			{
				retrace_count -= 2;
				if ( next_fli_frame ( 1 ) != FLI_OK )
					simulate_keypress ( KEY_SPACE );
				set_palette ( fli_palette );
				blit ( fli_bitmap , virtscreen , 0 , 0 , 0 , 0 , 320 , 200 );
				blit_KOF91();
				reset_fli_variables();
				if ( !mp3 )
					rest( 15 );
				while ( !retrace_count )
					;
				;
			}
			else
			{
				if ( y )
					MP3_Play();
			}
		}
		while ( !keypressed() );
	close_fli();
	set_palette( black_palette );
	draw_sprite( virtscreen , Title , 0 , 0 );
	blit_KOF91();
	clear_keybuf();
	x = 1;
	/*
	   while( x ){
	        if (y) MP3_Play();
	        if (keypressed()) x = 0;
	   }
	*/
	do
	{
		if ( y )
			MP3_Play();
	}
	while ( !keypressed() );
	MP3_Close();
	MP3_Exit();
	//Done();
}
/********************************************************************/
char get_kstatus ( char strg[] )
{
	char rtn_value='\0';
	char c;
	c = strg[ 0 ];
	switch ( c )
	{
			case 'U' :
			if ( (c = strg[ 1 ]) )
			{
				if ( c == 'L' )
					rtn_value = 7;
				else
					if ( c == 'R' )
						rtn_value = 8;
			}
			else
				rtn_value = 1;
			break;
			case 'D' :
			if ( (c = strg[ 1 ]) )
			{
				if ( c == 'L' )
					rtn_value = 5;
				else
					if ( c == 'R' )
						rtn_value = 6;
			}
			else
				rtn_value = 2;
			break;
			case 'L' :
			rtn_value = 3;
			break;
			case 'R' :
			rtn_value = 4;
			break;
			case 'P' :
			rtn_value = 9;
			break;
			case 'K' :
			rtn_value = 10;
			break;
			case 'N' :
			rtn_value = 0;
			break;
			default :
			rtn_value = 10;
			break;
	}
	return rtn_value;
}
/***************************************************************/
char check_kstatus1 ( char ctrl_key )
{
	char rtnv=0;
	poll_joystick();
	switch ( ctrl_key )
	{
			case 0 :
			if ( !P1_UP && !P1_DN && !P1_LT && !P1_RT
					&& !P1_WP && !P1_SP && !P1_WK && !P1_SK )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 1 :
			if ( P1_UP && !P1_DN && !P1_LT && !P1_RT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 2 :
			if ( P1_DN && !P1_UP && !P1_LT && !P1_RT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 3 :
			if ( P1_LT && !P1_UP && !P1_DN && !P1_RT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 4 :
			if ( P1_RT && !P1_UP && !P1_DN && !P1_LT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 5 :
			if ( P1_DN && !P1_UP && !P1_RT && P1_LT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 6 :
			if ( P1_DN && !P1_UP && P1_RT && !P1_LT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 7 :
			if ( !P1_DN && P1_UP && !P1_RT && P1_LT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 8 :
			if ( !P1_DN && P1_UP && P1_RT && !P1_LT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 9 :
			if ( ( P1_WP || P1_SP ) && ( !P1_WK && !P1_SK ) )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 10:
			if ( ( !P1_WP && !P1_SP ) && ( P1_WK || P1_SK ) )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			default:
			sprintf( log_mess, "problem in check_kstatus1() : %c",
					 ctrl_key );
			ERROR( log_mess );
			//exit( 0 );
			break;
	}
	if ( rtnv )
		last_status1 = ctrl_key;
	return rtnv;
}
/***********************************************************************/
char check_kstatus2 ( char ctrl_key )
{
	char rtnv=0;
	switch ( ctrl_key )
	{
			case 0 :
			if ( !P2_UP && !P2_DN && !P2_LT && !P2_RT
					&& !P2_WP && !P2_SP
					&& !P2_WK && !P2_SK )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 1 :
			if ( P2_UP && !P2_DN && !P2_LT
					&& !P2_RT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 2 :
			if ( P2_DN && !P2_UP && !P2_LT
					&& !P2_RT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 3 :
			if ( !P2_LT && !P2_UP && !P2_DN
					&& P2_RT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 4 :
			if ( !P2_RT && !P2_UP && !P2_DN
					&& P2_LT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 5 :
			if ( P2_DN && !P2_UP && P2_RT
					&& !P2_LT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 6 :
			if ( P2_DN && !P2_UP && !P2_RT
					&& P2_LT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 7 :
			if ( !P2_DN && P2_UP && P2_RT
					&& !P2_LT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 8 :
			if ( !P2_DN && P2_UP && !P2_RT
					&& P2_LT )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 9 :
			if ( ( P2_WP || P2_SP ) && ( !P2_WK
										 && !P2_SK ) )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			case 10:
			if ( ( !P2_WP && !P2_SP ) && ( P2_WK
										   || P2_SK ) )
				rtnv = 1;
			else
				rtnv = 0;
			break;
			default:
			sprintf( log_mess, "problem in check_kstatus2() : %c", ctrl_key );
			ERROR( log_mess );
			//exit( 0 );
			break;
	}
	if ( rtnv )
		last_status2 = ctrl_key;
	return rtnv;
}

// EOF
