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

#include "config.h"
#include "macros.h"
#include "global.h"
#include "loads.h"
#include "refresh.h"
#include "options.h"
#include "wavsounds.h"
#include  "sprites.h"
#include "inits.h"

int inits( void )
{
	/**** MAIN MENU ****/
	/* Demoniac: */

	clear_keybuf() ;

	for ( cx = 0 ; cx < nbchar ; cx++ )
	{
		defeated[ cx ] = 0 ;
	}


	story = 0 ;
	cx = 1 ;
	cy = 27 ;

	Selecteur1 = create_bitmap( 400, 28 );
	Selecteur2 = create_bitmap( 400, 28 );

	Bar2 = create_bitmap( 200,50 ) ;
	fill_sprite( Bar2 );

	Carton_KO = xload_pcx( DIR_SPRITE "mode.pcx", Pal );

	Carton_WN = xload_pcx( DIR_SPRITE "pick.pcx", Pal );

#if TIMEOVER > 0
	Carton_TO = xload_pcx( DIR_SPRITE "timeup.pcx", Pal );
#endif

	gmode = 2 ;
	p2_sel = 0 ;

	clear_keybuf() ;

	while ( cx )
	{
		draw_sprite( virtscreen , Title, 0 , 0 ) ;

		masked_blit ( Carton_KO , virtscreen , 0 , 0
			     , ( 320 - Carton_KO->w ) / 2
			     , ( 3 * 200 / 4 ) - ( Carton_KO->h / 2 )
			     , Carton_KO->w , Carton_KO->h ) ;

		masked_blit ( Carton_WN , virtscreen , 0 , 0
			     , ( 320 - Carton_KO->w ) / 2 + 12
			     , ( 3 * 200 / 4 ) - ( Carton_KO->h / 2 ) + cy
			     , Carton_WN->w , Carton_WN->h ) ;
		
		rectfill( virtscreen, 
		        ( 320 - Carton_KO->w ) / 2 + 32-2,
		        ( 3 * 200 / 4 ) - ( Carton_KO->h / 2 ) + 72,
		        ( 320 - Carton_KO->w ) / 2 + 32+(7*8)+1,
		        ( 3 * 200 / 4 ) - ( Carton_KO->h / 2 ) + 72 + 13,	
                        makecol( 0,1,0 ) );
		
		text_shadow( virtscreen, font, "OPTIONS", 
			   ( 320 - Carton_KO->w ) / 2 + 32,
			   ( 3 * 200 / 4 ) - ( Carton_KO->h / 2 ) + 72,
			   makecol( 255,255,255 ) );

		blit_KOF91() ;

		if ( key[ KEY_F1 ] && key[ KEY_F5 ] && key[ KEY_F9 ] )
		{
			for ( x = 0 ; x < nbchar ; x++ )
				secretchar[ x ] = 0 ;
			rectfill( virtscreen, 10, 64, 310, 90, sel_player1 );
			textout_centre ( virtscreen , font , 
					"SECRET CHARACTERS UNLOCKED",
					160 , 70 , bcolor ) ;
			blit_KOF91() ;
			Done();
			rest( 700 );
		}

		rest( 150 ) ;

		poll_joystick() ;

		if ( ( P2_SP ) || ( P2_SK )
				|| ( P2_WP ) || ( P2_WK )
				|| ( key[ KEY_ENTER ] )
				|| P1_WK || P1_SK || P1_WP || P1_SP )

		{  
		   Done(); 
		   cx = 0 ; 
		   if( gmode==3 ){
                      cx = 1;
//
		      options();

		   }
		}

		if ( ( P2_UP ) || P1_UP )
		{
			if ( gmode < 3 )
				gmode++ ;
			else
				gmode = 0 ;
			Bing();
		}

		if ( ( P2_DN ) || P1_DN )
		{
			if ( gmode > 0 )
				gmode-- ;
			else
				gmode = 3 ;
			Bing();
		}

		switch ( gmode )
		{
			case 0 :
			p2_sel = 1 ;
			cy = 57 ;
			break ;

			case 1 :
			p2_sel = 0 ;
			cy = 42 ;
			break ;

			case 2 :
			p2_sel = 0 ;
			cy = 27 ;
			break ;

			case 3 :
			cy = 72 ;
			break ;
		}

		if ( key[ KEY_ESC ] )
		{

			/* Stop trapping the keys */
			remove_keyboard() ;
			TRON( "\ndestroy_midi (music) " );
			destroy_midi ( music ) ;
			TRON( "\nOK. destroy_midi (music) " );
			destroy_bitmaps() ;
                        TRON( "\nOK. destroy_bitmaps()" );
			/* Deallocate sounds */
			freesounds();
			/* Return to text mode */
			allegro_exit() ;
			/* Display some stats if available */
			/*                  if ( nbpar > 0 )
			 
			                  allegro_message( "----> %ld FPS\n\n", n / (t2 - t1 ) );
			*/
			TRON( "ESC pressioned..." );
			exit( 0 ) ;
		}

	}

	if ( gmode )
	{
		cx = 1 ;
		cy = 42 ;

		diff = 1 ;

		Carton_KO = xload_pcx( DIR_SPRITE "diff.pcx", Pal );

		clear_keybuf() ;

		while ( cx )
		{
			draw_sprite( virtscreen , Title, 0 , 0 ) ;

			masked_blit ( Carton_KO , virtscreen , 0 , 0
				  , ( 320 - Carton_KO->w ) / 2
				  , ( 3 * 200 / 4 ) - ( Carton_KO->h / 2 )
				  , Carton_KO->w , Carton_KO->h ) ;

			masked_blit ( Carton_WN , virtscreen , 0 , 0
				  , ( 320 - Carton_KO->w ) / 2 + 12
				  , ( 3 * 200 / 4 ) - ( Carton_KO->h / 2 ) + cy
				  , Carton_WN->w , Carton_WN->h ) ;

			blit_KOF91() ;

			rest( 150 ) ;

			poll_joystick() ;

			if ( ( P2_SP ) || ( P2_SK )
				       || ( P2_WP ) || ( P2_WK )
				       || ( key[ KEY_ENTER ] )
				       || P1_WK || P1_SK || P1_WP || P1_SP )
			{ Done(); cx = 0 ; }

			if ( ( P2_UP ) || P1_UP )
			{
				if ( diff > 0 )
					diff-- ;
				else
					diff = 2 ;
				Bing();
			}

			if ( ( P2_DN ) || P1_DN )
			{
				if ( diff < 2 )
					diff++ ;
				else
					diff = 0 ;
				Bing();
			}

			switch ( diff )
			{
					case 0 :
					cy = 27 ;
					break ;

					case 1 :
					cy = 42 ;
					break ;

					case 2 :
					cy = 57 ;
					break ;
			}
		}
	}

	TRON( "step 1 : clear screen" );

	clear( screen ) ;

	// Selection Menu
	TRON( "step 2 : destroy midi" );

	TRON( "\ndestroy_midi (music) " );
	if ( music != NULL )
		destroy_midi ( music ) ;
	TRON( "\nOK. destroy_midi (music) " );

	return 0 ;
}

// EOF
