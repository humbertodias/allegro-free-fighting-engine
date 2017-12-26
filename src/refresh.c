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
#include "refresh.h"

/*******************************************************/
void blit_KOF91( void )
{

	static int y;

	if ( ( gfmode == 1 ) || ( gfmode > 2 ) )
	{
		stretch_blit ( virtscreen , scanliner , 0, 0, 320, 200,
					   0, 0, screen_width, screen_height );

		if ( ( gfmode == 1 ) || ( gfmode == 4 ) )
			for ( y = 0; y < 400; y += 2 )
				hline( scanliner , 0 , y , screen_width , 0 );

		acquire_screen();
		blit ( scanliner , screen , 0, 0, 0, 0, screen_width, screen_height );
		release_screen();

	}
	else
	{
		acquire_screen();
		blit ( virtscreen, screen , 0, 0, start_x, start_y, 320, 200 );
		release_screen();
	}

} /*blit_KOF91( void )*/

/*************************************************/
void blit_Bkgd( void )
{
	if ( animated )
	{
		if ( bkgd_counter > bkgd_delay )
		{
			reset_fli_variables();
			next_fli_frame( 1 );
			bkgd_counter = 0;
		}
		blit ( fli_bitmap, zoomscreen, 0, 0, 0, 0,
			   ( bkgdlim + 320 ), ( 200 + zoomsup ) );
	}
	else
	{
		blit ( Bkgd, zoomscreen, 0, 0, 0, 0,
			   ( bkgdlim + 320 ), ( 200 + zoomsup ) );
	}
} /* blit_Bkgd( void ) */

/*****************************************************************/
void refresh_screen( void )
{

	static int y;
	if ( ( gfmode == 1 ) || ( gfmode > 2 ) )
	{
		stretch_blit ( virtscreen , scanliner , 0, 0, 320, 200,
					   0, 0, screen_width, screen_height );

		if ( ( gfmode == 1 ) || ( gfmode == 4 ) )
			for ( y = 0; y < 400; y += 2 )
				hline( scanliner , 0 , y , screen_width , 0 );

		blit ( scanliner , screen , 0, 0, 0, 0, screen_width, screen_height );
	}
	else
	{
		blit ( virtscreen, screen , 0, 0, start_x, start_y, 320, 200 );
	}
} /*refresh_screen( void )*/

// EOF
