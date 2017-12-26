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

#include <allegro.h>
#include "global.h"
#include "macros.h"
#include "logger.h"
#include "sprites.h"


void destroy_bitmaps ( void )
{
	TRON( "\ndestroy_bitmaps()" );
	unload_datafile ( creditflic );
	TRON( "..." );
	unload_datafile ( fonts_dat );
	TRON( "..." );
	destroy_bitmap( Title );
	//destroy_bitmap(Selecteur);
	TRON( "..." );
	destroy_bitmap( Face1 );
	destroy_bitmap( Face2 );
	TRON( "..." );
	destroy_bitmap( virtscreen );
	TRON( "..." );
	destroy_bitmap( scanliner );
	TRON( "..." );
	destroy_bitmap( Bkgd );
	/*
	TRON("...");
	destroy_bitmap(Blood); 
	*/
	TRON( "..." );
	for ( num = 1; num < 7; ++num )
		destroy_bitmap( Player1.Jump[ num ] );
	TRON( "..." );
	for ( num = 1; num < 6; ++num )
		destroy_bitmap( Player1.KO[ num ] );
	TRON( "..." );
	for ( num = 1; num < ( p1.statik.nbf + 1 ); ++num )
		destroy_bitmap ( Player1.Static[ num ] );

	TRON( "..." );
	for ( num = 1; num < ( p1.walk.nbf + 1 ); ++num )
		destroy_bitmap( Player1.Walk[ num ] );
	TRON( "..." );
	for ( num = 1; num < ( p1.spunch.nbf + 1 ); ++num )
		destroy_bitmap( Player1.Punch[ num ] );
	TRON( "..." );
	for ( num = 1; num < ( p1.wpunch.nbf + 1 ); ++num )
		destroy_bitmap( Player1.WPunch[ num ] );

	TRON( "..." );
	for ( num = 1; num < ( p1.gpunch.nbf + 1 ); ++num )
		destroy_bitmap( Player1.GPunch[ num ] );
	TRON( "..." );
	for ( num = 1; num < ( p1.apunch.nbf + 1 ); ++num )
		destroy_bitmap( Player1.APunch[ num ] );
	TRON( "..." );
	for ( num = 1; num < ( p1.skick.nbf + 1 ); ++num )
		destroy_bitmap( Player1.Kick[ num ] );
	TRON( "..." );
	for ( num = 1; num < ( p1.wkick.nbf + 1 ); ++num )
		destroy_bitmap( Player1.WKick[ num ] );
	TRON( "..." );
	for ( num = 1; num < ( p1.gkick.nbf + 1 ); ++num )
		destroy_bitmap( Player1.GKick[ num ] );

	TRON( "..." );
	for ( num = 1; num < ( p1.hurt.nbf + 1 ); ++num )
		destroy_bitmap( Player1.Hurt[ num ] );

	TRON( "..." );
	for ( num = 1; num < ( p1.ghurt.nbf + 1 ); ++num )
		destroy_bitmap( Player1.GHurt[ num ] );
	TRON( "..." );
	for ( num = 1; num < ( p1.fireb.nbf + 1 ); ++num )
		destroy_bitmap( Player1.FireB[ num ] );
	TRON( "..." );
	for ( num = 1; num < ( p1.fball.nbf + 1 ); ++num )
		destroy_bitmap( Player1.FBall[ num ] );

	TRON( "..." );
	destroy_bitmap( Player1.Single[ 0 ] );
	TRON( "..." );
	destroy_bitmap( Player1.Single[ 1 ] );
	TRON( "..." );
	destroy_bitmap( Player1.Single[ 2 ] );
	TRON( "..." );
	destroy_bitmap( Player1.Single[ 3 ] );
	TRON( "..." );
	for ( num = 1; num < ( p1.akick.nbf + 1 ); ++num )
		destroy_bitmap( Player1.AKick[ num ] );

	// PLAYER 2 SPRITES
	TRON( "PLAYER 2 SPRITES" );
	for ( num = 1; num < ( p2.statik.nbf + 1 ); ++num )
		destroy_bitmap ( Player2.Static[ num ] );

	TRON( "..." );
	for ( num = 1; num < ( p2.walk.nbf + 1 ); ++num )
		destroy_bitmap( Player2.Walk[ num ] );

	for ( num = 1; num < ( p2.spunch.nbf + 1 ); ++num )
		destroy_bitmap( Player2.Punch[ num ] );
	for ( num = 1; num < ( p2.wpunch.nbf + 1 ); ++num )
		destroy_bitmap( Player2.WPunch[ num ] );
	for ( num = 1; num < ( p2.gpunch.nbf + 1 ); ++num )
		destroy_bitmap( Player2.GPunch[ num ] );
	for ( num = 1; num < ( p2.apunch.nbf + 1 ); ++num )
		destroy_bitmap( Player2.APunch[ num ] );
	for ( num = 1; num < 6; ++num )
		destroy_bitmap( Player2.Jump[ num ] );

	for ( num = 1; num < 6; ++num )
		destroy_bitmap( Player2.KO[ num ] );
	for ( num = 1; num < ( p2.skick.nbf + 1 ); ++num )
		destroy_bitmap( Player2.Kick[ num ] );
	for ( num = 1; num < ( p2.wkick.nbf + 1 ); ++num )
		destroy_bitmap( Player2.WKick[ num ] );

	for ( num = 1; num < ( p2.gkick.nbf + 1 ); ++num )
		destroy_bitmap( Player2.GKick[ num ] );
	for ( num = 1; num < ( p2.fireb.nbf + 1 ); ++num )
		destroy_bitmap( Player2.FireB[ num ] );
	for ( num = 1; num < ( p2.fball.nbf + 1 ); ++num )
		destroy_bitmap( Player2.FBall[ num ] );

	destroy_bitmap( Player2.Single[ 0 ] );
	destroy_bitmap( Player2.Single[ 1 ] );
	destroy_bitmap( Player2.Single[ 2 ] );
	destroy_bitmap( Player2.Single[ 3 ] );
	for ( num = 1; num < ( p2.hurt.nbf + 1 ); ++num )
		destroy_bitmap( Player2.Hurt[ num ] );
	for ( num = 1; num < ( p2.ghurt.nbf + 1 ); ++num )
		destroy_bitmap( Player2.GHurt[ num ] );
	for ( num = 1; num < ( p2.akick.nbf + 1 ); ++num )
		destroy_bitmap( Player2.AKick[ num ] );
	TRON( "OK. destroy_bitmaps()" );

}

// EOF
