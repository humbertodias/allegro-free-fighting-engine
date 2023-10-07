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
#include "stdio.h"
#include "loads.h"
#include "wavsounds.h"
#include "display.h"
#include "effects.h"
#include "monitor.h" 

/*************************************************************************
  Monitor functions:
  
  The Monitor provide an interface for debug and development of chars and 
    fine adjusts.
    
  void  console_monitor( void ); //Exibe the console monitor
  void  console_print( char *text ); // print in console text
 
 
 patch: ric@asbyte.com.br
**************************************************************************/

BITMAP *monit_screen;
BITMAP *saved_screen;
BITMAP *monit_sprite;
BITMAP *monit_bord;
BITMAP *text_screen;

char line_text[ 80 ];
char char_text;
int pos_text;
char monitor[ 80 ][ 80 ] ; // 15 lines x 80 coluns = 1200 bytes

/***************************************************/
void console_monitor( void )
{
	int i, screen_w, screen_h;

	TRON("console_monitor()");
	screen_w = screen->w;
	screen_h = screen->h;

	monit_sprite = create_bitmap( 320, 100 );
	monit_screen = create_bitmap( 320, 200 );
	saved_screen = create_bitmap( 320, 200 );
	text_screen = create_bitmap( 320, 200 );

	rectfill( monit_sprite, 0, 0, 320, 100, makecol( 0, 50, 0 ) );
	sprintf( file, DIR_SPRITES "monitor.pcx" );
//#if GAME_DEBUG >= 2
//	monit_bord = xload_pcx( file, Pal );
//#endif

//#if GAME_DEBUG < 2 
	monit_bord = xload_pcx( file, Pal );
//#endif	

	stretch_blit ( screen , saved_screen , 0, 0, screen_w, screen_h,
				   0, 0, 320, 200 );
	for ( i = -135;i <= 0;i = i + 9 ) // Exibe monitor
	{
		blit ( saved_screen, monit_screen , 0, 0, 0, 0, 
		       screen_w, screen_h );
		set_trans_blender( 255, 255, 255, 170 );
		draw_trans_sprite( monit_screen, monit_sprite, 0, i );

		draw_sprite( monit_screen, monit_bord, 0, i );
		draw_sprite_v_flip( monit_screen, monit_bord, 
				    320 - monit_bord->w, i );

		stretch_blit ( monit_screen , screen , 0, 0, 320, 200,
				 0, 0, screen_w, screen_h );
	}

	stretch_blit ( screen , monit_screen , 0, 0, screen_w, screen_h,
		   	0, 0, 320, 200 );

	stretch_blit ( screen , text_screen , 0, 0, screen_w, screen_h,
			0, 0, 320, 200 );

	print_buffer_text();
	
	clear_keybuf();
	pos_text = 0;
	sprintf( line_text, "%c", '\0' ); //ESPANTO
	while ( !key[ KEY_ESC ] && !key[ KEY_F12] )
	{
		if ( !keypressed() ) continue;

		char_text = readkey();

		if ( ( char_text >= 32 ) && ( char_text <= 126 ) )
		{
                  line_text[ pos_text++ ] = char_text;
		  if ( pos_text >= 79 ) pos_text = 0;
                        rectfill( screen, 0, 100, 320, 110, 
				 makecol( 0, 0, 0 ) );
                        textout( screen, small_font, line_text,
		                 monit_bord->w + 1, 100,
			         makecol( 0, 255, 0 ) );
		}
		else if ( char_text == 13 )
		{
                      console_print( line_text );
		      exec_console( line_text );
		      pos_text = 0;
		      for ( i = 0;i <= 80;i++ ) line_text[ i ] = 0;
                      stretch_blit ( monit_screen , screen , 0, 0, 320, 200,
		                    0, 0, screen_w, screen_h );
		      print_buffer_text();
		}
		else
		{
                      if ( pos_text > 0 ) pos_text--;
		      line_text[ pos_text ] = 0;
                      rectfill( screen, 0, 100, 320, 110,
                                makecol( 0, 0, 0 ) );
                      textout( screen, small_font, line_text,
                                monit_bord->w + 1, 100,
                                makecol( 0, 255, 0 ) );
		}
	}

	clear_keybuf();
	
	for ( i = 0;i >= -135;i = i - 9 ) // Hide monitor
	{
		blit ( saved_screen, monit_screen , 0, 0, 0, 0, 
		       screen_w, screen_h );
		set_trans_blender( 255, 255, 255, 170 );
		draw_trans_sprite( monit_screen, monit_sprite, 0, i );

		draw_sprite( monit_screen, monit_bord, 0, i );
		draw_sprite_v_flip( monit_screen, monit_bord, 
				    320 - monit_bord->w, i );
		stretch_blit ( monit_screen , screen , 0, 0, 320, 200,
					   0, 0, screen_w, screen_h );
		/*    rest(10); */
	} /* for(=-135... */

	stretch_blit ( saved_screen , screen , 0, 0, 320, 200,
				   0, 0, screen_w, screen_h );

	destroy_bitmap( monit_sprite );
	destroy_bitmap( monit_bord );
	destroy_bitmap( monit_screen );
	destroy_bitmap( saved_screen );
        rest(200);
	clear_keybuf();
	
} /* END void console_monitor( void )*/

