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
#include "config.h"
#include "macros.h"
#include "loads.h"
#include "wavsounds.h"

static SAMPLE *xwav[ DEF_MAX_WAV_SOUNDS ];

static int snd_vol;

/******************************************************/
void set_snd_vol( int xpar ){
  snd_vol = xpar;
}
/******************************************************/
int get_snd_vol(void){
  return snd_vol;
}
/******************************************************/
void freesounds( void )
{
   TRON( "\nfreesounds()" );
   destroy_sample ( xwav[ DEF_rd1 ] );
   destroy_sample ( xwav[ DEF_rd2 ] );
   destroy_sample ( xwav[ DEF_rd3 ] );
   destroy_sample ( xwav[ DEF_fight ] );
   destroy_sample ( xwav[ DEF_wp_alwa ] );
   destroy_sample ( xwav[ DEF_wk_alwa ] );
   destroy_sample ( xwav[ DEF_sp_alwa ] );
   destroy_sample ( xwav[ DEF_sk_alwa ] );
   destroy_sample ( xwav[ DEF_ko ] );
   destroy_sample ( xwav[ DEF_argh ] );
   destroy_sample ( xwav[ DEF_female ] );
   destroy_sample ( xwav[ DEF_done ] );
   destroy_sample ( xwav[ DEF_bing ] );
   destroy_sample ( xwav[ DEF_buzz ] );
   TRON( "\nfreesounds() - OK.\n" );
}
/******************************************************/
void playwav( int wavindex ){
     play_sample( xwav[ wavindex ], snd_vol, DEF_PAN, DEF_PITCH, 0 );
}
/******************************************************/
void loadwav (int wavindex, char *filename){
  xwav[wavindex] = xload_sample ( filename );
}
/******************************************************/
void Buzz ( void )
{
   stop_sample( xwav[DEF_buzz] );
   play_sample( xwav[DEF_buzz], snd_vol, DEF_PAN, DEF_PITCH, 0 );
}
/******************************************************/
void Bing ( void )
{
   play_sample( xwav[DEF_bing], snd_vol, DEF_PAN, DEF_PITCH, 0 );
   rest(10);
}
/******************************************************/
void Done ( void )
{
   play_sample( xwav[DEF_done], snd_vol, DEF_PAN, DEF_PITCH, 0 );
   rest(50);
}
/******************************************************/
void Block ( void )
{
   play_sample( xwav[DEF_block], snd_vol, DEF_PAN, DEF_PITCH, 0 );
}
/******************************************************/
void Paf ( char gana )
{
   switch ( gana )
   {
      case 1 :
         play_sample( xwav[DEF_wp_alwa] , snd_vol, DEF_PAN, DEF_PITCH, 0 );
         break;
      case 2 :
         play_sample( xwav[DEF_sp_alwa] , snd_vol, DEF_PAN, DEF_PITCH, 0 );
         break;
      case 3 :
         play_sample( xwav[DEF_wk_alwa] , snd_vol, DEF_PAN, DEF_PITCH, 0 );
         break;
      case 4 :
         play_sample( xwav[DEF_sk_alwa] , snd_vol, DEF_PAN, DEF_PITCH, 0 );
         break;
   }
}
/******************************************************/
void Hurt ( char gana )
{
   switch ( gana )
   {
      case 1 :
         play_sample( xwav[DEF_hit2] , snd_vol, DEF_PAN, DEF_PITCH, 0 );
         break;
      case 2 :
         play_sample( xwav[DEF_hit1] , snd_vol, DEF_PAN, DEF_PITCH, 0 );
         break;
      default:
         play_sample( xwav[DEF_hit1] , snd_vol, DEF_PAN, DEF_PITCH, 0 );
         break;
   }
}
/******************************************************/
void Round ( char which )
{
   switch ( which )
   {
      case 1 :
         play_sample( xwav[DEF_rd1] , snd_vol, DEF_PAN, DEF_PITCH, 0 );
         break;
      case 2 :
         play_sample( xwav[DEF_rd2] , snd_vol, DEF_PAN, DEF_PITCH, 0 );
         break;
      case 3 :
         play_sample( xwav[DEF_rd3] , snd_vol, DEF_PAN, DEF_PITCH, 0 );
         break;
      default :
         play_sample( xwav[DEF_rd1] , snd_vol, DEF_PAN, DEF_PITCH, 0 );
         break;
   }
}
/******************************************************/
void Fight( void )
{
   play_sample( xwav[DEF_fight] , snd_vol, DEF_PAN, DEF_PITCH, 0 );
}
/******************************************************/
void loadsounds( void )
{
	TRON( "\nloadsounds()" );
	xwav[ DEF_rd1 ] = xload_sample ( "wav" DIR_BAR "rd1.wav" );
	xwav[ DEF_rd2 ] = xload_sample ( "wav" DIR_BAR "rd2.wav" );
	xwav[ DEF_rd3 ] = xload_sample ( "wav" DIR_BAR "rd3.wav" );
	xwav[ DEF_fight ] = xload_sample ( "wav" DIR_BAR "fight.wav" );
	xwav[ DEF_wp_alwa ] = xload_sample ( "wav" DIR_BAR "wp_alwa.wav" );
	xwav[ DEF_wk_alwa ] = xload_sample ( "wav" DIR_BAR "wk_alwa.wav" );
	xwav[ DEF_sp_alwa ] = xload_sample ( "wav" DIR_BAR "sp_alwa.wav" );
	xwav[ DEF_sk_alwa ] = xload_sample ( "wav" DIR_BAR "sk_alwa.wav" );
	xwav[ DEF_ko ] = xload_sample ( "wav" DIR_BAR "ko.wav" );
	xwav[ DEF_argh ] = xload_sample ( "wav" DIR_BAR "argh1.wav" );
	xwav[ DEF_female ] = xload_sample ( "wav" DIR_BAR "argh2.wav" );
	xwav[ DEF_done ] = xload_sample ( "wav" DIR_BAR "done.wav" );
	xwav[ DEF_bing ] = xload_sample ( "wav" DIR_BAR "bing.wav" );
	xwav[ DEF_buzz ] = xload_sample ( "wav" DIR_BAR "buzz.wav" );
	xwav[ DEF_hit1 ] = xload_sample ( "wav" DIR_BAR "hit1.wav" );
	xwav[ DEF_hit2 ] = xload_sample ( "wav" DIR_BAR "hit2.wav" );
	xwav[ DEF_block ] = xload_sample ( "wav" DIR_BAR "block.wav" );
#if TIMEOVER > 0
	xwav[ DEF_to ] = xload_sample ( "wav" DIR_BAR "timeup.wav" );
#endif
#if PERFECT == 1
	xwav[ DEF_prfct ] = xload_sample( "wav" DIR_BAR "perfect.wav" );
#endif
	TRON( "\nloadsounds() - OK." );
}
/*************************************************************************/


// EOF

