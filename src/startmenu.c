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

#include "config.h"
#include "macros.h"
#include "global.h"
#include "display.h"
#include "refresh.h"
#include "wavsounds.h"
#include "loads.h"

char charname [ 1000 ][ 30 ] ;
char bkgdname [ 200 ][ 30 ] ;
char flcname [ 100 ][ 30 ] ;

/**************************************************/
int startmenu (void)
{
  char preview1_missing='\0';
  char preview2_missing='\0';

  TRON ("step 3 : clear selecteurs");
  rectfill (Selecteur1, 0, 0, Selecteur1->w, Selecteur1->h,
	    makecol (255, 0, 255));
  rectfill (Selecteur2, 0, 0, Selecteur2->w, Selecteur2->h,
	    makecol (255, 0, 255));

  if (gmode)
    p2_sel = 0;
  TRON ("step 4 : load midi");
  if ((music = xload_midi (DIR_MIDI "select.mid")) == NULL)
    {
      ERROR ("error in midi file format");
      //allegro_exit ();
      //exit (0);
    }
  else
    play_midi (music, 1);
  TRON ("step 5 : initialisations");
	/**** initialisations ****/

  //   set_palette ( Pal ) ;

  sel1 = 1;
  cx = 1;
  sel2 = 1;
  cy = 1;

  oldsel1 = 2;
  oldsel2 = 2;

  flag = 0;
  flag2 = 0;
  sens1 = 0;
  sens2 = 0;

  frameskip = 0;

  fight_round = 0;
  vic1 = vic2 = 0;

  actual = 0;
  actual2 = 0;

  last_char = 0;

  ak = ak2 = 0;
  apch = apch2 = 0;

  limak = limak2 = 1;

  limp1 = 1;
  limp2 = 1;
  TRON ("step 7 : clear virtscreen");
  clear (virtscreen);

  TRON ("step 8 : build up selecteurs");

  // este numeros, acho ? s�o os numeros de personagens e bkgs!
  // cx
  int fx;
  for (cx = -4; cx < 7; cx++)
    {
      fx = cx;

      while (fx < 0)
	fx = fx + nbchar;

      while (fx > (nbchar - 1))
	fx = fx - nbchar;
      sprintf (file, DIR_SPRITES
	       "%s" DIR_BAR "selsmall.pcx", charname[fx]);
      if ((Icon2 = xload_pcx (file, Pal)) == NULL)
	Icon2 = xload_pcx (DIR_SPRITES "selmiss.pcx", Pal);

      if (Icon2 != NULL)
	{
	  if (secretchar[fx])
	    {
	      for (x = 0; x < Icon2->w; x++)
		for (y = 0; y < Icon2->h; y++)
		  {
		    if (getpixel (Icon2, x, y) != tcolor)
		      putpixel (Icon2, x, y, bcolor);
		  }
	    }

	  blit (Icon2, Selecteur2, 0, 0, ((cx + 4) * 36) + 4, 1, 26, 26);
	  blit (Icon2, Selecteur1, 0, 0, ((cx + 4) * 36) + 4, 1, 26, 26);

	  if (secretchar[fx])
	    {
	      masked_blit (Lock, Selecteur2, 0, 0, ((cx + 4) * 36) + 4, 1,
			   26, 26);
	      masked_blit (Lock, Selecteur1, 0, 0, ((cx + 4) * 36) + 4, 1,
			   26, 26);
	    }

	}
      else
	{
	  ERROR ("failed loading selmiss.pcx");
	  allegro_exit ();
	  exit (0);
	}

    }
  TRON ("step 9 : load pattern");

  // aqui carregamos a imagen 'kof91', que fica atras dos personagens
  // ou a nova tela de background npattern.pcx via game.ini
  opt = get_config_int ("features", "new_display_bkgd", 0);
  printf ("\n\nopt: %d\n\n", opt);

  if (opt == 0)
    {
      Carton_KO = xload_pcx (DIR_SPRITES "pattern.pcx", Pal);
      drawing_mode (DRAW_MODE_COPY_PATTERN, Carton_KO, 0, 0);
      drawing_mode (DRAW_MODE_SOLID, Carton_KO, 0, 0);
    }
  else
    {
      Carton_KO = xload_pcx (DIR_SPRITES "npattern.pcx", Pal);
    }

  draw_sprite (virtscreen, Carton_KO, 0, 0);

  TRON ("step 10 : Blit");
  blit_KOF91 ();

  clear_keybuf ();

  bgd = 0;
  sol1 = 0;

  cx = cy = fx = fy = 1;

  TRON ("step 11 : retrace_count = 0");

  retrace_count = 0;

  TRON ("step 12 : !story mode");

  // se n�o for o modo story entra
  if (!story)
    {
      while (cy || cx)
	{
	  // Estou desabilitando/habilitando a rotina pra ficar
	  // 'rodando'a imagem kof91 ou n�o...
	  // Aqui, estou fazendo um comparativo para as novas
	  // features presentes no game.ini
	  if (opt == 0)
	    {

	      if (bgd < Carton_KO->w - 1)
		bgd++;
	      else
		bgd = 0;

	      if (sol1 > 0)
		sol1--;
	      else
		sol1 = Carton_KO->h - 1;

	      drawing_mode (DRAW_MODE_COPY_PATTERN, Carton_KO, bgd, sol1);
	    }

	  draw_sprite (virtscreen, Carton_KO, 0, 0);

	  if (opt == 0)
	    {
	      rectfill (virtscreen, 0, 0, 320, 200, 12);
	      drawing_mode (DRAW_MODE_SOLID, 0, 0, 0);
	    }

	  /* update character images if necessary */
	  if ((oldsel1 != sel1) || (oldsel2 != sel2))
	    {

				/*** load the face2 ***/

	      if (p2_sel && (oldsel2 != sel2))
		{

		  sprintf (passeur, DIR_SPRITES
			   "%s" DIR_BAR "static1.pcx", charname[sel2]);

		  sprintf (file2, DIR_SPRITES
			   "%s" DIR_BAR "preview.pcx", charname[sel2]);


		  // Se n�o achar uma personagem ....
		  if (xload_pcx (passeur, Pal) == NULL)
		    {
		      clone = (sel1 == sel2);

		      Face2 = xload_pcx (DIR_SPRITES "none.pcx", Pal);
		      draw_sprite (virtscreen, Face2, 220, 45);
		      Icon2 = xload_pcx (DIR_SPRITES "selmiss.pcx",
					 Pal);
		      sprintf (open2, "No character found");
		      text_centre_shadow (virtscreen, font, open2, 160, 120,
					  sel_player2);
		      strcpy (charn2, open2);
		    }
		  else
		    {
		      // sen�o carrega o selecionado ?
		      sprintf (file, DIR_SPRITES
			       "%s" DIR_BAR "selbig.pcx", charname[sel2]);

		      // aqui eu pego o nome do static1 do personagem 2
		      //sprintf( file2, DIR_SPRITES
		      //              "%s" DIR_BAR "static1.pcx", charname[ sel2 ] );


		      sprintf (passeur,
			       DIR_SPRITES "%s" DIR_BAR "char.ini",
			       charname[sel2]);

		      override_config_file (passeur);

		      if (secretchar[sel2])
			{
			  sprintf (open2, "LOCKED");
			}
		      else
			{

			  sprintf (open2, "%s",
				   get_config_string (charname[sel2], "name",
						      "No name Found 1"));

			  /*
			     open2 = get_config_string(
			     charname[sel2],
			     "name",
			     "No name Found 2"

			     );
			   */
			}
		      /*textout_centre(virtscreen, font, ( secretchar[sel2]?(open2="LOCKED"):(open2 =
		         get_config_string(charname[sel2], "name","No name Found 3"))
		         ),160, 120, sel_player2);
		       */
		      textout_centre (virtscreen, font, open2, 160, 120,
				      sel_player2);

		      strcpy (charn2, open2);

		      clone = (sel1 == sel2);

		      // aqui eu carrego o sprite static.pcx do p2
                      preview2_missing=file_missing( file2 );
		      if (preview2_missing == 0)
			tface2 = load_sprite (file2, Pal);

		      if ( (Face2 = load_sprite (file, Pal)) )
			{
			  if (secretchar[sel2])
			    {
			      for (x = 0; x < Face2->w; x++)
				{
				  for (y = 0; y < Face2->h; y++)
				    {
				      if (getpixel (Face2, x, y) != tcolor)
					putpixel (Face2, x, y, bcolor);
				    }	/* for( y = 0... */
				}	/* for( x = 0... */
			    }	/* if( secretchar... */

                          draw_sprite (virtscreen, Face2, 220, 30);


			  // aqui eu desenho o sprite static.pcx do p2
			  if (preview2_missing == 0)
                         draw_sprite (virtscreen, tface2, 200, 10);


			}
		      else
			{	/* if( Face2 = load_sprite(file, Pal)) */
			  draw_sprite (virtscreen, Face2, 220, 30);

			}	/* if( Face2 = load_sprite(file, Pal)) */

		      masked_blit (Selecteur2, virtscreen, 39, 0, 0, 172,
				   320, 27);
		      rect (virtscreen, 144, 172, 171, 199, sel_player2);

		    }

		}

				/*** load the face1 ***/

	      if (oldsel1 != sel1)
		{

		  sprintf (passeur, DIR_SPRITES
			   "%s" DIR_BAR "static1.pcx", charname[sel1]);

		  //buffer = passeur; // pega static1.pcx do personagem 1
		  //strcpy(buffer, passeur);

		  sprintf (file2, DIR_SPRITES
			   "%s" DIR_BAR "preview.pcx", charname[sel1]);



		  if (xload_pcx (passeur, Pal) == NULL)
		    {
		      Face1 = xload_pcx (DIR_SPRITES "none.pcx", Pal);

		      draw_sprite (virtscreen, Face1, 0, 30);
		      sprintf (open1, "No character found");
		      text_centre_shadow (virtscreen, font,
					  open1, 160, 70, sel_player1);

		      Icon1 =
			xload_pcx (DIR_SPRITES "selmiss.pcx", Pal);

		      draw_sprite (virtscreen, Icon1, 145, 1);

		      strcpy (charn1, open1);
		    }
		  else
		    {

		      sprintf (file, DIR_SPRITES
			       "%s" DIR_BAR "selbig.pcx", charname[sel1]);

		      sprintf (passeur, DIR_SPRITES
			       "%s" DIR_BAR "char.ini", charname[sel1]);

		      override_config_file (passeur);
		      if (secretchar[sel1])
			{
			  sprintf (open1, "LOCKED");
			}
		      else
			{

			  sprintf (open1, "%s",
				   get_config_string (charname[sel1], "name",
						      "No name found 4"));

			}

		      text_centre_shadow (virtscreen, font, open1, 160, 70,
					  sel_player1);
		      sprintf (charn1, "%s", open1);

		      // carrega a static1.pcx do p1
                      preview1_missing=file_missing( file2 );

                      if( preview1_missing==0 )
		         tface1 = xload_pcx (file2, Pal);

		      if ( (Face1 = xload_pcx (file, Pal)) )
			{
                          clone = (sel1==sel2);
                          sprintf (file, DIR_SPRITES
                               "%s" DIR_BAR "selbig.pcx", charname[sel2]);

                          Face2 = load_sprite (file, Pal);
                          draw_sprite (virtscreen, Face2, 220, 30);


                          if (preview2_missing == 0){
                              sprintf (file2, DIR_SPRITES
                                "%s" DIR_BAR "preview.pcx", charname[sel2]);
                               tface2 = load_sprite (file2, Pal);
                               draw_sprite (virtscreen, tface2, 200, 10);
                          }

			  if (secretchar[sel1])
			    {
			      for (x = 0; x < Face1->w; x++)
				{
				  for (y = 0; y < Face1->h; y++)
				    {
				      if (getpixel (Face1, x, y) != tcolor)
					putpixel (Face1, x, y, bcolor);
				    }
				}
			    }

			  draw_sprite (virtscreen, Face1, 0, 30);
			  // desenha a static1.pcx na tela do p1
			  if(preview1_missing==0)
			     draw_sprite (virtscreen, tface1, 20, 10);

			}
		      else
			{
			  Face1 =
			    xload_pcx (DIR_SPRITES "miss.pcx", Pal);
			  draw_sprite (virtscreen, Face1, 0, 30);
			  // desenha a static1.pcx na tela
			  //draw_sprite( virtscreen, tface1, 150, 60 );

			}

		      masked_blit (Selecteur1, virtscreen, 39, 0, 0, 0,
				   320, 27);
		      rect (virtscreen, 144, 0, 171, 27, sel_player1);
		    }

		}

	    }


	  if ((oldsel1 == sel1) || (oldsel2 == sel2))
	    {

	      if (oldsel1 == sel1)
		{
		  if (Face1 != NULL)
		    draw_sprite (virtscreen, Face1, 0, 30);

		  // desenha a static1.pcx na tela do p1
                  if(preview1_missing==0)
		     draw_sprite (virtscreen, tface1, 20, 10);

		  text_centre_shadow (virtscreen, font,
				      (charn1) ? charn1 : "problem", 160, 70,
				      sel_player1);

		  masked_blit (Selecteur1, virtscreen, 39, 0, 0, 0, 320, 27);

		  rect (virtscreen, 144, 0, 171, 27, sel_player1);
		}

	      if (oldsel2 == sel2)
		{
		  if (p2_sel && (Face2 != NULL))
		    {
		      text_centre_shadow (virtscreen, font,
					  (charn2) ? charn2 : "problem", 160,
					  120, sel_player2);

		      draw_sprite (virtscreen, Face2, 220, 30);
		      // desenha a static1.pcx na tela
		      //draw_sprite( virtscreen, tface1, 20, 10 );
		      // desenha a static1.pcx na tela do player 2
		      if (preview2_missing == 0)
			draw_sprite (virtscreen, tface2, 200, 10);


		      masked_blit (Selecteur2, virtscreen, 39, 0, 0, 172,
				   320, 27);

		      rect (virtscreen, 144, 172, 171, 199, sel_player2);

		    }
		}

	    }			// Aqui encerra a rotina de escolha dos personagems

	  retrace_count--;

	  blit_KOF91 ();

	  while (!retrace_count)
	    ;
	  ;

	  rest (20);

	  if (key[KEY_F1] && key[KEY_F5] && key[KEY_F9])
	    {
	      for (x = 0; x < nbchar; x++)
		secretchar[x] = 0;
	      rectfill (virtscreen, 10, 64, 310, 90, sel_player1);
	      text_centre_shadow (virtscreen, font,
				  "SECRET CHARACTERS UNLOCKED", 160, 70,
				  bcolor);
	      blit_KOF91 ();
	      Done ();
	      rest (700);
	    }


	  if (key[KEY_ESC])
	    {
	      /* goto Demoniac ; */
	      return 1;
	    }
#if SCREENSHOT > 0
	  if (key[KEY_F2])
	    screenshot ();
#endif

	  if (fy < 1)
	    fy++;

	  else
	    fy = 0;

	  if (fy < 1)
	    {

	      if (cy && p2_sel)
		{
		  oldsel2 = sel2;

		  poll_joystick ();

		  if ((!gmode && ((P2_SP) || (P2_SK)
				  || (P2_WP)
				  || (P2_WK))) || (gmode
						   && (P1_SP
						       || P1_WP
						       || P1_SK || P1_WK)))
		    {
		      sprintf (passeur, DIR_SPRITES
			       "%s" DIR_BAR "static1.pcx", charname[sel2]);

		      if ((xload_pcx (passeur, Pal) == NULL)
			  || secretchar[sel2])
			Buzz ();

		      else
			{
			  Done ();
			  cy = 0;
			}
		    }

		  if ((P2_LT && !gmode) || (gmode && P1_LT))
		    {
		      Bing ();
		      sel2--;
		      if (sel2 == -1)
			sel2 = nbchar - 1;

		      fx = sel2 - 5;
		      while (fx < 0)
			fx = fx + nbchar;

		      sprintf (file, DIR_SPRITES
			       "%s" DIR_BAR "selsmall.pcx", charname[fx]);

		      if ((Icon2 = xload_pcx (file, Pal)) == NULL)
			Icon2 =
			  xload_pcx (DIR_SPRITES "selmiss.pcx", Pal);
		      else if (secretchar[fx])
			{
			  for (x = 0; x < Icon2->w; x++)
			    for (y = 0; y < Icon2->h; y++)
			      {
				if (getpixel (Icon2, x, y) != tcolor)
				  putpixel (Icon2, x, y, bcolor);
			      }
			}

		      blit (Selecteur2, Selecteur2, 0, 0, 36, 0, 360, 27);

		      blit (Icon2, Selecteur2, 0, 0, 4, 1, 26, 26);

		      if (secretchar[fx])
			masked_blit (Lock, Selecteur2, 0, 0, 4, 1, 26, 26);

		    }

		  if ((P2_RT && !gmode) || (gmode && P1_RT))
		    {
		      Bing ();
		      sel2++;
		      if (sel2 == nbchar)
			sel2 = 0;

		      fx = sel2 + 5;
		      while (fx > (nbchar - 1))
			fx = fx - nbchar;

		      sprintf (file, DIR_SPRITES
			       "%s" DIR_BAR "selsmall.pcx", charname[fx]);

		      if ((Icon2 = xload_pcx (file, Pal)) == NULL)
			Icon2 =
			  xload_pcx (DIR_SPRITES "selmiss.pcx", Pal);
		      else if (secretchar[fx])
			{
			  for (x = 0; x < Icon2->w; x++)
			    for (y = 0; y < Icon2->h; y++)
			      {
				if (getpixel (Icon2, x, y) != tcolor)
				  putpixel (Icon2, x, y, bcolor);
			      }
			}

		      blit (Selecteur2, Selecteur2, 36, 0, 0, 0, 360, 27);

		      blit (Icon2, Selecteur2, 0, 0, 364, 1, 26, 26);

		      if (secretchar[fx])
			masked_blit (Lock, Selecteur2, 0, 0, 364, 1, 26, 26);

		    }
		}


	      if (cx)
		{
		  oldsel1 = sel1;

		  poll_joystick ();

		  if (P1_WK || P1_SK || P1_WP || P1_SP)
		    {

		      sprintf (passeur, DIR_SPRITES
			       "%s" DIR_BAR "static1.pcx", charname[sel1]);

		      if ((xload_pcx (passeur, Pal) == NULL)
			  || secretchar[sel1])
			Buzz ();

		      else
			{
			  Done ();
			  if (gmode == 2)
			    {
			      retrace_count = 0;
			      story = 1;
			      last_sel1 = sel1;
			      rest (100);
			      poll_joystick ();
			      clear_keybuf ();
			      cy = 0;
			    }

			  p2_sel = 1;
			  oldsel2 = 2;
			  rest (80);
			  poll_joystick ();
			  clear_keybuf ();
			  cx = 0;
			}
		    }

		  if (P1_LT)
		    {
		      Bing ();
		      sel1--;
		      if (sel1 == -1)
			sel1 = nbchar - 1;

		      fx = sel1 - 5;
		      while (fx < 0)
			fx = fx + nbchar;

		      sprintf (file, DIR_SPRITES
			       "%s" DIR_BAR "selsmall.pcx", charname[fx]);

		      if ((Icon1 = xload_pcx (file, Pal)) == NULL)
			Icon1 =
			  xload_pcx (DIR_SPRITES "selmiss.pcx", Pal);
		      else if (secretchar[fx])
			{
			  for (x = 0; x < Icon1->w; x++)
			    for (y = 0; y < Icon1->h; y++)
			      {
				if (getpixel (Icon1, x, y) != tcolor)
				  putpixel (Icon1, x, y, bcolor);
			      }
			}


		      blit (Selecteur1, Selecteur1, 0, 0, 36, 0, 360, 27);

		      blit (Icon1, Selecteur1, 0, 0, 4, 1, 26, 26);

		      if (secretchar[fx])
			masked_blit (Lock, Selecteur1, 0, 0, 4, 1, 26, 26);
		    }

		  if (P1_RT)
		    {
		      Bing ();
		      sel1++;
		      if (sel1 == nbchar)
			sel1 = 0;

		      fx = sel1 + 5;
		      while (fx > (nbchar - 1))
			fx = fx - nbchar;

		      sprintf (file, DIR_SPRITES
			       "%s" DIR_BAR "selsmall.pcx", charname[fx]);

		      if ((Icon1 = xload_pcx (file, Pal)) == NULL)
			Icon1 =
			  xload_pcx (DIR_SPRITES "selmiss.pcx", Pal);
		      else if (secretchar[fx])
			{
			  for (x = 0; x < Icon1->w; x++)
			    for (y = 0; y < Icon1->h; y++)
			      {
				if (getpixel (Icon1, x, y) != tcolor)
				  putpixel (Icon1, x, y, bcolor);
			      }
			}

		      blit (Selecteur1, Selecteur1, 36, 0, 0, 0, 360, 27);

		      blit (Icon1, Selecteur1, 0, 0, 364, 1, 26, 26);

		      if (secretchar[fx])
			masked_blit (Lock, Selecteur1, 0, 0, 364, 1, 26, 26);
		    }


		}

	    }

	}

    }


  clear (virtscreen);
  clear (scanliner);

  clear_keybuf ();

  retrace_count = 0;

  if (story)
    {
      sel1 = last_sel1;

      cy = cx = 1;

      for (x = 0; x < nbchar; x++)
	{
	  if (!defeated[x] && !secretchar[x] && present[x])
	    cy = 0;
	}

      if (cy)			/* end of game */
	{
	  for (x = 0; x < nbchar; x++)
	    secretchar[x] = 0;

	  if ((Selecteur1 = xload_pcx (DIR_SPRITES "end.pcx", 0)) ==
	      NULL)
	    {
	      ERROR ("end.pcx missing");
	      exit (0);
	    }

	  cx = 1;
	  while (cx)
	    {
	      if (opt == 0)
		{
		  if (bgd < Carton_KO->w - 1)
		    bgd++;
		  else
		    bgd = 0;

		  if (sol1 > 0)
		    sol1--;
		  else
		    sol1 = Carton_KO->h - 1;

		  drawing_mode (DRAW_MODE_COPY_PATTERN, Carton_KO, bgd, sol1);
		}

	      draw_sprite (virtscreen, Carton_KO, 0, 0);

	      if (opt == 0)
		{
		  rectfill (virtscreen, 0, 0, 320, 200, 12);
		  drawing_mode (DRAW_MODE_SOLID, 0, 0, 0);
		}

	      retrace_count--;

	      masked_blit (Selecteur1, virtscreen, 0, 0, 0, 0, 320, 200);

	      if (cx == 2)
		{
		  text_centre_shadow (virtscreen, font, "GAME",
				      240, 70, sel_player1);
		  text_centre_shadow (virtscreen, font, "OVER",
				      240, 90, sel_player1);
		}


	      blit_KOF91 ();

	      while (!retrace_count)
		;
	      ;

	      rest (20);


	      if ((P2_SP) || (P2_SK)
		  || (P2_WP) || (P2_WK)
		  || (key[KEY_ENTER]) || P1_WK || P1_SK || P1_WP || P1_SP)
		{
		  if (cx == 1)
		    {

		      sprintf (passeur, DIR_SPRITES
			       "%s" DIR_BAR "endimg.pcx", charname[sel1]);

		      if ((Selecteur2 = xload_pcx (passeur, 0)) != NULL)
			{
			  Selecteur1 = Selecteur2;
			  cx = 2;
			  clear_keybuf ();
			}

		      else
			cx = 0;
		    }

		  else
		    cx = 0;

		}


	    }

	  /* goto Demoniac ; */
	  // � o incio da engine :)
	  return 1;
	}

      cy = 1;

      while (cy)
	{
	  sel2 = rand () % nbchar;

	  if (!defeated[sel2] && !secretchar[sel2] & present[sel2])
	    cy = 0;
	}

      sprintf (file, DIR_SPRITES
	       "%s" DIR_BAR "selbig.pcx", charname[sel2]);

      sprintf (passeur, DIR_SPRITES
	       "%s" DIR_BAR "char.ini", charname[sel2]);

      override_config_file (passeur);

      clone = (sel1 == sel2);
      TRON ("Primeiro TRON estranho");
#if GAME_DEBUG > 0
      sprintf (log_mess, "var:clone:%d\n", clone);
      TRON ("Tron estranho...");
      TRON (log_mess);
#endif
      TRON ("OK...");
      Face2 = load_sprite (file, Pal);

      sprintf (passeur, DIR_SPRITES
	       "%s" DIR_BAR "selbig.pcx", charname[sel1]);

      Face1 = xload_pcx (passeur, Pal);

      bgd = 0;
      sol1 = 0;

      cx = 1;

      while (cx)
	{
	  if (key[KEY_ESC])
	    {
	      /* goto Demoniac ; */
	      return 1;
	    }

	  if (opt == 0)
	    {
	      if (bgd < Carton_KO->w - 1)
		bgd++;
	      else
		bgd = 0;

	      if (sol1 > 0)
		sol1--;
	      else
		sol1 = Carton_KO->h - 1;

	      drawing_mode (DRAW_MODE_COPY_PATTERN, Carton_KO, bgd, sol1);
	    }

	  draw_sprite (virtscreen, Carton_KO, 0, 0);

	  if (opt == 0)
	    {
	      rectfill (virtscreen, 0, 0, 320, 200, 12);
	      drawing_mode (DRAW_MODE_SOLID, 0, 0, 0);
	    }

	  retrace_count--;

	  if (Face2 != NULL)
	    {
              clone =(sel1 == sel2 );
	      draw_sprite(virtscreen, Face2, 220, 30);
	    }

	  if (Face1 != NULL)
	    draw_sprite (virtscreen, Face1, 0, 30);

	  text_centre_shadow (virtscreen, font, "NEXT OPPONENT",
			      160, 70, sel_player1);


	  sprintf (open2, "%s",
		   get_config_string (charname[sel2], "name",
				      "No name found 7"));

	  /*            open2 =
	     get_config_string(sectn, "name","No name found 8");
	   */


	  text_centre_shadow (virtscreen, font, open2, 160, 120, sel_player2);

	  sprintf (charn2, "%s", open2);

	  blit_KOF91 ();

	  while (!retrace_count)
	    ;
	  ;

	  rest (20);


	  if ((P2_SP) || (P2_SK)
	      || (P2_WP) || (P2_WK)
	      || (key[KEY_ENTER]) || P1_WK || P1_SK || P1_WP || P1_SP)
	    {
	      Done ();
	      cx = 0;
	    }


	}


    }

  clear (virtscreen);

  blit_KOF91 ();


  // INICIO DA SESS�O QUE CARREGA O BACKGROUND !!!!!

#if GAME_DEBUG > 0
  sprintf (log_mess, "\ncheck strings :\n  char1 : %s\n  char2 : %s\n",
	   charname[sel1], charname[sel2]);
  TRON (log_mess);
#endif

  rest (15);
  poll_joystick ();
  clear_keybuf ();

  cx = 1;
  bgd = 1;

  Carton_R1 = create_bitmap (260, 160);
  sprintf (passeur, "bkgds" DIR_BAR "%s", bkgdname[(int)bgd]);

  if ((Carton_R2 = xload_pcx (passeur, Pal)) == NULL)
    {

      if ((open_fli (passeur) == FLI_OK))
	{
	  Carton_R2 = create_bitmap (320, 200);
	  reset_fli_variables ();
	  next_fli_frame (1);
	  set_palette (fli_palette);
	  blit (fli_bitmap, Carton_R2, (fli_bitmap->w / 2) - 160,
		fli_bitmap->h - 200, 0, 0, 320, 200);
	  close_fli ();
	}
      else
	{
	  ERROR ("Can't open .flc file!");
	  exit (0);
	}


    }

  stretch_blit (Carton_R2, Carton_R1, (Carton_R2->w / 2) - 160,
		Carton_R2->h - 200, 320, 200, 0, 0, 260, 160);

  /*draw_sprite( virtscreen , Carton_R1 , 30 , 10 ); */


  fx = 1;

  retrace_count = 0;

  poll_joystick ();

  while (cx)
    {
      if (opt == 0)
	{
	  if (sol2 < Carton_KO->w - 1)
	    sol2++;
	  else
	    sol2 = 0;

	  if (sol1 > 0)
	    sol1--;
	  else
	    sol1 = Carton_KO->h - 1;

	  drawing_mode (DRAW_MODE_COPY_PATTERN, Carton_KO, sol2, sol1);
	}

      draw_sprite (virtscreen, Carton_KO, 0, 0);

      if (opt == 0)
	{
	  rectfill (virtscreen, 0, 0, 320, 200, 12);
	  drawing_mode (DRAW_MODE_SOLID, 0, 0, 0);
	}

      draw_sprite (virtscreen, Carton_R1, 30, 10);

      text_centre_shadow (virtscreen, font, "< Select Background >",
			  160, 180, sel_player1);

      blit_KOF91 ();

      poll_joystick ();

      while (!retrace_count)
	;
      ;

      rest (20);


      //rest(10);

      poll_joystick ();


      if (fx < 2)
	fx++;

      else
	fx = 0;

      if (fx < 1)
	{

	  if ((P2_SP) || (P2_SK)
	      || (P2_WP) || (P2_WK)
	      || (key[KEY_ENTER]) || P1_WK || P1_SK || P1_WP || P1_SP)
	    {
	      Done ();
	      cx = 0;
	    }

	  if (P1_LT || (P2_LT))
	    {
	      bgd--;

	      while (bgd < 0)
		bgd = bgd + nbbkgd;
	      sprintf (passeur, "bkgds" DIR_BAR "%s", bkgdname[(int)bgd]);

	      if ((Carton_R2 = xload_pcx (passeur, Pal)) == NULL)
		{
		  if ((open_fli (passeur) == FLI_OK))
		    {
		      Carton_R2 = create_bitmap (320, 200);
		      reset_fli_variables ();
		      next_fli_frame (1);
		      set_palette (fli_palette);
		      blit (fli_bitmap, Carton_R2,
			    (fli_bitmap->w / 2) - 160,
			    fli_bitmap->h - 200, 0, 0, 320, 200);
		      close_fli ();
		    }
		  else
		    {
		      ERROR ("Can't open .flc file.");
		      exit (0);
		    }
		}

	      stretch_blit (Carton_R2, Carton_R1,
			    (Carton_R2->w / 2) - 160, Carton_R2->h - 200,
			    320, 200, 0, 0, 260, 160);

	      Bing ();

	    }

	  if (P1_RT || (P2_RT))
	    {
	      bgd++;

	      while (bgd > (nbbkgd - 1))
		bgd = bgd - nbbkgd;

	      sprintf (passeur, "bkgds" DIR_BAR "%s", bkgdname[(int)bgd]);

	      if ((Carton_R2 = xload_pcx (passeur, Pal)) == NULL)
		{
		  if ((open_fli (passeur) == FLI_OK))
		    {
		      Carton_R2 = create_bitmap (320, 200);
		      reset_fli_variables ();
		      next_fli_frame (1);
		      set_palette (fli_palette);
		      blit (fli_bitmap, Carton_R2,
			    (fli_bitmap->w / 2) - 160,
			    fli_bitmap->h - 200, 0, 0, 320, 200);
		      close_fli ();
		    }
		  else
		    {
		      ERROR ("Can't open .flc file.");
		      exit (0);
		    }
		}

	      stretch_blit (Carton_R2, Carton_R1,
			    (Carton_R2->w / 2) - 160, Carton_R2->h - 200,
			    320, 200, 0, 0, 260, 160);

	      Bing ();
	    }


	}


    }


  TRON ("\ndestroy_midi(music)");
  destroy_midi (music);
  TRON ("\nOK. destroy_midi(music)");

#if GAME_DEBUG > 0
  sprintf (log_mess, "\ncheck bkgd path :\n %s", passeur);
  TRON (log_mess);
#endif

  /*   fade_out( 2 ); */

  clear (screen);
  clear (virtscreen);

  // aqui ele mostra na hora que carrega a luta loading...
  text_centre_shadow (virtscreen, font, "Now Loading... please wait", 160,
		      150, makecol (255, 255, 255));
  rect (virtscreen, 20, 170, 280, 190, sel_player2);

  //carrega o logo do kof2003 no now loading...
  tface1 = xload_pcx (DIR_SPRITES "kofload.pcx", Pal);
  draw_sprite (virtscreen, tface1, 0, 0);

  //rest (500); //pra testar e ver a imagem

  blit_KOF91 ();

	/******* GET CHARACTER 2 INFO *********************/

  last_sel2 = sel2;

  strcpy (sectn, charname[sel2]);

  sprintf (passeur, DIR_SPRITES
	   "%s" DIR_BAR "char.ini", charname[sel2]);

#if GAME_DEBUG > 0
  sprintf (log_mess, "check char1 path :\n %s", passeur);
  TRON (log_mess);
#endif


  override_config_file (passeur);

  p2.width = get_config_int (sectn, "static_w", 100);
  p2.spunch.w = get_config_int (sectn, "spunch_w", 150);
  p2.wpunch.w = get_config_int (sectn, "wpunch_w", 150);
  p2.skick.w = get_config_int (sectn, "skick_w", 150);
  p2.wkick.w = get_config_int (sectn, "wkick_w", 150);
  p2.apunch.w = get_config_int (sectn, "apunch_w", 150);
  p2.gpunch.w = get_config_int (sectn, "gpunch_w", 150);
  p2.akick.w = get_config_int (sectn, "akick_w", 150);
  p2.gkick.w = get_config_int (sectn, "gkick_w", 150);
  p2.intro.w = get_config_int (sectn, "intro_w", 150);
  p2.ko.w = get_config_int (sectn, "ko_w", 150);
  p2.outwin.w = get_config_int (sectn, "outwin_w", 150);

  p2.height = get_config_int (sectn, "height", 130);

  p2.statik.nbf = get_config_int (sectn, "static_nbf", 4);

  p2.walk.nbf = get_config_int (sectn, "walk_nbf", 5);

  p2.spunch.nbf = get_config_int (sectn, "spunch_nbf", 3);

  p2.wpunch.nbf = get_config_int (sectn, "wpunch_nbf", 3);

  p2.skick.nbf = get_config_int (sectn, "skick_nbf", 4);

  p2.wkick.nbf = get_config_int (sectn, "wkick_nbf", 4);

  p2.apunch.nbf = get_config_int (sectn, "apunch_nbf", 1);

  p2.gpunch.nbf = get_config_int (sectn, "gpunch_nbf", 4);

  p2.akick.nbf = get_config_int (sectn, "akick_nbf", 1);

  p2.gkick.nbf = get_config_int (sectn, "gkick_nbf", 4);

  p2.hurt.nbf = get_config_int (sectn, "hurt_nbf", 1);

  p2.ghurt.nbf = get_config_int (sectn, "ghurt_nbf", 1);

  p2.intro.nbf = get_config_int (sectn, "intro_nbf", 1);

  p2.outwin.nbf = get_config_int (sectn, "outwin_nbf", 1);

  p2.statik.spd = get_config_int (sectn, "static_spd", 8);
  p2.walk.spd = get_config_int (sectn, "walk_spd", 8);
  p2.spunch.spd = get_config_int (sectn, "spunch_spd", 8);
  p2.wpunch.spd = get_config_int (sectn, "wpunch_spd", 8);
  p2.skick.spd = get_config_int (sectn, "skick_spd", 8);
  p2.wkick.spd = get_config_int (sectn, "wkick_spd", 8);
  p2.apunch.spd = get_config_int (sectn, "apunch_spd", 8);
  p2.gpunch.spd = get_config_int (sectn, "gpunch_spd", 8);
  p2.akick.spd = get_config_int (sectn, "akick_spd", 8);
  p2.gkick.spd = get_config_int (sectn, "gkick_spd", 8);
  p2.outwin.spd = get_config_int (sectn, "outwin_spd", 10);

  p2.spunch.hit = get_config_int (sectn, "spunch_hit", 2);
  p2.wpunch.hit = get_config_int (sectn, "wpunch_hit", 2);
  p2.skick.hit = get_config_int (sectn, "skick_hit", 2);
  p2.wkick.hit = get_config_int (sectn, "wkick_hit", 2);
  p2.apunch.hit = get_config_int (sectn, "apunch_hit", 2);
  p2.gpunch.hit = get_config_int (sectn, "gpunch_hit", 2);
  p2.akick.hit = get_config_int (sectn, "akick_hit", 2);
  p2.gkick.hit = get_config_int (sectn, "gkick_hit", 2);

  p2.spunch.dmg = get_config_int (sectn, "spunch_dmg", 2);
  p2.wpunch.dmg = get_config_int (sectn, "wpunch_dmg", 1);
  p2.skick.dmg = get_config_int (sectn, "skick_dmg", 2);
  p2.wkick.dmg = get_config_int (sectn, "wkick_dmg", 1);
  p2.apunch.dmg = get_config_int (sectn, "apunch_dmg", 1);
  p2.gpunch.dmg = get_config_int (sectn, "gpunch_dmg", 1);
  p2.akick.dmg = get_config_int (sectn, "akick_dmg", 2);
  p2.gkick.dmg = get_config_int (sectn, "gkick_dmg", 2);

  p2.spunch.offx = get_config_int (sectn, "spunch_offx", p2.spunch.w);
  p2.wpunch.offx = get_config_int (sectn, "wpunch_offx", p2.wpunch.w);
  p2.skick.offx = get_config_int (sectn, "skick_offx", p2.skick.w);
  p2.wkick.offx = get_config_int (sectn, "wkick_offx", p2.wkick.w);
  p2.apunch.offx = get_config_int (sectn, "apunch_offx", p2.apunch.w);
  p2.gpunch.offx = get_config_int (sectn, "gpunch_offx", p2.gpunch.w);
  p2.akick.offx = get_config_int (sectn, "akick_offx", p2.akick.w);
  p2.gkick.offx = get_config_int (sectn, "gkick_offx", p2.gkick.w);

  p2.spunch.offy = get_config_int (sectn, "spunch_offy", p2.height - 50);
  p2.wpunch.offy = get_config_int (sectn, "wpunch_offy", p2.height - 50);
  p2.skick.offy = get_config_int (sectn, "skick_offy", p2.height - 50);
  p2.wkick.offy = get_config_int (sectn, "wkick_offy", p2.height - 50);
  p2.apunch.offy = get_config_int (sectn, "apunch_offy", p2.height - 50);
  p2.gpunch.offy = get_config_int (sectn, "gpunch_offy", p2.height - 50);
  p2.akick.offy = get_config_int (sectn, "akick_offy", p2.height - 50);
  p2.gkick.offy = get_config_int (sectn, "gkick_offy", p2.height - 50);


  p2.statik.defx = get_config_int (sectn, "static_defx", p2.width);
  p2.walk.defx = get_config_int (sectn, "walk_defx", p2.width);
  p2.jump.defx = get_config_int (sectn, "jump_defx", p2.width);
  p2.crouch.defx = get_config_int (sectn, "crouch_defx", p2.width);
  p2.spunch.defx = get_config_int (sectn, "spunch_defx", p2.width);
  p2.wpunch.defx = get_config_int (sectn, "wpunch_defx", p2.width);
  p2.skick.defx = get_config_int (sectn, "skick_defx", p2.width);
  p2.wkick.defx = get_config_int (sectn, "wkick_defx", p2.width);
  p2.apunch.defx = get_config_int (sectn, "apunch_defx", p2.width);
  p2.gpunch.defx = get_config_int (sectn, "gpunch_defx", p2.width);
  p2.akick.defx = get_config_int (sectn, "akick_defx", p2.width);
  p2.gkick.defx = get_config_int (sectn, "gkick_defx", p2.width);

  p2.statik.defy = get_config_int (sectn, "static_defy", p2.height - 50);
  p2.walk.defy = get_config_int (sectn, "walk_defy", p2.height - 50);
  p2.jump.defy = get_config_int (sectn, "jump_defy", p2.height - 50);
  p2.crouch.defy = get_config_int (sectn, "crouch_defy", p2.height - 50);
  p2.spunch.defy = get_config_int (sectn, "spunch_defy", p2.height - 50);
  p2.wpunch.defy = get_config_int (sectn, "wpunch_defy", p2.height - 50);
  p2.skick.defy = get_config_int (sectn, "skick_defy", p2.height - 50);
  p2.wkick.defy = get_config_int (sectn, "wkick_defy", p2.height - 50);
  p2.apunch.defy = get_config_int (sectn, "apunch_defy", p2.height - 50);
  p2.gpunch.defy = get_config_int (sectn, "gpunch_defy", p2.height - 50);
  p2.akick.defy = get_config_int (sectn, "akick_defy", p2.height - 50);
  p2.gkick.defy = get_config_int (sectn, "gkick_defy", p2.height - 50);


  female2 = get_config_int (sectn, "female", 0);

  if (gmode)
    {
      switch (diff)
	{

	case 0:
	  offense = get_config_int (sectn, "easy_off", easy_off);
	  defense = get_config_int (sectn, "easy_def", easy_def);
	  break;

	case 1:
	  offense = get_config_int (sectn, "medium_off", medium_off);
	  defense = get_config_int (sectn, "medium_def", medium_def);
	  break;

	case 2:
	  offense = get_config_int (sectn, "hard_off", hard_off);
	  defense = get_config_int (sectn, "hard_def", hard_def);
	  break;

	default:
	  offense = get_config_int (sectn, "medium_off", medium_off);
	  defense = get_config_int (sectn, "medium_def", medium_def);
	  break;

	}

    }

  for (i = 0; i < 2; i++)
    temp_strg[i] = 0;

  p2.fireb.flg = get_config_int (sectn, "fireb", 0);

  if (p2.fireb.flg)
    {
      p2.fball.w = get_config_int (sectn, "fball_w", 1);
      p2.fball.nbf = get_config_int (sectn, "fball_nbf", 1);
      p2.fball.spd = get_config_int (sectn, "fball_spd", 1);
      p2.fball.pix = get_config_int (sectn, "fball_pix", 1);
      p2.fball.offx = get_config_int (sectn, "fball_offx", p2.fball.w);
      p2.fball.offy = get_config_int (sectn, "fball_offy", 10);


      fball_h2 = get_config_int (sectn, "fball_h", 1);
      fball_cx2 = get_config_int (sectn, "fball_cx", 1);
      fball_cy2 = get_config_int (sectn, "fball_cy", 1);

      p2.fireb.w = get_config_int (sectn, "fireb_w", 1);
      p2.fireb.nbf = get_config_int (sectn, "fireb_nbf", 1);
      p2.fireb.spd = get_config_int (sectn, "fireb_spd", 1);
      p2.fireb.hit = get_config_int (sectn, "fireb_hit", 1);


      sprintf (p2.fireb.exec, "%s",
	       get_config_string (sectn, "fireb_exec", ""));

      /*      p2.fireb.exec =
         get_config_string(sectn, "fireb_exec", "");
       */
      p2.fireb.dmg = get_config_int (sectn, "fireb_dmg", 3);

      cx = 1;
      nb = 0;
      i = 0;
      y = 0;

      while (cx)
	{
	  c = p2.fireb.exec[i];

	  if (c != ' ')
	    {
	      if (c == '*')
		{
		  p2.fireb.end = nb;
		  cx = 0;
		}

	      temp_strg[y] = c;
	      y++;
	    }
	  else
	    {
	      p2.fireb.seq[nb] = get_kstatus (temp_strg);
	      nb++;
	      y = 0;
	    }

	  i++;
	}

    }


  for (i = 0; i < 2; i++)
    temp_strg[i] = 0;

  p2.rush.flg = get_config_int (sectn, "rush", 0);

  if (p2.rush.flg)
    {
      p2.rush.w = get_config_int (sectn, "rush_w", 1);
      p2.rush.nbf = get_config_int (sectn, "rush_nbf", 1);
      p2.rush.spd = get_config_int (sectn, "rush_spd", 1);
      p2.rush.hit = get_config_int (sectn, "rush_hit", 1);
      p2.rush.rot = get_config_int (sectn, "rush_rot", 1);
      p2.rush.pix = get_config_int (sectn, "rush_pix", 1);


      sprintf (p2.rush.exec, "%s",
	       get_config_string (sectn, "rush_exec", ""));

      /*      p2.rush.exec=
         get_config_string(sectn, "rush_exec", "") ;
       */
      p2.rush.dmg = get_config_int (sectn, "rush_dmg", 3);
      p2.rush.offx = get_config_int (sectn, "rush_offx", p2.rush.w);
      p2.rush.offy = get_config_int (sectn, "rush_offy", 10);


      cx = 1;
      nb = 0;
      i = 0;
      y = 0;

      while (cx)
	{
	  c = p2.rush.exec[i];

	  if (c != ' ')
	    {
	      if (c == '*')
		{
		  p2.rush.end = nb;
		  cx = 0;
		}

	      temp_strg[y] = c;
	      y++;
	    }
	  else
	    {
	      p2.rush.seq[nb] = get_kstatus (temp_strg);
	      nb++;
	      y = 0;
	    }

	  i++;
	}

    }

  for (i = 0; i < 2; i++)
    temp_strg[i] = 0;

  p2.smove.flg = get_config_int (sectn, "smove", 0);

  if (p2.smove.flg)
    {
      p2.smove.w = get_config_int (sectn, "smove_w", 1);
      p2.smove.nbf = get_config_int (sectn, "smove_nbf", 1);
      p2.smove.spd = get_config_int (sectn, "smove_spd", 1);
      p2.smove.hit = get_config_int (sectn, "smove_hit", 1);
      p2.smove.rot = get_config_int (sectn, "smove_rot", 1);


      sprintf (p2.smove.exec, "%s",
	       get_config_string (sectn, "smove_exec", ""));

      /*
         p2.smove.exec=
         get_config_string(sectn, "smove_exec", "");

       */
      p2.smove.dmg = get_config_int (sectn, "smove_dmg", 3);
      p2.smove.offx = get_config_int (sectn, "smove_offx", p2.smove.w);
      p2.smove.offy = get_config_int (sectn, "smove_offy", 10);

      cx = 1;
      nb = 0;
      i = 0;
      y = 0;

      while (cx)
	{
	  c = p2.smove.exec[i];

	  if (c != ' ')
	    {
	      if (c == '*')
		{
		  p2.smove.end = nb;
		  cx = 0;
		}

	      temp_strg[y] = c;
	      y++;
	    }
	  else
	    {
	      p2.smove.seq[nb] = get_kstatus (temp_strg);
	      nb++;
	      y = 0;
	    }

	  i++;
	}

    }

  for (i = 0; i < 2; i++)
    temp_strg[i] = 0;

  p2.firebx.flg = get_config_int (sectn, "firebx", 0);

  if (p2.firebx.flg)
    {
      p2.fballx.w = get_config_int (sectn, "fballx_w", 1);
      p2.fballx.nbf = get_config_int (sectn, "fballx_nbf", 1);
      p2.fballx.spd = get_config_int (sectn, "fballx_spd", 1);
      p2.fballx.pix = get_config_int (sectn, "fballx_pix", 1);
      p2.fballx.offx = get_config_int (sectn, "fballx_offx", p2.fballx.w);
      p2.fballx.offy = get_config_int (sectn, "fballx_offy", 10);


      fballx_h2 = get_config_int (sectn, "fballx_h", 1);
      fballx_cx2 = get_config_int (sectn, "fballx_cx", 1);
      fballx_cy2 = get_config_int (sectn, "fballx_cy", 1);

      p2.firebx.w = get_config_int (sectn, "firebx_w", 1);
      p2.firebx.nbf = get_config_int (sectn, "firebx_nbf", 1);
      p2.firebx.spd = get_config_int (sectn, "firebx_spd", 1);
      p2.firebx.hit = get_config_int (sectn, "firebx_hit", 1);


      sprintf (p2.firebx.exec, "%s",
	       get_config_string (sectn, "firebx_exec", ""));

      /*
         p2.firebx.exec=
         get_config_string(sectn, "firebx_exec", "");
       */

      p2.firebx.spec = get_config_int (sectn, "firebx_spec", 0);
      p2.firebx.dmg =
	get_config_int (sectn, "firebx_dmg", p2.firebx.spec ? 5 : 3);

      if (!p2.firebx.spec)
	p2.firebx.sflg = 1;

      else
	p2.firebx.sflg = 0;

      cx = 1;
      nb = 0;
      i = 0;
      y = 0;

      while (cx)
	{
	  c = p2.firebx.exec[i];

	  if (c != ' ')
	    {
	      if (c == '*')
		{
		  p2.firebx.end = nb;
		  cx = 0;
		}

	      temp_strg[y] = c;
	      y++;
	    }
	  else
	    {
	      p2.firebx.seq[nb] = get_kstatus (temp_strg);
	      nb++;
	      y = 0;
	    }

	  i++;
	}

    }


  for (i = 0; i < 2; i++)
    temp_strg[i] = 0;

  p2.rushx.flg = get_config_int (sectn, "rushx", 0);

  if (p2.rushx.flg)
    {
      p2.rushx.w = get_config_int (sectn, "rushx_w", 1);
      p2.rushx.nbf = get_config_int (sectn, "rushx_nbf", 1);
      p2.rushx.spd = get_config_int (sectn, "rushx_spd", 1);
      p2.rushx.hit = get_config_int (sectn, "rushx_hit", 1);
      p2.rushx.rot = get_config_int (sectn, "rushx_rot", 1);
      p2.rushx.pix = get_config_int (sectn, "rushx_pix", 1);


      sprintf (p2.rushx.exec, "%s",
	       get_config_string (sectn, "rushx_exec", ""));

      /*
         p2.rushx.exec=
         get_config_string(sectn, "rushx_exec", "");
       */

      p2.rushx.spec = get_config_int (sectn, "rushx_spec", 0);
      p2.rushx.dmg =
	get_config_int (sectn, "rushx_dmg", p2.rushx.spec ? 5 : 3);
      p2.rushx.offx = get_config_int (sectn, "rushx_offx", p2.rushx.w);
      p2.rushx.offy = get_config_int (sectn, "rushx_offy", 10);

      if (!p2.rushx.spec)
	p2.rushx.sflg = 1;

      else
	p2.rushx.sflg = 0;

      cx = 1;
      nb = 0;
      i = 0;
      y = 0;

      while (cx)
	{
	  c = p2.rushx.exec[i];

	  if (c != ' ')
	    {
	      if (c == '*')
		{
		  p2.rushx.end = nb;
		  cx = 0;
		}

	      temp_strg[y] = c;
	      y++;
	    }
	  else
	    {
	      p2.rushx.seq[nb] = get_kstatus (temp_strg);
	      nb++;
	      y = 0;
	    }

	  i++;
	}

    }

  for (i = 0; i < 2; i++)
    temp_strg[i] = 0;

  p2.smovex.flg = get_config_int (sectn, "smovex", 0);

  if (p2.smovex.flg)
    {
      p2.smovex.w = get_config_int (sectn, "smovex_w", 1);
      p2.smovex.nbf = get_config_int (sectn, "smovex_nbf", 1);
      p2.smovex.spd = get_config_int (sectn, "smovex_spd", 1);
      p2.smovex.hit = get_config_int (sectn, "smovex_hit", 1);
      p2.smovex.rot = get_config_int (sectn, "smovex_rot", 1);


      sprintf (p2.smovex.exec, "%s",
	       get_config_string (sectn, "smovex_exec", ""));

      /*
         p2.smovex.exec=
         get_config_string(sectn, "smovex_exec", "");
       */

      p2.smovex.spec = get_config_int (sectn, "smovex_spec", 0);
      p2.smovex.dmg =
	get_config_int (sectn, "smovex_dmg", p2.smovex.spec ? 5 : 3);
      p2.smovex.offx = get_config_int (sectn, "smovex_offx", p2.smovex.w);
      p2.smovex.offy = get_config_int (sectn, "smovex_offy", 10);

      if (!p2.smovex.spec)
	p2.smovex.sflg = 1;

      else
	p2.smovex.sflg = 0;


      cx = 1;
      nb = 0;
      i = 0;
      y = 0;

      while (cx)
	{
	  c = p2.smovex.exec[i];

	  if (c != ' ')
	    {
	      if (c == '*')
		{
		  p2.smovex.end = nb;
		  cx = 0;
		}

	      temp_strg[y] = c;
	      y++;
	    }
	  else
	    {
	      p2.smovex.seq[nb] = get_kstatus (temp_strg);
	      nb++;
	      y = 0;
	    }

	  i++;
	}

    }


  tol2 = p2.width - p2.statik.defx;



	/******* LOAD SPRITES2 ****************************/

  // aqui ele mostra na hora que carrega a luta loading...
  text_centre_shadow (virtscreen, font, "Now Loading... please wait", 160,
		      150, makecol (255, 255, 255));
  rect (virtscreen, 20, 170, 280, 190, sel_player2);
  rectfill (virtscreen, 20, 170, 80, 190, sel_player2);
  draw_sprite (virtscreen, tface1, 0, 0);
  blit_KOF91 ();

  rectfill (virtscreen, 0, 0, virtscreen->w, virtscreen->h,
	    makecol (255, 0, 255));
  /*clear_to_color( virtscreen , makecol(255,0,255) ) ; */

  TRON ("loading sprites 2");

  clone = (sel2 == sel1);
  load_sprites2 ();

  rectfill (virtscreen, 0, 0, virtscreen->w, virtscreen->h,
	    makecol (0, 0, 0));
  TRON ("sprites 2 loaded");

  // aqui ele mostra na hora que carrega a luta loading...
  text_centre_shadow (virtscreen, font, "Now Loading... please wait", 160,
		      150, makecol (255, 255, 255));

  rect (virtscreen, 20, 170, 280, 190, sel_player2);
  rectfill (virtscreen, 20, 170, 150, 190, sel_player2);
  draw_sprite (virtscreen, tface1, 0, 0);
  blit_KOF91 ();


	/******* GET CHARACTER 1 INFO *********************/

  last_sel1 = sel1;
  strcpy (sectn, charname[sel1]);
  sprintf (passeur, DIR_SPRITES "%s" DIR_BAR "char.ini",
	   charname[sel1]);

#if GAME_DEBUG > 0
  sprintf (log_mess, "check char2 path :\n %s", passeur);
  TRON (log_mess);
#endif

  override_config_file (passeur);


  p1.width = get_config_int (sectn, "static_w", 100);
  p1.spunch.w = get_config_int (sectn, "spunch_w", 150);
  p1.wpunch.w = get_config_int (sectn, "wpunch_w", 150);
  p1.skick.w = get_config_int (sectn, "skick_w", 150);
  p1.wkick.w = get_config_int (sectn, "wkick_w", 150);
  p1.apunch.w = get_config_int (sectn, "apunch_w", 150);
  p1.gpunch.w = get_config_int (sectn, "gpunch_w", 150);
  p1.akick.w = get_config_int (sectn, "akick_w", 150);
  p1.gkick.w = get_config_int (sectn, "gkick_w", 150);
  p1.intro.w = get_config_int (sectn, "intro_w", 150);
  p1.ko.w = get_config_int (sectn, "ko_w", 150);
  p1.outwin.w = get_config_int (sectn, "outwin_w", 150);

  p1.height = get_config_int (sectn, "height", 130);

  p1.statik.nbf = get_config_int (sectn, "static_nbf", 4);

  p1.walk.nbf = get_config_int (sectn, "walk_nbf", 5);

  p1.spunch.nbf = get_config_int (sectn, "spunch_nbf", 3);

  p1.wpunch.nbf = get_config_int (sectn, "wpunch_nbf", 3);

  p1.skick.nbf = get_config_int (sectn, "skick_nbf", 4);

  p1.wkick.nbf = get_config_int (sectn, "wkick_nbf", 4);

  p1.apunch.nbf = get_config_int (sectn, "apunch_nbf", 1);

  p1.gpunch.nbf = get_config_int (sectn, "gpunch_nbf", 4);

  p1.akick.nbf = get_config_int (sectn, "akick_nbf", 1);

  p1.gkick.nbf = get_config_int (sectn, "gkick_nbf", 4);

  p1.hurt.nbf = get_config_int (sectn, "hurt_nbf", 1);

  p1.ghurt.nbf = get_config_int (sectn, "ghurt_nbf", 1);

  p1.intro.nbf = get_config_int (sectn, "intro_nbf", 1);

  p1.outwin.nbf = get_config_int (sectn, "outwin_nbf", 1);

  p1.statik.spd = get_config_int (sectn, "static_spd", 8);
  p1.walk.spd = get_config_int (sectn, "walk_spd", 8);
  p1.spunch.spd = get_config_int (sectn, "spunch_spd", 8);
  p1.wpunch.spd = get_config_int (sectn, "wpunch_spd", 8);
  p1.skick.spd = get_config_int (sectn, "skick_spd", 8);
  p1.wkick.spd = get_config_int (sectn, "wkick_spd", 8);
  p1.apunch.spd = get_config_int (sectn, "apunch_spd", 8);
  p1.gpunch.spd = get_config_int (sectn, "gpunch_spd", 8);
  p1.akick.spd = get_config_int (sectn, "akick_spd", 8);
  p1.gkick.spd = get_config_int (sectn, "gkick_spd", 8);
  p1.outwin.spd = get_config_int (sectn, "outwin_spd", 10);

  p1.spunch.hit = get_config_int (sectn, "spunch_hit", 2);
  p1.wpunch.hit = get_config_int (sectn, "wpunch_hit", 2);
  p1.skick.hit = get_config_int (sectn, "skick_hit", 2);
  p1.wkick.hit = get_config_int (sectn, "wkick_hit", 2);
  p1.apunch.hit = get_config_int (sectn, "apunch_hit", 1);
  p1.gpunch.hit = get_config_int (sectn, "gpunch_hit", 2);
  p1.akick.hit = get_config_int (sectn, "akick_hit", 1);
  p1.gkick.hit = get_config_int (sectn, "gkick_hit", 2);

  p1.spunch.dmg = get_config_int (sectn, "spunch_dmg", 2);
  p1.wpunch.dmg = get_config_int (sectn, "wpunch_dmg", 1);
  p1.skick.dmg = get_config_int (sectn, "skick_dmg", 2);
  p1.wkick.dmg = get_config_int (sectn, "wkick_dmg", 1);
  p1.apunch.dmg = get_config_int (sectn, "apunch_dmg", 1);
  p1.gpunch.dmg = get_config_int (sectn, "gpunch_dmg", 1);
  p1.akick.dmg = get_config_int (sectn, "akick_dmg", 2);
  p1.gkick.dmg = get_config_int (sectn, "gkick_dmg", 2);

  p1.spunch.offx = get_config_int (sectn, "spunch_offx", p1.spunch.w);
  p1.wpunch.offx = get_config_int (sectn, "wpunch_offx", p1.wpunch.w);
  p1.skick.offx = get_config_int (sectn, "skick_offx", p1.skick.w);
  p1.wkick.offx = get_config_int (sectn, "wkick_offx", p1.wkick.w);
  p1.apunch.offx = get_config_int (sectn, "apunch_offx", p1.apunch.w);
  p1.gpunch.offx = get_config_int (sectn, "gpunch_offx", p1.gpunch.w);
  p1.akick.offx = get_config_int (sectn, "akick_offx", p1.akick.w);
  p1.gkick.offx = get_config_int (sectn, "gkick_offx", p1.gkick.w);

  p1.spunch.offy = get_config_int (sectn, "spunch_offy", p1.height - 50);
  p1.wpunch.offy = get_config_int (sectn, "wpunch_offy", p1.height - 50);
  p1.skick.offy = get_config_int (sectn, "skick_offy", p1.height - 50);
  p1.wkick.offy = get_config_int (sectn, "wkick_offy", p1.height - 50);
  p1.apunch.offy = get_config_int (sectn, "apunch_offy", p1.height - 50);
  p1.gpunch.offy = get_config_int (sectn, "gpunch_offy", p1.height - 50);
  p1.akick.offy = get_config_int (sectn, "akick_offy", p1.height - 50);
  p1.gkick.offy = get_config_int (sectn, "gkick_offy", p1.height - 50);


  p1.statik.defx = get_config_int (sectn, "static_defx", p1.width);
  p1.walk.defx = get_config_int (sectn, "walk_defx", p1.width);
  p1.jump.defx = get_config_int (sectn, "jump_defx", p1.width);
  p1.crouch.defx = get_config_int (sectn, "crouch_defx", p1.width);
  p1.spunch.defx = get_config_int (sectn, "spunch_defx", p1.width);
  p1.wpunch.defx = get_config_int (sectn, "wpunch_defx", p1.width);
  p1.skick.defx = get_config_int (sectn, "skick_defx", p1.width);
  p1.wkick.defx = get_config_int (sectn, "wkick_defx", p1.width);
  p1.apunch.defx = get_config_int (sectn, "apunch_defx", p1.width);
  p1.gpunch.defx = get_config_int (sectn, "gpunch_defx", p1.width);
  p1.akick.defx = get_config_int (sectn, "akick_defx", p1.width);
  p1.gkick.defx = get_config_int (sectn, "gkick_defx", p1.width);

  p1.statik.defy = get_config_int (sectn, "static_defy", p1.height - 50);
  p1.walk.defy = get_config_int (sectn, "walk_defy", p1.height - 50);
  p1.jump.defy = get_config_int (sectn, "jump_defy", p1.height - 50);
  p1.crouch.defy = get_config_int (sectn, "crouch_defy", p1.height - 50);
  p1.spunch.defy = get_config_int (sectn, "spunch_defy", p1.height - 50);
  p1.wpunch.defy = get_config_int (sectn, "wpunch_defy", p1.height - 50);
  p1.skick.defy = get_config_int (sectn, "skick_defy", p1.height - 50);
  p1.wkick.defy = get_config_int (sectn, "wkick_defy", p1.height - 50);
  p1.apunch.defy = get_config_int (sectn, "apunch_defy", p1.height - 50);
  p1.gpunch.defy = get_config_int (sectn, "gpunch_defy", p1.height - 50);
  p1.akick.defy = get_config_int (sectn, "akick_defy", p1.height - 50);
  p1.gkick.defy = get_config_int (sectn, "gkick_defy", p1.height - 50);


  female1 = get_config_int (sectn, "female", 0);


  for (i = 0; i < 2; i++)
    temp_strg[i] = 0;

  p1.fireb.flg = get_config_int (sectn, "fireb", 0);

  if (p1.fireb.flg)
    {
      fball_h1 = get_config_int (sectn, "fball_h", 1);
      p1.fball.w = get_config_int (sectn, "fball_w", 1);
      p1.fball.nbf = get_config_int (sectn, "fball_nbf", 1);
      p1.fball.spd = get_config_int (sectn, "fball_spd", 1);
      p1.fball.pix = get_config_int (sectn, "fball_pix", 1);
      fball_cx1 = get_config_int (sectn, "fball_cx", 1);
      fball_cy1 = get_config_int (sectn, "fball_cy", 1);
      p1.fball.offx = get_config_int (sectn, "fball_offx", p1.fball.w);
      p1.fball.offy = get_config_int (sectn, "fball_offy", 10);

      p1.fireb.w = get_config_int (sectn, "fireb_w", 1);
      p1.fireb.nbf = get_config_int (sectn, "fireb_nbf", 1);
      p1.fireb.spd = get_config_int (sectn, "fireb_spd", 1);
      p1.fireb.hit = get_config_int (sectn, "fireb_hit", 1);


      sprintf (p1.fireb.exec, "%s",
	       get_config_string (sectn, "fireb_exec", ""));

      /*
         p1.fireb.exec=
         get_config_string(sectn, "fireb_exec", "");
       */

      p1.fireb.dmg = get_config_int (sectn, "fireb_dmg", 3);

      cx = 1;
      nb = 0;
      i = 0;
      y = 0;

      while (cx)
	{
	  c = p1.fireb.exec[i];

	  if (c != ' ')
	    {
	      if (c == '*')
		{
		  p1.fireb.end = nb;
		  cx = 0;
		}

	      temp_strg[y] = c;
	      y++;
	    }
	  else
	    {
	      p1.fireb.seq[nb] = get_kstatus (temp_strg);
	      nb++;
	      y = 0;
	    }

	  i++;
	}

    }



  for (i = 0; i < 2; i++)
    temp_strg[i] = 0;

  p1.rush.flg = get_config_int (sectn, "rush", 0);

  if (p1.rush.flg)
    {
      p1.rush.w = get_config_int (sectn, "rush_w", 1);
      p1.rush.nbf = get_config_int (sectn, "rush_nbf", 1);
      p1.rush.spd = get_config_int (sectn, "rush_spd", 1);
      p1.rush.hit = get_config_int (sectn, "rush_hit", 1);
      p1.rush.rot = get_config_int (sectn, "rush_rot", 1);
      p1.rush.pix = get_config_int (sectn, "rush_pix", 1);


      sprintf (p1.rush.exec, "%s",
	       get_config_string (sectn, "rush_exec", ""));

      /*
         p1.rush.exec=
         get_config_string(sectn, "rush_exec", "");
       */

      p1.rush.dmg = get_config_int (sectn, "rush_dmg", 3);
      p1.rush.offx = get_config_int (sectn, "rush_offx", p1.rush.w);
      p1.rush.offy = get_config_int (sectn, "rush_offy", 10);

      cx = 1;
      nb = 0;
      i = 0;
      y = 0;

      while (cx)
	{
	  c = p1.rush.exec[i];

	  if (c != ' ')
	    {
	      if (c == '*')
		{
		  p1.rush.end = nb;
		  cx = 0;
		}

	      temp_strg[y] = c;
	      y++;
	    }
	  else
	    {
	      p1.rush.seq[nb] = get_kstatus (temp_strg);
	      nb++;
	      y = 0;
	    }

	  i++;
	}

    }



  for (i = 0; i < 2; i++)
    temp_strg[i] = 0;

  p1.smove.flg = get_config_int (sectn, "smove", 0);

  if (p1.smove.flg)
    {
      p1.smove.w = get_config_int (sectn, "smove_w", 1);
      p1.smove.nbf = get_config_int (sectn, "smove_nbf", 1);
      p1.smove.spd = get_config_int (sectn, "smove_spd", 1);
      p1.smove.hit = get_config_int (sectn, "smove_hit", 1);
      p1.smove.rot = get_config_int (sectn, "smove_rot", 1);


      sprintf (p1.smove.exec, "%s",
	       get_config_string (sectn, "smove_exec", ""));

      /*
         p1.smove.exec=
         get_config_string(sectn, "smove_exec", "");

       */
      p1.smove.dmg = get_config_int (sectn, "smove_dmg", 3);
      p1.smove.offx = get_config_int (sectn, "smove_offx", p1.smove.w);
      p1.smove.offy = get_config_int (sectn, "smove_offy", 10);

      cx = 1;
      nb = 0;
      i = 0;
      y = 0;

      while (cx)
	{
	  c = p1.smove.exec[i];

	  if (c != ' ')
	    {
	      if (c == '*')
		{
		  p1.smove.end = nb;
		  cx = 0;
		}

	      temp_strg[y] = c;
	      y++;
	    }
	  else
	    {
	      p1.smove.seq[nb] = get_kstatus (temp_strg);
	      nb++;
	      y = 0;
	    }

	  i++;
	}

    }
  /* SPECIALS */

  for (i = 0; i < 2; i++)
    temp_strg[i] = 0;

  p1.firebx.flg = get_config_int (sectn, "firebx", 0);

  if (p1.firebx.flg)
    {
      fballx_h1 = get_config_int (sectn, "fballx_h", 1);
      p1.fballx.w = get_config_int (sectn, "fballx_w", 1);
      p1.fballx.nbf = get_config_int (sectn, "fballx_nbf", 1);
      p1.fballx.spd = get_config_int (sectn, "fballx_spd", 1);
      p1.fballx.pix = get_config_int (sectn, "fballx_pix", 1);
      fballx_cx1 = get_config_int (sectn, "fballx_cx", 1);
      fballx_cy1 = get_config_int (sectn, "fballx_cy", 1);
      p1.fballx.offx = get_config_int (sectn, "fballx_offx", p1.fballx.w);
      p1.fballx.offy = get_config_int (sectn, "fballx_offy", 10);

      p1.firebx.w = get_config_int (sectn, "firebx_w", 1);
      p1.firebx.nbf = get_config_int (sectn, "firebx_nbf", 1);
      p1.firebx.spd = get_config_int (sectn, "firebx_spd", 1);
      p1.firebx.hit = get_config_int (sectn, "firebx_hit", 1);


      sprintf (p1.firebx.exec, "%s",
	       get_config_string (sectn, "firebx_exec", ""));
      /*
         p1.firebx.exec=
         get_config_string(sectn, "firebx_exec", "");
       */

      p1.firebx.spec = get_config_int (sectn, "firebx_spec", 0);
      p1.firebx.dmg =
	get_config_int (sectn, "firebx_dmg", p1.firebx.spec ? 5 : 3);


      if (!p1.firebx.spec)
	p1.firebx.sflg = 1;

      else
	p1.firebx.sflg = 0;

      cx = 1;
      nb = 0;
      i = 0;
      y = 0;

      while (cx)
	{
	  c = p1.firebx.exec[i];

	  if (c != ' ')
	    {
	      if (c == '*')
		{
		  p1.firebx.end = nb;
		  cx = 0;
		}

	      temp_strg[y] = c;
	      y++;
	    }
	  else
	    {
	      p1.firebx.seq[nb] = get_kstatus (temp_strg);
	      nb++;
	      y = 0;
	    }

	  i++;
	}

    }



  for (i = 0; i < 2; i++)
    temp_strg[i] = 0;

  p1.rushx.flg = get_config_int (sectn, "rushx", 0);

  if (p1.rushx.flg)
    {
      p1.rushx.w = get_config_int (sectn, "rushx_w", 1);
      p1.rushx.nbf = get_config_int (sectn, "rushx_nbf", 1);
      p1.rushx.spd = get_config_int (sectn, "rushx_spd", 1);
      p1.rushx.hit = get_config_int (sectn, "rushx_hit", 1);
      p1.rushx.rot = get_config_int (sectn, "rushx_rot", 1);
      p1.rushx.pix = get_config_int (sectn, "rushx_pix", 1);


      sprintf (p1.rushx.exec, "%s",
	       get_config_string (sectn, "rushx_exec", ""));

      /*
         p1.rushx.exec=
         get_config_string(sectn, "rushx_exec", "");
       */

      p1.rushx.spec = get_config_int (sectn, "rushx_spec", 0);
      p1.rushx.dmg =
	get_config_int (sectn, "rushx_dmg", p1.rushx.spec ? 5 : 3);
      p1.rushx.offx = get_config_int (sectn, "rushx_offx", p1.rushx.w);
      p1.rushx.offy = get_config_int (sectn, "rushx_offy", 10);

      if (!p1.rushx.spec)
	p1.rushx.sflg = 1;

      else
	p1.rushx.sflg = 0;

      cx = 1;
      nb = 0;
      i = 0;
      y = 0;

      while (cx)
	{
	  c = p1.rushx.exec[i];

	  if (c != ' ')
	    {
	      if (c == '*')
		{
		  p1.rushx.end = nb;
		  cx = 0;
		}

	      temp_strg[y] = c;
	      y++;
	    }
	  else
	    {
	      p1.rushx.seq[nb] = get_kstatus (temp_strg);
	      nb++;
	      y = 0;
	    }

	  i++;
	}

    }



  for (i = 0; i < 2; i++)
    temp_strg[i] = 0;

  p1.smovex.flg = get_config_int (sectn, "smovex", 0);

  if (p1.smovex.flg)
    {
      p1.smovex.w = get_config_int (sectn, "smovex_w", 1);
      p1.smovex.nbf = get_config_int (sectn, "smovex_nbf", 1);
      p1.smovex.spd = get_config_int (sectn, "smovex_spd", 1);
      p1.smovex.hit = get_config_int (sectn, "smovex_hit", 1);
      p1.smovex.rot = get_config_int (sectn, "smovex_rot", 1);


      sprintf (p1.smovex.exec, "%s",
	       get_config_string (sectn, "smovex_exec", ""));

      /*
         p1.smovex.exec=
         get_config_string(sectn, "smovex_exec", "");
       */

      p1.smovex.spec = get_config_int (sectn, "smovex_spec", 0);
      p1.smovex.dmg =
	get_config_int (sectn, "smovex_dmg", p1.smovex.spec ? 5 : 3);
      p1.smovex.offx = get_config_int (sectn, "smovex_offx", p1.smovex.w);
      p1.smovex.offy = get_config_int (sectn, "smovex_offy", 10);

      if (!p1.smovex.spec)
	p1.smovex.sflg = 1;

      else
	p1.smovex.sflg = 0;

      cx = 1;
      nb = 0;
      i = 0;
      y = 0;

      while (cx)
	{
	  c = p1.smovex.exec[i];

	  if (c != ' ')
	    {
	      if (c == '*')
		{
		  p1.smovex.end = nb;
		  cx = 0;
		}

	      temp_strg[y] = c;
	      y++;
	    }
	  else
	    {
	      p1.smovex.seq[nb] = get_kstatus (temp_strg);
	      nb++;
	      y = 0;
	    }

	  i++;
	}

    }


  tol1 = (p1.statik.defx);



	/*********** LOAD SPRITES1 *********************/

  text_centre_shadow (virtscreen, font, "Now Loading... please wait", 160,
		      150, makecol (255, 255, 255));
  rect (virtscreen, 20, 170, 280, 190, sel_player2);
  rectfill (virtscreen, 20, 170, 190, 190, sel_player2);
  draw_sprite (virtscreen, tface1, 0, 0);
  blit_KOF91 ();

  load_sprites1 ();

  text_centre_shadow (virtscreen, font, "Now Loading... please wait", 160,
		      150, makecol (255, 255, 255));
  rect (virtscreen, 20, 170, 280, 190, sel_player2);
  rectfill (virtscreen, 20, 170, 250, 190, sel_player2);
  draw_sprite (virtscreen, tface1, 0, 0);
  blit_KOF91 ();

  override_config_file ("bkgds" DIR_BAR "bkgd.ini");


	/*********** ALL SPRITES ARE NOW IN MEMORY ********/


  /* LOADING THE BACKGROUND NOW */

  foreground = animated = 0;

  sprintf (file, "bkgds" DIR_BAR "%s", bkgdname[(int)bgd]);

#if GAME_DEBUG > 0
  sprintf (log_mess, "check bkgd path (again) :\n %s", file);
  TRON (log_mess);
#endif

  if ((Bkgd = xload_pcx (file, 0)) == NULL)
    {
      if ((open_fli (file) != FLI_OK))
	{
	  ERROR ("failed loading Bkgd");
	  allegro_exit ();
	  exit (0);
	}
      else
	{
	  reset_fli_variables ();
	  next_fli_frame (1);
	  set_palette (fli_palette);
	  Bkgd = fli_bitmap;
	  animated = 1;
	  foreground = 0;
	}
    }

  else
    {
      animated = 0;

      sprintf (file, "bkgds" DIR_BAR "frgds" DIR_BAR "%s", bkgdname[(int)bgd]);

      if ((Frgd = xload_pcx (file, 0)) == NULL)
	foreground = 0;
      else
	foreground = 1;

      /*    foreground = !( ( Frgd = xload_pcx( file, 0 ) ) == NULL ); */

    }



  bkgdlim = Bkgd->w - 320;

  zoomsup = Bkgd->h - 200;

  bkgd_delay = 15;

  zoomscreen = create_bitmap (Bkgd->w, Bkgd->h);


  /* BACKGROUND LOADED */

  y = ustrlen (bkgdname[(int)bgd]);

  strcpy (file, DIR_MIDI);
  ustrncat (file, bkgdname[(int)bgd], y - 4);
  strcat (file, ".mid");

  if ((music = xload_midi (file)) == NULL)
    {
      ERROR ("error in midi file format");
      //allegro_exit ();
      //exit (0);
    }


  /* MIDI FILE LOADED */

  override_config_file ("game.ini");

  LBbanner = xload_pcx (DIR_SPRITES "lifebars.pcx", 0);
  LBbanner2 = xload_pcx (DIR_SPRITES "lifebars2.pcx", 0);

  Bar = xload_pcx (DIR_SPRITES "bar.pcx", 0);
  draw_sprite_h_flip (Bar2, Bar, 0, 0);

  // Aqui irei carregar as caras que ficam no lado da barra de life

  sprintf (file2, DIR_SPRITES "%s" DIR_BAR "selsmall.pcx",
	   charname[sel1]);


  tface1 = xload_pcx (file2, Pal);

  tface_border1 = xload_pcx(DIR_SPRITES "bsmall1.pcx", Pal);

  sprintf (file2, DIR_SPRITES "%s" DIR_BAR "selsmall.pcx",
	   charname[sel2]);
  clone = (sel1 == sel2);

  tface2 = load_sprite(file2, Pal);
  tface_border2 = xload_pcx(DIR_SPRITES "bsmall2.pcx", Pal);

  //if ( tface1 != NULL )
  //      draw_sprite( virtscreen, tface1, 0, 25 );


  // encerra comandos para mostrar as caras :)

  novic0 = xload_pcx (DIR_SPRITES "novic0.pcx", 0);
  novic1 = xload_pcx (DIR_SPRITES "novic1.pcx", 0);

  avic0 = xload_pcx (DIR_SPRITES "vic0.pcx", 0);
  avic1 = xload_pcx (DIR_SPRITES "vic1.pcx", 0);

  /*    Blood = xload_pcx(DIR_SPRITES "lastpal.pcx", Pal);
   */


  Carton_R1 = xload_pcx (DIR_SPRITES "round1.pcx", 0);

  Carton_R2 = xload_pcx (DIR_SPRITES "round2.pcx", 0);

  Carton_R3 = xload_pcx (DIR_SPRITES "round3.pcx", 0);

  Carton_FT = xload_pcx (DIR_SPRITES "fight.pcx", 0);

  Carton_WN = xload_pcx (DIR_SPRITES "winner.pcx", 0);

  Carton_KO = xload_pcx (DIR_SPRITES "ko.pcx", 0);

  Power = xload_pcx (DIR_SPRITES "power.pcx", 0);

  PBar = xload_pcx (DIR_SPRITES "pbar.pcx", 0);

// claudemir - era 0 e ficava ligado, agora � 1 desligado, eu acho :)   
#if TIMEOVER > 0
  TRON ("Loading timeborder.pcx...");
  if (file_missing(DIR_SPRITES "timeborder.pcx") ){
     B_clock[10] = create_bitmap( 29,21 );
     rectfill( B_clock[10],0,0,29,21, makecol( 0, 0, 0 ) );
  }else{
     B_clock[10] = xload_pcx (DIR_SPRITES "timeborder.pcx", 0);
  }

  TRON ("Loading numbers 0.pcx - 9.pcx");
  B_clock[0] = xload_pcx (DIR_SPRITES "0.pcx", 0);
  B_clock[1] = xload_pcx (DIR_SPRITES "1.pcx", 0);
  B_clock[2] = xload_pcx (DIR_SPRITES "2.pcx", 0);
  B_clock[3] = xload_pcx (DIR_SPRITES "3.pcx", 0);
  B_clock[4] = xload_pcx (DIR_SPRITES "4.pcx", 0);
  B_clock[5] = xload_pcx (DIR_SPRITES "5.pcx", 0);
  B_clock[6] = xload_pcx (DIR_SPRITES "6.pcx", 0);
  B_clock[7] = xload_pcx (DIR_SPRITES "7.pcx", 0);
  B_clock[8] = xload_pcx (DIR_SPRITES "8.pcx", 0);
  B_clock[9] = xload_pcx (DIR_SPRITES "9.pcx", 0);
  TRON ("ok");
#endif

#if PERFECT == 1

  B_prfct = xload_pcx (DIR_SPRITES "perfect.pcx", 0);
#endif

#if SHADOW == 1

  B_shadow = xload_pcx (DIR_SPRITES "shadow.pcx", 0);
#endif

  text_centre_shadow (virtscreen, font, "Now Loading... please wait", 160,
		      150, makecol (255, 255, 255));
  rect (virtscreen, 20, 170, 280, 190, sel_player2);
  rectfill (virtscreen, 20, 170, 280, 190, sel_player2);

  draw_sprite (virtscreen, tface1, 0, 0);
  blit_KOF91 ();

  /* some more character info that depends from zoomsup */

  sol2 = (zoomsup + 185) - p2.height;

  jump_how2 = (zoomsup + 125) - p2.height;

  sol1 = (zoomsup + 185) - p1.height;

  jump_how1 = (zoomsup + 125) - p1.height;

	/******/

  cx = (int) (bkgdlim / 2);

  cy = zoomsup;

  zoomwidth = 320;

  spr1x = ((cx + 75) - (p1.width / 2)), spr1y = sol1;
  spr2x = ((cx + 250) - (p2.width / 2)), spr2y = sol2;

  fx = fy = 1;

  flag = flag2 = 1;

  fball1 = fball2 = 0;
  fballx1 = fballx2 = 0;

  spower1 = spower2 = 0;

  p1.fireb.succ = p2.fireb.succ = 0;
  p1.rush.succ = p2.rush.succ = 0;
  rush_stop1 = rush_stop2 = 0;
  smove_stop1 = smove_stop2 = 0;

  p1.firebx.succ = p2.firebx.succ = 0;
  p1.rushx.succ = p2.rushx.succ = 0;
  rushx_stop1 = rushx_stop2 = 0;
  smovex_stop1 = smovex_stop2 = 0;

  p1.wimpact_ct = p2.wimpact_ct = 0;
  p1.simpact_ct = p2.simpact_ct = 0;
  p1.bimpact_ct = p2.bimpact_ct = 0;

  damag1 = damag2 = 1;

  autho_ht1 = autho_ht2 = 0;


  life1 = life2 = 501;
#if TIMEOVER > 0

  PUT_t_clock (xget_config_int ("timer", "t_clock", 0));
#endif

  inverted = 0;
  deb = 1;

  fight_round = 1;
  vic1 = vic2 = 0;

  vsynch = 1;

  ok = 1;

  vic_flag = 1;

  /*   set_palette (black_palette) ; */
  set_palette (fli_palette);

  clear (screen);

  /* Display the background (not necessary) */

  if (animated)
    {
      reset_fli_variables ();

      next_fli_frame (1);

      blit (fli_bitmap, virtscreen, cx, zoomsup, 0, 0, 320, 200);
    }

  else

    blit (Bkgd, virtscreen, cx, zoomsup, 0, 0, 320, 200);


  blit_KOF91 ();


  play_midi (music, 1);

  set_palette (fli_palette);

	/****************************************/
  return 0;
}
/***********************************************/

// EOF
