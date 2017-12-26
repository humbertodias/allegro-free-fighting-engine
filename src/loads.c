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

#include <string.h>
#include "global.h"
#include "stdio.h"
#include "macros.h"
#include "config.h"
#include "loads.h"
#include "palettes.h"
#include "wavsounds.h"

/*************************************************************************
 patch: ric@asbyte.com.br
**************************************************************************/


static char c_midi_ok=0;
/*************************************************************************
* __load_sprite__() look in macros.h the xload_pcx macro 
*
* this function replace the load_pcx function of allegro and add the green 
*    clone player.
* __load_sprite__() verify if pcx file is found and report to logfile.
**************************************************************************/
#if GAME_DEBUG > 0
BITMAP *__load_sprite__( const char *file, PALLETE Pal, char *n_file , int n_line )
{
#else
BITMAP * __load_sprite__( const char * file, PALLETE Pal )
{
#endif
	static BITMAP *p1;
	static BITMAP *p2;
	static int missing;

	missing=0;
#if GAME_DEBUG > 0
	missing=check_file( file, n_file, n_line );
#else
	missing=check_file( file );
#endif
        if(missing == 1){
	    p2 = create_bitmap( 113,16 );
            rectfill( p2, 0, 0, 112,15, makecol( 255, 0, 0 ) );
	    rect( p2, 0, 0, 112,15, makecol( 0, 0, 255 ) );
	    textout( p2, small_font, "Image Missing!", 2, 4, 0 );
	}else{
	    p2 = load_pcx ( file , Pal );
	}
	
	p1 = create_bitmap ( p2->w, p2->h );

	fill_sprite( p1 );

	draw_sprite_h_flip( p1, p2, 0, 0 );
	fill_sprite( p2 );

	if ( clone )
	{
		if(view_palette_found()==0){
                   set_trans_blender( 0, 255, 0, 0 );
                   draw_lit_sprite( p2, p1, 0, 0, 100 );
		}else{
		   draw_sprite(p2,create_palette( p1 ),0,0);
//                   draw_sprite(p2,p1,0,0); 
		}
		
	}
	else
	{
		draw_sprite( p2, p1, 0, 0 );
	}
        fill_sprite( p1 );
        
	return p2;
}

/*************************************************************************
* __xload_pcx__() look in macros.h the xload_pcx macro 
*
* this function replace the load_pcx function of allegro.
* __xload_pcx__() verify if pcx file is found and report to logfile.
**************************************************************************/
#if GAME_DEBUG > 0
BITMAP *__xload_pcx__( const char * file, PALLETE Pal, char * n_file , int n_line )
{
#else
BITMAP * __xload_pcx__( const char * file, PALLETE Pal )
{
#endif
	BITMAP * PCX_RETURNED;
        char missing;


	missing = 0;
#if GAME_DEBUG > 0
	sprintf( log_mess, "FILE:%s LINE:%d | LOADING: %s", n_file, n_line, file );
	TRON( log_mess );
#endif

#if GAME_DEBUG > 0
	missing=check_file( file, n_file, n_line );
#else
	missing=check_file( file );
#endif
        if ( missing == 1 ){
//	    TRON("CREATING IMAGE...");
	    PCX_RETURNED = create_bitmap( 113,16 ); //load_pcx( "sprites" DIR_BAR "miss.pcx", Pal );
            rectfill( PCX_RETURNED, 0, 0, 112,15, makecol( 255, 0, 0 ) );
	    rect( PCX_RETURNED, 0, 0, 112,15, makecol( 0, 0, 255 ) );
	    textout( PCX_RETURNED, small_font, "Image Missing!", 2, 4, 0 );
//	    TRON("OK");
	}else{
            TRON("LOADING...");
            PCX_RETURNED = load_pcx( file, Pal );		
	    TRON("OK");
	}
	TRON("OK");
	return PCX_RETURNED;
}
/*************************************************************************
* __xload_midi()__ 
*
* this function replace the load_midi function of allegro.
* __xload_midi()__ verify if midi file is found and report to logfile.
**************************************************************************/
#if GAME_DEBUG > 0
MIDI *__xload_midi__( const char * file, char * n_file, int n_line)
{
#else
MIDI *__xload_midi__( const char * file )
{
#endif	
	MIDI * tmpmidi;
	static int missing;

	missing=0;
#if GAME_DEBUG > 0
	missing=check_file( file, n_file, n_line );
#else
	missing=check_file( file );
#endif
 	if(missing==1){
          tmpmidi=NULL;
	  c_midi_ok=0;
	}else{
	  c_midi_ok=1;	
          tmpmidi=load_midi( file );
	}
	return tmpmidi;	
}

/*************************************************************************
* xload_midi() 
*
* this function replace the load_midi function of allegro.
* xload_midi() verify if midi file is found and report to logfile.
**************************************************************************/

#if GAME_DEBUG > 0
SAMPLE *__xload_sample__( const char * file, char *n_file, int n_line  )
{
#else
SAMPLE *__xload_sample__( const char * file )
{
#endif	
        SAMPLE *tmpsample;
	static char missing;

	missing=0;
	
#if GAME_DEBUG > 0
	missing=check_file( file, n_file, n_line );
#else
	missing=check_file( file );
#endif
 	if(missing==1){
          tmpsample=create_sample(8,2,14400,0);
	}else{
          tmpsample=load_sample( file );
	}

	return tmpsample;
}

/******************************************************
* check_file( filename ) 
*   verify if filename is found and report to logfile
*******************************************************/
#if GAME_DEBUG > 0
char check_file( const char * file, char * n_file , int n_line )
{
#else
char check_file( const char * file )
{
#endif
        static FILE *tmpfile;
	static char missing; 

        missing =0 ;
	if ( ( tmpfile=fopen( file, "r" ) ) == NULL )
	{
		#if GAME_DEBUG > 0
		sprintf( log_mess, "FILE:%s LINE:%d ERROR: file (%s) not found! ",
				 n_file, n_line, file );
		__error__( log_mess, n_file, n_line );

                #else
		sprintf( log_mess, "ERROR: file (%s) not found! ", file );
		ERROR( log_mess );
		#endif

	        missing = 1;
		
	}else{
	   fclose( tmpfile );
	}

	return missing;
}
/****************************************/
char file_missing( const char * file ){
        static FILE *tmpfile;
        static char missing;
                                                                                
        missing =0;
        if ( ( tmpfile=fopen( file, "r" ) ) == NULL )
        {
                missing = 1;
        }else{
           fclose( tmpfile );
        }
                                                                                
        return missing;
}
/****************************************/
char midi_ok( void ){
 return c_midi_ok;
}
/***************************************/
// This funciom is based in TDAM 
// author: Ivan Toledo (birdie@alumnos.utem.cl) 
BITMAP * create_shadow(BITMAP * src, int w, int h, char t_shadow)
{
        BITMAP * tmp_image = create_bitmap(w, h/4);
        int line,col;

        stretch_blit(src, tmp_image, 0, 0, w, h, 0, 0, w, h/4);
        
        if(t_shadow==0){ 
        for (line = 0; line < tmp_image->w; line++)
                for (col = 0; col < tmp_image->h; col++)
                        if (getpixel(tmp_image, line, col) != makecol(255,0,255))
                          putpixel(tmp_image, line, col, makecol(0,0,0) );
        }
        return tmp_image;
}
/*****************************************************************************/
/* Start of LOAD FUNCTIONS */
void load_sprites1( void )
{
	TRON( "\nload_sprites1()" );
	sprintf( passeur, "sprites" DIR_BAR "%s" DIR_BAR, charname[ sel1 ] );
	TRON( "loading..." );
	for ( num = 1; num < ( p1.statik.nbf + 1 ); ++num )
	{
		sprintf( file, "%sstatic%d.pcx", passeur, num );
		Player1.Static[ num ] = xload_pcx( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p1.walk.nbf + 1 ); ++num )
	{
		sprintf( file, "%swalk%d.pcx", passeur, num );
		Player1.Walk[ num ] = xload_pcx ( file , Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p1.spunch.nbf + 1 ); ++num )
	{
		sprintf( file, "%sspunch%d.pcx", passeur, num );
		Player1.Punch[ num ] = xload_pcx ( file , Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p1.wpunch.nbf + 1 ); ++num )
	{
		sprintf( file, "%swpunch%d.pcx", passeur, num );
		Player1.WPunch[ num ] = xload_pcx ( file , Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p1.gpunch.nbf + 1 ); ++num )
	{
		sprintf( file, "%sgpunch%d.pcx", passeur, num );
		Player1.GPunch[ num ] = xload_pcx ( file , Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p1.apunch.nbf + 1 ); ++num )
	{
		sprintf( file, "%sapunch%d.pcx", passeur, num );
		Player1.APunch[ num ] = xload_pcx( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p1.intro.nbf + 1 ); ++num )
	{
		sprintf( file, "%sintro%d.pcx", passeur, num );
		Player1.Kick[ num ] = xload_pcx ( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p1.gkick.nbf + 1 ); ++num )
	{
		sprintf( file, "%sgkick%d.pcx", passeur, num );
		Player1.GKick[ num ] = xload_pcx( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p1.wkick.nbf + 1 ); ++num )
	{
		sprintf( file, "%swkick%d.pcx", passeur, num );
		Player1.WKick[ num ] = xload_pcx( file , Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p1.hurt.nbf + 1 ); ++num )
	{
		sprintf( file, "%shurt%d.pcx", passeur, num );
		Player1.Hurt[ num ] = xload_pcx ( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p1.ghurt.nbf + 1 ); ++num )
	{
		sprintf( file, "%sghurt%d.pcx", passeur, num );
		Player1.GHurt[ num ] = xload_pcx ( file, 0 );
	}
	TRON( "loading..." );
	for ( num = 1; num < 6; ++num )
	{
		sprintf( file, "%sjump%d.pcx", passeur, num );
		Player1.Jump[ num ] = xload_pcx ( file , Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < 6; ++num )
	{
		sprintf( file, "%sko%d.pcx", passeur, num );
		Player1.KO[ num ] = xload_pcx ( file , Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < 5; ++num )
	{
		strcpy ( file, passeur );
		switch ( ( int ) num )
		{
				case 1 :
				strcat ( file, "block1.pcx" );
				Player1.Single[ 0 ] = xload_pcx ( file , 0 );
				break;
				case 2 :
				strcat ( file, "crouch1.pcx" );
				Player1.Single[ 1 ] = xload_pcx ( file, Pal );
				break;
				case 3 :
				strcat ( file, "gblock1.pcx" );
				Player1.Single[ 2 ] = xload_pcx ( file, Pal );
				break;
				case 4 :
				strcat ( file, "hurt1.pcx" );
				Player1.Single[ 3 ] = xload_pcx ( file, Pal );
				break;
		}
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p1.akick.nbf + 1 ); ++num )
	{
		sprintf( file, "%sakick%d.pcx", passeur, num );
		Player1.AKick[ num ] = xload_pcx ( file, Pal );
	}
	TRON( "loading..." );
	if ( p1.fireb.flg )
	{
		TRON( "loading..." );

		for ( num = 1; num < ( p1.fireb.nbf + 1 ); ++num )
		{
			sprintf( file, "%sfireb%d.pcx", passeur, num );
			Player1.FireB[ num ] = xload_pcx ( file, Pal );
		}
		TRON( "loading..." );

		for ( num = 1; num < ( p1.fball.nbf + 1 ); ++num )
		{
			sprintf( file, "%sfball%d.pcx", passeur, num );
			Player1.FBall[ num ] = xload_pcx ( file, Pal );
		}
		TRON( "loading..." );
		sprintf( file, "%sfireb.wav", passeur );
		loadwav (DEF_hadoken1, file);
	}
	TRON( "loading..." );
	if ( p1.rush.flg )
	{
		TRON( "loading..." );
		for ( num = 1; num < ( p1.rush.nbf + 1 ); ++num )
		{
			sprintf( file, "%srush%d.pcx", passeur, num );
			Player1.Rush[ num ] = xload_pcx ( file, Pal );
		}
		TRON( "loading..." );

		sprintf( file, "%srush.wav", passeur );
		loadwav( DEF_copter1, file );
	}
	TRON( "loading..." );
	if ( p1.smove.flg )
	{
		TRON( "loading..." );

		for ( num = 1; num < ( p1.smove.nbf + 1 ); ++num )
		{
			sprintf( file, "%ssmove%d.pcx", passeur, num );
			Player1.SMove[ num ] = xload_pcx ( file, Pal );
		}
		TRON( "loading..." );
		sprintf( file, "%ssmove.wav", passeur );
		loadwav( DEF_dragon1, file );
	}
	TRON( "loading..." );
	// SPECIALS
	if ( p1.firebx.flg )
	{
		TRON( "loading..." );

		for ( num = 1; num < ( p1.firebx.nbf + 1 ); ++num )
		{
			sprintf( file, "%sfirebx%d.pcx", passeur, num );
			Player1.FireBX[ num ] = xload_pcx ( file, Pal );
		}
		TRON( "loading..." );

		for ( num = 1; num < ( p1.fballx.nbf + 1 ); ++num )
		{
			sprintf( file, "%sfballx%d.pcx", passeur, num );
			Player1.FBallX[ num ] = xload_pcx ( file, Pal );
		}
		TRON( "loading..." );
		sprintf( file, "%sfirebx.wav", passeur );
		loadwav( DEF_hadokenx1, file );
	}
	TRON( "loading..." );
	if ( p1.rushx.flg )
	{
		TRON( "loading..." );

		for ( num = 1; num < ( p1.rushx.nbf + 1 ); ++num )
		{
			sprintf( file, "%srushx%d.pcx", passeur, num );
			Player1.RushX[ num ] = xload_pcx( file, Pal );
		}
		TRON( "loading..." );
		sprintf( file, "%srushx.wav", passeur );
		loadwav( DEF_copterx1, file );
	}
	TRON( "loading..." );
	if ( p1.smovex.flg )
	{

		TRON( "loading..." );
		for ( num = 1; num < ( p1.smovex.nbf + 1 ); ++num )
		{
			sprintf( file, "%ssmovex%d.pcx", passeur, num );
			Player1.SMoveX[ num ] = xload_pcx ( file, Pal );
		}
		TRON( "loading..." );
		sprintf( file, "%ssmovex.wav", passeur );
		loadwav( DEF_dragonx1, file );
		
	}
	TRON( "\nload_sprites1() - OK." );
}
/********************************************************/
void load_sprites2( void )
{
	char myfile[500];
	TRON( "\nload_sprites2()" );
	sprintf( passeur, "sprites" DIR_BAR "%s" DIR_BAR, charname[ sel2 ] );
	
	if(clone){
	   sprintf( myfile, "%spalette.ini", passeur );   
//	   printf("[%s]\n",myfile);
           load_palette( myfile,1 );
	}
	for ( num = 1; num < ( p2.statik.nbf + 1 ); ++num )
	{
           sprintf( file, "%sstatic%d.pcx", passeur, num );
	   Player2.Static[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );

	for ( num = 1; num < 5; ++num )
	{
		strcpy ( file, passeur );
		switch ( ( int ) num )
		{
				case 1 :
				strcat ( file, "block1.pcx" );
				Player2.Single[ 0 ] = load_sprite( file, Pal );
				break;
				case 2 :
				strcat ( file, "crouch1.pcx" );
				Player2.Single[ 1 ] = load_sprite( file, Pal );
				break;
				case 3 :
				strcat ( file, "gblock1.pcx" );
				Player2.Single[ 2 ] = load_sprite( file, Pal );
				break;
				case 4 :
				strcat ( file, "hurt1.pcx" );
				Player2.Single[ 3 ] = load_sprite( file, Pal );
				break;
		}
	}
	TRON( "loading..." );

	for ( num = 1; num < ( p2.walk.nbf + 1 ); ++num )
	{
		sprintf( file, "%swalk%d.pcx", passeur, num );
		Player2.Walk[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p2.spunch.nbf + 1 ); ++num )
	{
		sprintf( file, "%sspunch%d.pcx", passeur, num );
		Player2.Punch[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p2.wpunch.nbf + 1 ); ++num )
	{
		sprintf( file, "%swpunch%d.pcx", passeur, num );
		Player2.WPunch[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p2.apunch.nbf + 1 ); ++num )
	{
		sprintf( file, "%sapunch%d.pcx", passeur, num );
		Player2.APunch[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p2.gpunch.nbf + 1 ); ++num )
	{
		sprintf( file, "%sgpunch%d.pcx", passeur, num );
		Player2.GPunch[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p2.intro.nbf + 1 ); ++num )
	{
		sprintf( file, "%sintro%d.pcx", passeur, num );
		Player2.Kick[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p2.gkick.nbf + 1 ); ++num )
	{
		sprintf( file, "%sgkick%d.pcx", passeur, num );
		Player2.GKick[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p2.wkick.nbf + 1 ); ++num )
	{
		sprintf( file, "%swkick%d.pcx", passeur, num );
		Player2.WKick[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p2.hurt.nbf + 1 ); ++num )
	{
		sprintf( file, "%shurt%d.pcx", passeur, num );
		Player2.Hurt[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p2.ghurt.nbf + 1 ); ++num )
	{
		sprintf( file, "%sghurt%d.pcx", passeur, num );
		Player2.GHurt[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < 6; ++num )
	{
		sprintf( file, "%sjump%d.pcx", passeur, num );
		Player2.Jump[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < 6; ++num )
	{
		sprintf( file, "%sko%d.pcx", passeur, num );
		Player2.KO[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	for ( num = 1; num < ( p2.akick.nbf + 1 ); ++num )
	{
		sprintf( file, "%sakick%d.pcx", passeur, num );
		Player2.AKick[ num ] = load_sprite( file, Pal );
	}
	TRON( "loading..." );
	if ( p2.fireb.flg )
	{
		TRON( "loading..." );
		for ( num = 1; num < ( p2.fireb.nbf + 1 ); ++num )
		{
			sprintf( file, "%sfireb%d.pcx", passeur, num );
			Player2.FireB[ num ] = load_sprite( file, Pal );
		}
		TRON( "loading..." );
		for ( num = 1; num < ( p2.fball.nbf + 1 ); ++num )
		{
			sprintf( file, "%sfball%d.pcx", passeur, num );
			Player2.FBall[ num ] = load_sprite( file, Pal );
		}
		TRON( "loading..." );
		sprintf( file, "%sfireb.wav", passeur );
		loadwav( DEF_hadoken2, file );
	}
	TRON( "loading..." );
	if ( p2.rush.flg )
	{
		TRON( "loading..." );
		for ( num = 1; num < ( p2.rush.nbf + 1 ); ++num )
		{
			sprintf( file, "%srush%d.pcx", passeur, num );
			Player2.Rush[ num ] = load_sprite( file, Pal );
		}
		TRON( "loading..." );
		sprintf( file, "%srush.wav", passeur );
		loadwav( DEF_copter2, file );
		
	}
	TRON( "loading..." );
	if ( p2.smove.flg )
	{
		TRON( "loading..." );
		for ( num = 1; num < ( p2.smove.nbf + 1 ); ++num )
		{
			sprintf( file, "%ssmove%d.pcx", passeur, num );
			Player2.SMove[ num ] = load_sprite( file, Pal );
		}
		TRON( "loading..." );
		sprintf( file, "%ssmove.wav", passeur );
		loadwav( DEF_dragon2, file );
	}
	TRON( "loading..." );
	// SUPER SPECIALS
	if ( p2.firebx.flg )
	{
		TRON( "loading..." );
		for ( num = 1; num < ( p2.firebx.nbf + 1 ); ++num )
		{
			sprintf( file, "%sfirebx%d.pcx", passeur, num );
			Player2.FireBX[ num ] = load_sprite( file, Pal );
		}
		TRON( "loading..." );
		for ( num = 1; num < ( p2.fballx.nbf + 1 ); ++num )
		{
			sprintf( file, "%sfballx%d.pcx", passeur, num );
			Player2.FBallX[ num ] = load_sprite( file, Pal );
		}
		TRON( "loading..." );
		sprintf( file, "%sfirebx.wav", passeur );
		loadwav( DEF_hadokenx2, file );
	}
	TRON( "loading..." );
	if ( p2.rushx.flg )
	{
		TRON( "loading..." );
		for ( num = 1; num < ( p2.rushx.nbf + 1 ); ++num )
		{
			sprintf( file, "%srushx%d.pcx", passeur, num );
			Player2.RushX[ num ] = load_sprite( file, Pal );
		}
		TRON( "loading..." );
		sprintf( file, "%srushx.wav", passeur );
		loadwav( DEF_copterx2, file );
	}
	TRON( "loading..." );
	if ( p2.smovex.flg )
	{
		TRON( "loading..." );
		for ( num = 1; num < ( p2.smovex.nbf + 1 ); ++num )
		{
			sprintf( file, "%ssmovex%d.pcx", passeur, num );
			Player2.SMoveX[ num ] = load_sprite( file, Pal );
		}
		TRON( "loading..." );
		sprintf( file, "%ssmovex.wav", passeur );
		loadwav( DEF_dragonx2, file );
	}

	TRON( "\nload_sprites2() - OK." );
}
/********************************************************/
void load_kick1 ( void )
{
	int i;
	TRON( "\nload_kick1()" );
	sprintf( passeur, "sprites" DIR_BAR "%s" DIR_BAR, charname[ sel1 ] );

	for ( i = 1; i < ( p1.skick.nbf + 1 ); ++i )
	{
		sprintf( file, "%sskick%d.pcx", passeur, i );
		
		Player1.Kick[ i ] = xload_pcx( file, Pal );
		
	}
	TRON( "\nload_kick1() - OK" );
}
/********************************************************/
void load_kick2 ( void )
{
	TRON( "\nload_kick2()" );
	sprintf( passeur, "sprites" DIR_BAR "%s" DIR_BAR, charname[ sel2 ] );
	rectfill( zoomscreen, 0, 0, zoomscreen->w, zoomscreen->h, makecol( 255, 0, 255 ) );
	for ( num = 1; num < ( p2.skick.nbf + 1 ); ++num )
	{
		sprintf( file, "%sskick%d.pcx", passeur, num );
		Player2.Kick[ num ] = load_sprite( file, Pal );
	}
	TRON( "\nload_kick2() - OK" );
}
/************************************************************************/
void load_win1 ( void )
{
	sprintf( passeur, "sprites" DIR_BAR "%s" DIR_BAR, charname[ sel1 ] );
	for ( num = 1; num < ( p1.outwin.nbf + 1 ); ++num )
	{
		sprintf( file, "%soutwin%d.pcx", passeur, num );
		Player1.Kick[ num ] = xload_pcx( file , Pal );
	}
}
/*********************************************************/
void load_win2 ( void )
{
	TRON( "\nload_win2()" );
	sprintf( passeur, "sprites" DIR_BAR "%s" DIR_BAR, charname[ sel2 ] );
	rectfill( virtscreen, 0, 0, virtscreen->w, virtscreen->h, makecol( 255, 0, 255 ) );

	for ( num = 1; num < ( p2.outwin.nbf + 1 ); ++num )
	{
		sprintf( file, "%soutwin%d.pcx", passeur, num );
		Player2.Kick[ num ] = load_sprite( file, Pal );
	}
	TRON( "\nload_win2() - OK." );
}
/* END of LOAD FUNCTIONS */

// EOF
