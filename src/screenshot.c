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

#include "allegro.h"
#include "global.h"
#include "screenshot.h"

#if SCREENSHOT > 0 
/*************************************************************/
void screenshot( void )
{
	char file_name[ 13 ] = "screen1.pcx";
	int ret;
	BITMAP *B_screenshot;

	DIALOG menu_dialog[] =
		{
			/* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)                    (d2)  (dp)              (dp2) (dp3) */
			{ d_edit_proc, 80, 32, 512, 48, 255, 0, 0, 0, sizeof( file_name ) - 1, 0, file_name, NULL, NULL },
			{ d_button_proc, 80, 132, 161, 49, 255, 0, 0, D_EXIT, 0, 0, "SAVE", NULL, NULL },
			{ NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL }
		};

	save_pcx( "tmp", screen, 0 );
	install_mouse();
	ret = do_dialog( menu_dialog, -1 );
	B_screenshot = load_pcx( "tmp", 0 );
	save_pcx( file_name, B_screenshot , 0 );
	unlink( "tmp" );
	remove_mouse();
	destroy_bitmap( B_screenshot );
} /* screenshot( void ) */
#endif

// EOF
