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
#include "stdio.h"
#include "monitor.h"
#include "logger.h" 
/*************************************************************************
 
 patch: ric@asbyte.com.br
 
**************************************************************************/
#if GAME_DEBUG > 0
void __tron__( char *log_mess, char *n_file , int n_line )
{
	static FILE *logfile;
	

	if ( ( logfile = fopen( "kof2003.log", "a" ) ) == ( FILE* ) NULL )
	{
		fprintf( stderr, "\nWARNING! Can't open log file.\n" );
		fprintf( stderr, "TRON() ERROR in: %s LINE:%d\n",n_file,n_line );
		allegro_exit();
		exit( 0 );
	}
	else
	{
		fprintf( logfile, "FILE: %s LINE:%d: %s\n\n",
				 n_file, n_line, log_mess );
		console_print( log_mess );

	}
	fclose( logfile );
}

void __tronstart__( char *log_mess, char *n_file , int n_line )
{
	FILE *logfile;
	

	if ( ( logfile = fopen( "kof2003.log", "a" ) ) == ( FILE* ) NULL )
	{
		fprintf( stderr, "\nWARNING!. Can't open log file.\n" );
		fprintf( stderr, "TRONSTART() ERROR in: %s LINE:%d\n",n_file,n_line );
		
		allegro_exit();
		exit( 0 );
	}
	else
	{
		fprintf( logfile, "\n"
				 "**********************************************************************\n"
				 "*       kof2003 v" VERSION " Compiled at " BRDATE " - (" BRTIME ")\n"
				 "**********************************************************************\n"
				 "<TRONSTART> FILE:%s LINE:%d: %s\n\n",
				 n_file, n_line, log_mess );
		console_print( log_mess );
	}
	fclose( logfile );
}
#endif

/*******************************/
void __error__( char *log_mess, char *n_file , int n_line )
{
	static FILE * logfile;

	if ( ( logfile = fopen( "kof2003.log", "a" ) ) == ( FILE* ) NULL )
	{
		fprintf( stderr, "\n(ER)WARNING!. Can't open log file.\n" );
		fprintf( stderr, "ERROR() ERROR in: %s LINE:%d\n",n_file,n_line );

		allegro_exit();
		exit( 0 );
	}
	else
	{
		fprintf( stderr, "<ERROR> FILE:%s LINE:%d:\n  ***ERROR: %s\n\n",
				 n_file, n_line, log_mess );

		fprintf( logfile, "<ERROR> FILE:%s LINE:%d:\n  ***ERROR: %s\n\n",
				 n_file, n_line, log_mess );
	}
	fclose( logfile );
	return;
}

// EOF