void print_buffer_text( void ){
	
int i;
			
   for ( i = 0;i <= 10;i++ ) // print text buffer ...
   {
       text_shadow( screen, small_font, monitor[ i ],
                   monit_bord->w + 2, ( i * 9 ),
                   makecol( 255, 0, 0 ) );
                                    
   } /* for(i=0... */
   rectfill( screen, 0, 100, 320, 110, makecol( 0, 0, 0 ) );

   textout( screen, small_font, line_text,
             monit_bord->w + 1, 100,
             makecol( 0, 255, 0 ) );
}
/**********************************************************/
void console_print( char *text )
{
	int i;
	
	for ( i = 1;i <= 10;i++ ) 
	     sprintf( monitor[ i - 1 ], "%s", monitor[ i ] );
	
	sprintf( monitor[ 10 ], "%s", text );

} /* END void console_print( char text[80] ) */
/*****************************************/
void exec_console( char *text )
{
	char scommand[4][40] ; // Typed commands
	int i, ind_param, ind_word;
	
#if GAME_DEBUG > 0
	// Write in log file typed commands
        sprintf( log_mess,"%s",text );
	TRON( log_mess );
#endif	
	
        for( i=0; i<40; i++){
           scommand[0][i]='\0';
           scommand[1][i]='\0';
	   scommand[2][i]='\0';
	}
        
	ind_param=0; ind_word=0;
        for( i=0; text[i]; i++)
        {
            text[i] = tolower( text[i] ); // set lowercase
            if ( ( text[i]==' ' ) || ( text[i]=='\0' ) )
            {
               ind_param++;
               ind_word=0;
               do
               {
                  i++;
               } while ( text[i]==' ' ); // ignore extra spaces...
            }
            if ( text[i]=='[' )
            {
               i++; // jump the '['
               do
               {
                  scommand[ind_param][ind_word++] = text[i];
                  i++;
               } while ( text[i]!=']' && isprint( text[i]) );
            }else{
                 scommand[ind_param][ind_word++] = text[i];
            }
        }
	
// ----- monitor commands ( only 2 parameters for command )	

// CLS/CLEAR 
	if ( !strcmp( scommand[0], "cls" ) || !strcmp( scommand[0], "clear" ) )
	{
		for ( i = 0;i <= 10;i++ )
		{
			for ( ind_word = 0;ind_word <= 80;ind_word++ )
				monitor[ i ][ ind_word ] = 0;
		}
	}
//------------

// QUIT
	if ( !strcmp( scommand[0], "quit" ) ) exit( 0 );
//------------

// SAVEDISPLAY	
	if ( !strcmp( scommand[0], "savedisplay" ) )
	{
		console_print( "Saving DIAPLAY parameters..." );
		save_display();
		console_print( "READY!" );
	}
//------------

// list [variable] | [initial of variable] 	
	if ( !strcmp( scommand[0], "list" ) ) 
		K_listvars( scommand[1] );		
//------------

// set <variable> [operator] [<integer number>|<variable>]
	if ( !strcmp( scommand[0],"set") )
	        K_setvar( scommand[1], scommand[2], scommand[3] );
//------------

// dev <user_variable>
        if ( !strcmp( scommand[0],"def") )
                K_defvar( scommand[1] );

	if( !strcmp( scommand[0], "fadeout"))
		kof_fade_out( atoi( scommand[1]) );
// tron <menssagem>
	if ( !strcmp( scommand[0],"tron") )
	        TRON( scommand[1] );
// fx1 <f|fx|r|rx|s|sx> 
        if (!strcmp( scommand[0], "fx1" ) )
	 {
             p1.fireb.succ = 0;		    
             p1.firebx.succ = 0;
	     p1.rush.succ = 0;
	     p1.rushx.succ = 0;
	     p1.smove.succ = 0;
	     p1.smovex.succ =0;
	     flag = 0 ; autho_ht1 = 1 ;
             spower1 += 10 ;
		 
            if (p1.fireb.flg && !strcmp(scommand[1],"f")) 
	    {
	       actual = 30;
	       playwav( DEF_hadoken1 );
	       
	    } else if (p1.firebx.flg && !strcmp(scommand[1],"fx"))
	    {
               actual = 31;
               playwav( DEF_hadokenx1 );
	       	
	    }else if (p1.rush.flg && !strcmp(scommand[1],"r"))
	    {
               actual = 40;
	       playwav( DEF_copter1 );
	       
	    }else if (p1.rushx.flg && !strcmp(scommand[1],"rx")) 
	    {
               actual = 41;
	       playwav( DEF_copterx1 );
	       
	    }else if (p1.smove.flg && !strcmp(scommand[1],"s"))
	    {
               actual = 50;
	       playwav( DEF_dragon1 );
	    }else if (p1.smovex.flg && !strcmp(scommand[1],"sx"))
	    {
               actual = 51;
	       playwav( DEF_dragonx1 );
	    }                      
	 }
// fx2 <f|fx|r|rx|s|sx> 
        if (!strcmp( scommand[0], "fx2" ) )
	 {
             p2.fireb.succ = 0;	    
             p2.firebx.succ = 0;
	     p2.rush.succ = 0;
	     p2.rushx.succ = 0;
	     p2.smove.succ = 0;
	     p2.smovex.succ =0;
	     flag2 = 0 ; autho_ht2 = 1 ;
             spower2 += 10 ;
		 
            if ( p2.fireb.flg && !strcmp(scommand[1],"f")) 
	    {
	       actual2 = 30;
	       playwav( DEF_hadoken2 );
	       
	    } else if ( p2.firebx.flg && !strcmp(scommand[1],"fx"))
	    {
               actual2 = 31;
               playwav( DEF_hadokenx2 );

	       
	    }else if (p2.rush.flg && !strcmp(scommand[1],"r"))
	    {
               actual2 = 40;
               playwav( DEF_copter2 );

	    }else if (p2.rushx.flg && !strcmp(scommand[1],"rx")) 
	    {
               actual2 = 41;
	       playwav( DEF_copterx2 );
	       
	    }else if ( p2.smove.flg && !strcmp(scommand[1],"s"))
	    {
               actual2 = 50;
	       playwav( DEF_dragon2 );
	       
	    }else if (p2.smovex.flg && !strcmp(scommand[1],"sx"))
	    {
               actual2 = 51;
	       playwav( DEF_dragonx2 );
	    }                      
	 }
} /*void exec_console...*/
/* ---------------------------- */
void run_script(int modo, int round, char *dir_p1, char *dir_p2)
{
   char namefile[30];
   char text[80];
   char ch=' ';
   int i=0;
   int player;
   char dir_px[100];
   FILE *scriptfile;
 
   
 for(player=2;player>=1;player--)
 {
      if(player==1)
      {
         sprintf(dir_px,"%s",dir_p1);
      }else{
         sprintf(dir_px,"%s",dir_p2);
      }

      /*************************************/
      if ( modo==0 )
      {
        sprintf(namefile, "sprites" DIR_BAR "%s" DIR_BAR "round%d.kof",
                dir_px, round );
      }

      if( modo==1 )
      {
        sprintf(namefile, "sprites" DIR_BAR "%s" DIR_BAR "win%d.kof",
                dir_px, round );
      }

      if( modo==2 )
      {
        sprintf(namefile, "sprites" DIR_BAR "%s" DIR_BAR "lose%d.kof",
                dir_px, round );
      }
      /*******************************/
      sprintf( log_mess, "OPEN:%s", namefile );
      TRON( log_mess );
      if ( ( scriptfile = fopen( namefile, "r" ) ) == ( FILE* ) NULL )
      {
 	sprintf( log_mess, "Script file (%s) not found.\n", namefile );
	TRON( log_mess );
      }else{
	/*******************************/      
        for(i=0;i<80;i++) text[i]='\0';
        i=0;	
        while( !feof(scriptfile) )
        {
	   ch = fgetc(scriptfile); 
	   while( isprint( ch ) )
	   { 
		     if(ch=='!')
		     {
	     	       text[i++] = 'p';
	               text[i++] = (player==1?'1':'2');
		     }else
		     if(ch=='@')
		     {
		       text[i++] = 'p';
		       text[i++] = (player==1?'2':'1');
		     }else 
		       text[i++] = ch;
	     ch = fgetc(scriptfile);	
	   }//while( isprint... 
	   text[i] = '\0';
           exec_console( text ); 
	   i=0;
	//-------------------------------/   
	} //while(!feof...
        fclose( scriptfile );
     } // if(fopen....
 }//for(player...   
}
/**********************************************/

// EOF
