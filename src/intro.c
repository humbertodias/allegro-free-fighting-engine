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

#include "global.h"
#include "macros.h"
#include "config.h"
#include "display.h"
#include "refresh.h"
#include "moves.h"
#include "loads.h"
#include "wavsounds.h"
#include "intro.h"
/***********************************************/


void intro( void )
{
	int i = 1;
	int f1 = 1;
	int f2 = 1;

	TRON( "INI - Intro" );
	retrace_count = 0;
	f1 = 1;
	f2 = 1;
	i = 1;
 
        initdisplay();
        start_timer();
        displays();
        blit_KOF91();
        blit_Bkgd();

	while ( ( f1 <= p1.intro.nbf ) && ( f2 <= p2.intro.nbf ) )
	{
		if ( animated )
			bkgd_counter++;
		if ( retrace_count > 2 )
			retrace_count = 0;
		retrace_count--;
		if ( retrace_count > 0 )
			;
		else
		{
			if ( foreground )
				masked_blit( Frgd, zoomscreen,
							 0, 0, 0, 0, ( bkgdlim + 320 ),
							 ( 200 + zoomsup ) );
			blit( zoomscreen, virtscreen, cx, zoomsup, 0, 0,
				  320, 200 );
                        refresh_timer();
			displays();
			blit_KOF91();
			blit_Bkgd();
		}
		if ( f1 < p1.intro.nbf )
			GKick( f1 , spr1x, spr1y, 2 );
		else
			GStatic( ( int ) ( f1 / p1.statik.spd ), spr1x, spr1y );
		if ( f2 < p2.intro.nbf )
			GKick2( f2, spr2x, spr2y, 2 );
		else
			GStatic2( ( int ) ( f2 / p2.statik.spd ), spr2x, spr2y );

		if ( ++i >= 5 )
		{ f1++; f2++; i = 1; }

	} /* while */
	TRON( "END - Intro" );

}
/****************************************************************/


/****************************************************************/
void introround( int round )
{
	int deb = 1;
#if GAME_DEBUG > 0
	sprintf( log_mess, "ROUND: %d", round );
#endif
	TRON( log_mess );

	while ( deb )
	{
		deb++;
		if ( deb == 2 )
		{
			load_kick1();
			load_kick2();
			retrace_count = 0;
		}

		if ( deb == 50 )
			Round( round );

		if ( deb == 240 )
			Fight();
		/*
		 * Update the real screen 
		 */ 
		// vsync() ;

		if ( animated )
			bkgd_counter++;

		retrace_count--;

		if ( retrace_count > 0 )
			;
		else
		{
                        refresh_timer();
			displays();
			blit_KOF91();
			blit_Bkgd();

			GStatic2( ( int ) ( flag2 / p2.statik.spd ), spr2x, spr2y );
			GStatic( ( int ) ( flag / p1.statik.spd ), spr1x, spr1y );
			if ( foreground )
				masked_blit( Frgd, zoomscreen,
							 0, 0, 0, 0, ( bkgdlim + 320 ),
							 ( 200 + zoomsup ) );
			blit( zoomscreen, virtscreen, cx, zoomsup, 0, 0, 320, 200 );
			/*		while (!retrace_count);; */
		}

		flag++;
		flag2++;
		if ( flag > ( ( p1.statik.spd * p1.statik.nbf ) - 1 ) )
			flag = 0;
		if ( flag2 > ( ( p2.statik.spd * p2.statik.nbf ) - 1 ) )
			flag2 = 0;
		if ( ( deb > 50 ) && ( deb < 150 ) )
		{
			if ( deb < 76 )
				angle = ( deb - 50 ) * ( Carton_R1->w / 25 );
			else if ( deb > 124 )
				angle = ( 25 - ( deb - 125 ) ) * ( Carton_R1->w / 25 );
			else
				angle = Carton_R1->w;
			switch ( round )
			{
					case 1:
					stretch_sprite( virtscreen, Carton_R1,
									( 320 - angle ) / 2,
									( 200 - Carton_R1->h ) / 2, angle,
									Carton_R1->h );
					break;
					case 2:
					stretch_sprite( virtscreen, Carton_R2,
									( 320 - angle ) / 2,
									( 200 - Carton_R2->h ) / 2, angle,
									Carton_R2->h );
					break;
					case 3:
					stretch_sprite( virtscreen, Carton_R3,
									( 320 - angle ) / 2,
									( 200 - Carton_R3->h ) / 2, angle,
									Carton_R3->h );
					break;
					default:
					stretch_sprite( virtscreen, Carton_R1,
									( 320 - angle ) / 2,
									( 200 - Carton_R1->h ) / 2, angle,
									Carton_R1->h );
					break;
			}
		}
		else if ( ( deb > 240 ) && ( deb < 320 ) )
		{
			masked_blit( Carton_FT, virtscreen, 0, 0,
						 ( 320 - Carton_FT->w ) / 2,
						 ( 200 - Carton_FT->h ) / 2, Carton_FT->w,
						 Carton_FT->h );
		}
		else if ( deb > 320 )
		{
			if ( !nbpar )
			{
				/*
				 * Frame count starts at 0 
				 */
				n = 0;
				clear_keybuf();
				poll_joystick();
			}

			deb = 0;
			retrace_count = 0;
		}
	} /* while (deb) */
#if GAME_DEBUG > 0
	sprintf( log_mess, "ROUND: %d - OK", round );
#endif
	TRON( log_mess );

}
/*********************************************************************/

// EOF
