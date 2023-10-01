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
#include <allegro.h>
#include "config.h"
#include "global.h"
#include "macros.h"
#include "initfiles.h"

/************************************************************************/
static int rgb[ DEF_MAX_CLONE_PIXELS ];
static int xrgb[ DEF_MAX_CLONE_PIXELS ];
//int i;
static int ii;
static int iii;
static INI_section color_default;
static int palette_found=0;
/************************************************************************/
void set_palette_found( int xset ){
     palette_found=xset;
}
/************************************************************************/
int view_palette_found(void){
    return palette_found;
}
/************************************************************************/
void load_palette( char *file_palette, int nclone ){
      static INI_section clone_palette;
      static char xname[20],yname[20];
      static int r,g,b;
      static FILE *tmpfile;

      if ( ( tmpfile=fopen( file_palette, "r" ) ) == NULL )
      {
            sprintf( log_mess, "ERROR: file (%s) not found! ", file_palette );
            ERROR( log_mess );
         set_palette_found( 0 );
      }else{
        fclose(tmpfile);
        set_palette_found( 1 );
        
// load color_default pixels in rgb[] matrix
        color_default=ini_get_vars(file_palette,"color_default");

	if(color_default.len>DEF_MAX_CLONE_PIXELS) 
		color_default.len=DEF_MAX_CLONE_PIXELS ;
	
        for(i=0;i<color_default.len;i++){
		
            sprintf(xname,"pixel%d",i+1);
            getcolorstr(&r,&g,&b, 
               ini_get_str(file_palette,"color_default",xname,"0,0,0") );
            rgb[i]=makecol( r, g, b );

//	    printf("[%s]=%d,%d,%d %s\n",xname,r,g,b,ini_get_str(file_palette,"color_default",xname,"0,0,0"));
	}
// Sets clone_paletteN           
        sprintf(yname,"clone_palette%.2d",nclone);
        clone_palette=ini_get_vars(file_palette,xname);

// load clone pixels in xrgb[] matrix
        for(i=0;i<color_default.len;i++){
          sprintf(xname,"pixel%d_new",i+1);
          getcolorstr(&r,&g,&b,
             ini_get_str(file_palette,yname,xname,"0,0,0") );
	  
          xrgb[i]=makecol( r, g, b ); 
//          printf("[%s]=%d,%d,%d %s\n",xname,r,g,b,ini_get_str(file_palette,yname,xname,"0,0,0"));

	}
      }	
}
/************************************************************************/
BITMAP * create_palette( BITMAP *sprite )
{
        static BITMAP *tmp_image;
// Create a tmp_image
        tmp_image=create_bitmap(sprite->w,sprite->h);
        rectfill(tmp_image,0,0,sprite->w,sprite->h,makecol(255,0,255));
        draw_sprite (tmp_image,sprite,0,0);

	if( (palette_found=1) )
	{
           for (i = 0; i < tmp_image->w; i++)
           {
             for (ii = 0; ii < tmp_image->h; ii++)
             {
                // perform the exanges 
                for(iii=0;iii<color_default.len;iii++){
                 if ( getpixel(tmp_image, i, ii) == rgb[iii] ){
                      putpixel(tmp_image, i, ii,xrgb[iii]);
		 }
		}
             }// for(ii
           }// for(i
	}
	return tmp_image;
}
/***************************************************************************/

// EOF
