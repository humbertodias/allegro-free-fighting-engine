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
#include "config.h"
#include "macros.h"
#include "global.h"
#include "loads.h"
#include "initfiles.h"
#include "wavsounds.h"
#include "effects.h"
#include "display.h"
#include "intro.h"
#include "refresh.h"
#include "moves.h"
#include "monitor.h"
#include "batle.h"


/*******************************************/
int K_batle (void){

   /********* THE REAL GAME STARTS HERE ***********************/
  /* Loop until we're asked to exit */
  kof_fade_in (screen, 9999);
  TRON ("OK1");
  initdisplay ();
  TRON ("OK2");
  intro ();
  TRON ("OK3");
  introround (fight_round);
  TRON ("OK4");
  refresh_vflags ();
  TRON ("OK5");
  while (ok)
    {
      //****************************************************************
      //* Round beginning Loop 
      //****************************************************************
      if (frameskip < skp_flag)
	frameskip++;
      else
	frameskip = 0;

      if (animated)
	bkgd_counter++;

      if (retrace_count > 4)
	retrace_count = 0;
      retrace_count--;

      /* the big drawing loop */
      //if (retrace_count > 0) ;else
      if (!(retrace_count > 0) && (!frameskip))
	{
	  /*
	   * Increment the frame count 
	   */
	  if (!nbpar)
	    n++;

	  vic_flag++;

	  if (vic_flag > 40)
	    vic_flag = 1;

	  blit_KOF91 ();
	  blit_Bkgd ();

#if GAME_DEBUG > 0
	  show_fpc ();
#endif

	  //refresh_lifebar();
	  //refresh_powerbar();
	  // Claudemir
	  //refresh_life_faces();
#if TIMEOVER > 0
	  refresh_timer ();
#endif
	  refresh_vflags ();
	  displays ();

	  /*
	   * Invert Sprite priority when hurt 
	   */

#if GAME_DEBUG >= 2
	  TRON ("Invert Sprite priority when hurt ");
	  sprintf (log_mess, "actual2: %d", actual2);
	  TRON (log_mess);
#endif

	  switch (actual2)
	    {
	    case 8:
	      GSingle2 (0, spr2x, spr2y);
	      break;

	    case 18:
	      GSingle2 (2, spr2x, spr2y);
	      break;

	    case 9:
	      GHurt2 ((hflag2 / (25 / p2.hurt.nbf)), spr2x, spr2y);
	      break;

	    case 19:
	      GGHurt2 ((hflag2 / (25 / p2.ghurt.nbf)), spr2x, spr2y);
	      break;

	    case 20:
	      GKO2 ((hflag2 / 7), (spr2x - (p2.ko.w - p2.width)),
		    spr2y, (p2.ko.w - p2.width));
	      break;
	    }

#if GAME_DEBUG >= 2
	  TRON ("Draw the sprites");
	  /* Draw the sprites */
	  sprintf (log_mess, "ak: %d", ak);
	  TRON (log_mess);
#endif

	  if (ak)
	    {
	      GAKick ((ak / p1.akick.spd), spr1x, spr1y);
	      p1.limdefx = p1.akick.defx;
	      p1.limdefy = p1.akick.defy;
	    }
	  else if (apch)
	    {
	      GAPunch ((apch / p1.apunch.spd), spr1x, spr1y);
	      p1.limdefx = p1.apunch.defx;
	      p1.limdefy = p1.apunch.defy;
	    }
	  else
	    switch (actual)
	      {
	      case 0:
		GStatic ((flag / p1.statik.spd), spr1x, spr1y);
		p1.limdefx = p1.statik.defx;
		p1.limdefy = p1.statik.defy;
		break;
	      case 111:
	      case 1:
		GWalk ((flag / p1.walk.spd), spr1x, spr1y);
		p1.limdefx = p1.walk.defx;
		p1.limdefy = p1.walk.defy;

		break;
	      case 2:
		GSingle (1, spr1x, spr1y);
		p1.limdefx = p1.crouch.defx;
		p1.limdefy = p1.crouch.defy;
		break;
	      case 3:
	      case 4:
	      case 5:
		if (spr1y < (sol1 - 50))
		  {
		    GJump (3, spr1x, spr1y);
		  }
		else
		  {
		    if (sens1)
		      {
			if (spr1y < (sol1 - 20))
			  {
			    GJump (4, spr1x, spr1y);
			  }
			else
			  {
			    GJump (5, spr1x, spr1y);
			  }
		      }
		    else
		      {
			if (spr1y < (sol1 - 20))
			  {
			    GJump (2, spr1x, spr1y);
			  }
			else
			  {
			    GJump (1, spr1x, spr1y);
			  }
		      }
		  }
		p1.limdefx = p1.jump.defx;
		p1.limdefy = p1.jump.defy;
		break;
	      case 30:
		GFireB ((flag / p1.fireb.spd), spr1x, spr1y);
		break;
	      case 40:
		GRush ((flag / p1.rush.spd), spr1x, spr1y);
		break;
	      case 50:
		GSMove ((flag / p1.smove.spd), spr1x, spr1y);
		break;
	      case 31:
		GFireBX ((flag / p1.firebx.spd), spr1x, spr1y);
		break;
	      case 41:
		GRushX ((flag / p1.rushx.spd), spr1x, spr1y);
		break;
	      case 51:
		GSMoveX ((flag / p1.smovex.spd), spr1x, spr1y);
		break;
	      case 6:
		GPunch ((flag / p1.spunch.spd), spr1x, spr1y);
		p1.limdefx = p1.spunch.defx;
		p1.limdefy = p1.spunch.defy;
		break;
	      case 61:
		GWPunch ((flag / p1.wpunch.spd), spr1x, spr1y);
		p1.limdefx = p1.wpunch.defx;
		p1.limdefy = p1.wpunch.defy;
		break;
	      case 7:
		GKick ((flag / p1.skick.spd), spr1x, spr1y, 1);
		p1.limdefx = p1.skick.defx;
		p1.limdefy = p1.skick.defy;
		break;
	      case 71:
		GWKick ((flag / p1.wkick.spd), spr1x, spr1y);
		p1.limdefx = p1.wkick.defx;
		p1.limdefy = p1.wkick.defy;
		break;
	      case 10:
		GGKick ((flag / p1.gkick.spd), spr1x, spr1y);
		p1.limdefx = p1.gkick.defx;
		p1.limdefy = p1.gkick.defy;
		break;
	      case 13:
		GGPunch ((flag / p1.gpunch.spd), spr1x, spr1y);
		p1.limdefx = p1.gpunch.defx;
		p1.limdefy = p1.gpunch.defy;
		break;
	      case 8:
		GSingle (0, spr1x, spr1y);
		break;
	      case 18:
		GSingle (2, spr1x, spr1y);
		break;
	      case 9:
		GHurt ((hflag / (25 / p1.hurt.nbf)), spr1x, spr1y);
		break;
	      case 19:
		GGHurt ((hflag / (25 / p1.ghurt.nbf)), spr1x, spr1y);
		break;
	      case 20:
		GKO ((hflag / 7), spr1x, spr1y);
		break;
	      }

#if GAME_DEBUG >= 2
	  sprintf (log_mess, "ak2: %d", ak2);
	  TRON (log_mess);
#endif
	  if (ak2)
	    {
	      GAKick2 ((ak2 / p2.akick.spd),
		       (spr2x - (p2.akick.w - p2.width)), spr2y);
	      p2.limdefx = p2.akick.defx;
	      p2.limdefy = p2.akick.defy;
	    }
	  else if (apch2)
	    {
	      GAPunch2 ((apch2 / p2.apunch.spd),
			(spr2x - (p2.apunch.w - p2.width)), spr2y);
	      p2.limdefx = p2.apunch.defx;
	      p2.limdefy = p2.apunch.defy;
	    }
	  else
	    switch (actual2)
	      {
	      case 0:
		GStatic2 ((flag2 / p2.statik.spd), spr2x, spr2y);
		p2.limdefx = p2.statik.defx;
		p2.limdefy = p2.statik.defy;
		break;
	      case 111:
	      case 1:
		GWalk2 ((flag2 / p2.walk.spd), spr2x, spr2y);
		p2.limdefx = p2.walk.defx;
		p2.limdefy = p2.walk.defy;
		break;
	      case 2:
		GSingle2 (1, spr2x, spr2y);
		p2.limdefx = p2.crouch.defx;
		p2.limdefy = p2.crouch.defy;
		break;
	      case 3:
	      case 4:
	      case 5:
		if (spr2y < (sol2 - 50))
		  {
		    GJump2 (3, spr2x, spr2y);
		  }
		else
		  {
		    if (sens2)
		      {
			if (spr2y < (sol2 - 20))
			  {
			    GJump2 (4, spr2x, spr2y);
			  }
			else
			  {
			    GJump2 (5, spr2x, spr2y);
			  }
		      }
		    else
		      {
			if (spr2y < (sol2 - 20))
			  GJump2 (2, spr2x, spr2y);
			else
			  GJump2 (1, spr2x, spr2y);
		      }
		  }
		p2.limdefx = p2.crouch.defx;
		p2.limdefy = p2.crouch.defy;
		break;
	      case 30:
		GFireB2 ((flag2 / p2.fireb.spd),
			 spr2x - (p2.fireb.w - p2.width), spr2y);
		break;
	      case 40:
		GRush2 ((flag2 / p2.rush.spd), spr2x - (p2.rush.w - p2.width),
			spr2y);
		break;
	      case 50:
		GSMove2 ((flag2 / p2.smove.spd),
			 spr2x - (p2.smove.w - p2.width), spr2y);
		break;
	      case 31:
		GFireBX2 ((flag2 / p2.firebx.spd),
			  spr2x - (p2.firebx.w - p2.width), spr2y);
		break;
	      case 41:
		GRushX2 ((flag2 / p2.rushx.spd),
			 spr2x - (p2.rushx.w - p2.width), spr2y);
		break;
	      case 51:
		GSMoveX2 ((flag2 / p2.smovex.spd),
			  spr2x - (p2.smovex.w - p2.width), spr2y);
		break;
	      case 6:
		GPunch2 ((flag2 / p2.spunch.spd),
			 spr2x - (p2.spunch.w - p2.width), spr2y);
		p2.limdefx = p2.spunch.defx;
		p2.limdefy = p2.spunch.defy;
		break;
	      case 61:
		GWPunch2 ((flag2 / p2.wpunch.spd),
			  spr2x - (p2.wpunch.w - p2.width), spr2y);
		p2.limdefx = p2.wpunch.defx;
		p2.limdefy = p2.wpunch.defy;
		break;
	      case 7:
		GKick2 ((flag2 / p2.skick.spd),
			spr2x - (p2.skick.w - p2.width), spr2y, 1);
		p2.limdefx = p2.skick.defx;
		p2.limdefy = p2.skick.defy;
		break;
	      case 71:
		GWKick2 ((flag2 / p2.wkick.spd),
			 spr2x - (p2.wkick.w - p2.width), spr2y);
		p2.limdefx = p2.wkick.defx;
		p2.limdefy = p2.wkick.defy;
		break;
	      case 10:
		GGKick2 ((flag2 / p2.gkick.spd),
			 spr2x - (p2.gkick.w - p2.width), spr2y);
		p2.limdefx = p2.gkick.defx;
		p2.limdefy = p2.gkick.defy;
		break;
	      case 13:
		GGPunch2 ((flag2 / p2.gpunch.spd),
			  spr2x - (p2.gpunch.w - p2.width), spr2y);
		p2.limdefx = p2.gpunch.defx;
		p2.limdefy = p2.gpunch.defy;
		break;
	      }
	  if (fball1)
	    GFBall ((fball_flg1 / p1.fball.spd), fb1x, fb1y);
	  if (fball2)
	    GFBall2 ((fball_flg2 / p2.fball.spd), fb2x, fb2y);
	  if (fballx1)
	    GFBallX ((fballx_flg1 / p1.fballx.spd), fbx1x, fbx1y);
	  if (fballx2)
	    GFBallX2 ((fballx_flg2 / p2.fballx.spd), fbx2x, fbx2y);
	  /* IMPACTS ************************** */

#if GAME_DEBUG >= 2
	  TRON ("IMPACTS");
#endif
	  if (p1.wimpact_ct)
	    {
	      p1.wimpact_ct--;
	      misc = (11 - p1.wimpact_ct) / (10 / wimpact_nbf);
	      if (misc > wimpact_nbf)
		{
		  misc = wimpact_nbf;
		  p1.wimpact_ct = 0;
		}
	      if (misc == 0)
		misc = 1;
	      set_trans_blender (255, 255, 255, 170);
	      draw_trans_sprite (zoomscreen, WImpact[misc],
				 p1.limoffx - (WImpact[misc]->w / 2),
				 p1.limoffy - (WImpact[misc]->h / 2));
	    }
	  if (p2.wimpact_ct)
	    {
	      p2.wimpact_ct--;
	      misc = (11 - p2.wimpact_ct) / (10 / wimpact_nbf);
	      if (misc > wimpact_nbf)
		{
		  misc = wimpact_nbf;
		  p2.wimpact_ct = 0;
		}
	      if (misc == 0)
		misc = 1;
	      B_Temp = create_bitmap (WImpact[misc]->w, WImpact[misc]->h);
	      rectfill (B_Temp, 0, 0, WImpact[misc]->w, WImpact[misc]->h,
			makecol (255, 0, 255));
	      draw_sprite_h_flip (B_Temp, WImpact[misc], 0, 0);
	      set_trans_blender (255, 255, 255, 170);
	      draw_trans_sprite (zoomscreen, B_Temp,
				 p2.limoffx - (WImpact[misc]->w / 2),
				 p2.limoffy - (WImpact[misc]->h / 2));
	    }
	  if (p1.simpact_ct)
	    {
	      p1.simpact_ct--;
	      misc = (11 - p1.simpact_ct) / (10 / simpact_nbf);
	      if (misc > simpact_nbf)
		{
		  misc = simpact_nbf;
		  p1.simpact_ct = 0;
		}
	      if (misc == 0)
		misc = 1;
	      set_trans_blender (255, 255, 255, 170);
	      draw_trans_sprite (zoomscreen, SImpact[misc],
				 p1.limoffx - (SImpact[misc]->w / 2),
				 p1.limoffy - (SImpact[misc]->h / 2));
	    }
	  if (p2.simpact_ct)
	    {
	      p2.simpact_ct--;
	      misc = (11 - p2.simpact_ct) / (10 / simpact_nbf);
	      if (misc > simpact_nbf)
		{
		  misc = simpact_nbf;
		  p2.simpact_ct = 0;
		}
	      if (misc == 0)
		misc = 1;
	      B_Temp =
		create_system_bitmap (SImpact[misc]->w, SImpact[misc]->h);
	      rectfill (B_Temp, 0, 0, SImpact[misc]->w, SImpact[misc]->h,
			makecol (255, 0, 255));
	      draw_sprite_h_flip (B_Temp, SImpact[misc], 0, 0);
	      set_trans_blender (255, 255, 255, 170);
	      draw_trans_sprite (zoomscreen, B_Temp,
				 p2.limoffx - (SImpact[misc]->w / 2),
				 p2.limoffy - (SImpact[misc]->h / 2));
	    }
	  if (p1.bimpact_ct)
	    {
	      p1.bimpact_ct--;
	      misc = (11 - p1.bimpact_ct) / (10 / bimpact_nbf);
	      if (misc > bimpact_nbf)
		{
		  misc = bimpact_nbf;
		  p1.bimpact_ct = 0;
		}
	      if (misc == 0)
		misc = 1;
	      set_trans_blender (255, 255, 255, 170);
	      draw_trans_sprite (zoomscreen, BlockImpact[misc],
				 p1.limoffx - (BlockImpact[misc]->w / 2),
				 p1.limoffy - (BlockImpact[misc]->h / 2));
	    }
	  if (p2.bimpact_ct)
	    {
	      p2.bimpact_ct--;
	      misc = (11 - p2.bimpact_ct) / (10 / bimpact_nbf);
	      if (misc > bimpact_nbf)
		{
		  misc = bimpact_nbf;
		  p2.bimpact_ct = 0;
		}
	      if (misc == 0)
		misc = 1;
	      B_Temp =
		create_system_bitmap (BlockImpact[misc]->w,
				      BlockImpact[misc]->h);
	      rectfill (B_Temp, 0, 0, BlockImpact[misc]->w,
			BlockImpact[misc]->h, makecol (255, 0, 255));
	      draw_sprite_h_flip (B_Temp, BlockImpact[misc], 0, 0);
	      set_trans_blender (255, 255, 255, 170);
	      draw_trans_sprite (zoomscreen, B_Temp,
				 p2.limoffx - (BlockImpact[misc]->w / 2),
				 p2.limoffy - (BlockImpact[misc]->h / 2));
	    }
	  if (foreground)
	    masked_blit (Frgd, zoomscreen, 0, 0, 0, 0, (bkgdlim + 320),
			 (200 + zoomsup));
	  cy = (200 + zoomsup) - ZMRATIO;
	  if (cy < 1)
	    cy = 1;
	  if ((cy < zoomsup) && (cy > 0))
	    stretch_blit (zoomscreen, virtscreen, cx, cy, zoomwidth,
			  ZMRATIO, 0, 0, 320, 200);
	  else
	    {
	      blit (zoomscreen, virtscreen, cx, zoomsup, 0, 0, 320, 200);
	    }

	  /*
	     if (inverted)
	     blit(zoomscreen, virtscreen, zoomscreen->w - spr1x+1, zoomsup, 0, 0, 320, 200);
	     else 
	     blit(zoomscreen, virtscreen, spr1x, zoomsup, 0, 0, 320, 200);
	   */

	  /* If a key is hit, check it out  */

#if GAME_DEBUG >= 2
	  TRON ("TEST KEYS");
#endif
	  if (keypressed){
	    K_pause_game();
          }
	  /*
	     while (!retrace_count);;
	   */

	  /*  end of the big drawing loop */
	  adjusts (F_SLOWDOWN);

	}

     /********** PRIORITARY ANIMATIONS UPDATING ************/
     /***** PLAYER1 **********/

#if GAME_DEBUG >= 2
      TRON ("PRIORITARY ANIMATIONS UPDATING\nPLAYER1");
#endif
      if (actual == 3)		// SIMPLE JUMP
	{
	  if (spr1y < jump_how1)
	    sens1 = 1;
	  if (sens1)
	    {
	      if (spr1y < (jump_how1 + 4))
		spr1y++;
	      else if (spr1y < (jump_how1 + 17))
		spr1y += 2;
	      else
		spr1y += 3;

	      if (spr1y > sol1)
		{
		  actual = 0;
		  spr1y = sol1;
		  sens1 = 0;
		  limak = 1;
		}
	    }
	  else
	    {
	      if (spr1y < (jump_how1 + 4))
		spr1y--;
	      else if (spr1y < (jump_how1 + 17))
		spr1y -= 2;
	      else
		spr1y -= 3;
	    }
	}
      else if (actual == 4)	// JUMP BACKWARD
	{
	  if (spr1y < jump_how1)
	    sens1 = 1;
	  if (sens1)
	    {
	      if (spr1y < (jump_how1 + 4))
		spr1y++;
	      else if (spr1y < (jump_how1 + 30))
		spr1y += 2;
	      else
		spr1y += 3;

	      if (spr1x > (limg + cx))
		spr1x -= 2;
	      else if (spr2x < (limd + cx))
		for (i = 0; i < s; i++)
		  {
		    if (cx > 1)
		      {
			cx--;
			spr1x--;
		      }
		  }
	      else
		for (i = 0; i < s; i++)
		  {
		    if (cx > 1)
		      if (zoomwidth < ((200 + zoomsup) * 1.6))
			{
			  zoomwidth++;
			  cx--;
			  spr1x--;
			}
		  }

	      if (spr1y > sol1)
		{
		  actual = 0;
		  spr1y = sol1;
		  sens1 = 0;
		  limak = 1;
		}
	    }
	  else
	    {
	      if (spr1y < (jump_how1 + 4))
		spr1y--;
	      else if (spr1y < (jump_how1 + 30))
		spr1y -= 2;
	      else
		spr1y -= 3;

	      if (spr1x > (limg + cx))
		spr1x -= 2;
	      else if (spr2x < (limd + cx))
		for (i = 0; i < s; i++)
		  {
		    if (cx > 1)
		      {
			cx--;
			spr1x--;
		      }
		  }
	      else
		for (i = 0; i < s; i++)
		  {
		    if (cx > 1)
		      if (zoomwidth < ((200 + zoomsup) * 1.6))
			{
			  zoomwidth++;
			  cx--;
			  spr1x--;
			}
		  }
	    }
	}
      else if (actual == 5)	// JUMP FORWARD
	{
	  if (spr1y < jump_how1)
	    sens1 = 1;
	  if (sens1)
	    {
	      if (spr1y < (jump_how1 + 4))
		spr1y++;
	      else if (spr1y < (jump_how1 + 30))
		spr1y += 2;
	      else
		spr1y += 3;
	      if (zoomwidth > 320)
		{
		  zoomwidth--;
		  cx++;
		}
#if TEMP == 0

	      if ((spr1x + tol1) < (spr2x + tol2))
#endif

		spr1x += 2;

#if TEMP == 0

	      else
#endif
	      if (spr2x < (limd + cx))
		{
		  spr1x++;
		  spr2x++;
		}
	      else if (cx < (bkgdlim + 320 - zoomwidth))
		{
		  cx++;
		  spr1x++;
		  spr2x++;
		}
	      if (spr1y > sol1)
		{
		  actual = 0;
		  spr1y = sol1;
		  sens1 = 0;
		  limak = 1;

#if TEMP > 0

		  if (spr1x > spr2x)
		    {
		      inverted = !inverted;
#if GAME_DEBUG >= 2
		      sprintf (log_mess, "\n Invertendo...%d\n", inverted);
		      TRON (log_mess);
#endif

		      spr1x = zoomscreen->w - spr1x - Player1.Jump[1]->w + 1;
		      spr2x = zoomscreen->w - spr2x - Player2.Jump[1]->w + 1;
		    }
#endif

		}
	    }
	  else
	    {
	      if (spr1y < (jump_how1 + 4))
		spr1y--;
	      else if (spr1y < (jump_how1 + 30))
		spr1y -= 2;
	      else
		spr1y -= 3;
	      if (zoomwidth > 320)
		{
		  zoomwidth--;
		  cx++;
		}
#if TEMP == 0
	      if ((spr1x + tol1) < (spr2x + tol2))
#endif

		spr1x += 2;
#if TEMP == 0

	      else
#endif
	      if (spr2x < (limd + cx))
		{
		  spr1x++;
		  spr2x++;
		}
	      else if (cx < (bkgdlim + 320 - zoomwidth))
		{
		  cx++;
		  spr1x++;
		  spr2x++;
		}
	    }
	}

		/***************************************************************/
      else if ((actual == 40) && (limp1 > 0))	// RUSH
	{
	  flag++;
	  if (flag > ((p1.rush.hit - 1) * p1.rush.spd))
	    {
	      if ((spr1x + tol1) < (spr2x + tol2))
		spr1x += p1.rush.pix;
	      if (zoomwidth > 320)
		{
		  zoomwidth--;
		  cx++;
		}
	      if (flag >
		  p1.rush.spd * (p1.rush.hit +
				 (p1.rush.nbf - p1.rush.hit) / 2))
		if ((spr1x + p1.rush.offx) > (spr2x + p2.width - p2.limdefx))
		  {
		    damag2 = p1.rush.dmg;
		    p1.limoffx = spr1x + p1.rush.offx;
		    p1.limoffy = spr1y + p1.rush.offy;
		    if (spr2y < (sol2 - 5))
		      {
			p1.simpact_ct = 10;
			actual2 = 20;
			Hurt (1);
		      }
		    else if (P2_DN)
		      {
			if (P2_RT || com_gblock)
			  {
			    p1.bimpact_ct = 10;
			    actual2 = 18;
			    fy = 1;
			  }
			else
			  {
			    p1.simpact_ct = 10;
			    actual2 = 19;
			    Hurt (1);
			  }
		      }
		    else
		      {
			if (P2_RT || com_block)
			  {
			    p1.bimpact_ct = 10;
			    actual2 = 8;
			    fy = 1;
			  }
			else
			  {
			    p1.simpact_ct = 10;
			    actual2 = 9;
			    Hurt (1);
			  }
		      }
		    flag2 = 0;
		    fball1 = 0;
		    autho_ht1 = 0;
		    rush_stop1 = 0;
		    flag = 0;
		    actual = 0;
		    limp1 = -delayer;
		  }
	      if (flag > (p1.rush.spd * p1.rush.nbf))
		{
		  flag = (p1.rush.hit - 1) * p1.rush.spd;
		  rush_stop1++;
		  if (rush_stop1 + 1 > p1.rush.rot)
		    {
		      fball1 = 0;
		      autho_ht1 = 0;
		      rush_stop1 = 0;
		      flag = 0;
		      actual = 0;
		      limp1 = -delayer;
		    }
		}
	    }
	}
      else if ((actual == 50) && (limp1 > 0))	// SMOVE
	{
	  flag++;
	  if (flag > ((p1.smove.hit - 1) * p1.smove.spd))
	    {
	      if (flag >
		  p1.smove.spd * (p1.smove.hit +
				  (p1.smove.nbf - p1.smove.hit) / 2))
		if ((spr1x + p1.smove.offx) > (spr2x + p2.width - p2.limdefx))
		  {
		    damag2 = p1.smove.dmg;
		    p1.limoffx = spr1x + p1.smove.offx;
		    p1.limoffy = spr1y + p1.smove.offy;
		    if (spr2y < (sol2 - 5))
		      {
			p1.simpact_ct = 10;
			actual2 = 20;
			Hurt (1);
		      }
		    else if (P2_DN)
		      {
			if (P2_RT || com_gblock)
			  {
			    p1.bimpact_ct = 10;
			    actual2 = 18;
			    fy = 1;
			  }
			else
			  {
			    p1.simpact_ct = 10;
			    actual2 = 19;
			    Hurt (1);
			  }
		      }
		    else
		      {
			if (P2_RT || com_block)
			  {
			    p1.bimpact_ct = 10;
			    actual2 = 8;
			    fy = 1;
			  }
			else
			  {
			    p1.simpact_ct = 10;
			    actual2 = 9;
			    Hurt (1);
			  }
		      }
		    flag2 = 0;
		    fball1 = 0;
		    autho_ht1 = 0;
		    smove_stop1 = 0;
		    flag = 0;
		    actual = 0;
		    limp1 = -delayer;
		  }
	      if (flag > (p1.smove.spd * p1.smove.nbf))
		{
		  flag = (p1.smove.hit - 1) * p1.smove.spd;
		  smove_stop1++;
		  if (smove_stop1 + 1 > p1.smove.rot)
		    {
		      fball1 = 0;
		      autho_ht1 = 0;
		      smove_stop1 = 0;
		      flag = 0;
		      actual = 0;
		      limp1 = -delayer;
		    }
		}
	    }
	}
      else if ((actual == 30) && (limp1 > 0))	// FIREB
	{
	  flag++;
	  if (flag == ((p1.fireb.hit - 1) * p1.fireb.spd))
	    {
	      fball1 = 1;
	      fball_flg1 = 0;
	      fb1x = spr1x + fball_cx1;
	      fb1y = spr1y + fball_cy1 - (fball_h1 / 2);
	    }
	  if (flag > ((p1.fireb.spd * p1.fireb.nbf) + 10))
	    {
	      flag = 0;
	      actual = 0;
	      limp1 = -delayer;
	    }
	}
      // SPECIALS
      else if ((actual == 41) && (limp1 > 0))
	{
	  flag++;
	  if (flag > ((p1.rushx.hit - 1) * p1.rushx.spd))
	    {
	      if ((spr1x + tol1) < (spr2x + tol2))
		spr1x += p1.rushx.pix;
	      if (zoomwidth > 320)
		{
		  zoomwidth--;
		  cx++;
		}
	      if (flag >
		  p1.rushx.spd * (p1.rushx.hit +
				  (p1.rushx.nbf - p1.rushx.hit) / 2))
		if ((spr1x + p1.rushx.offx) > (spr2x + p2.width - p2.limdefx))
		  {
		    damag2 = p1.rushx.dmg;
		    p1.limoffx = spr1x + p1.rushx.offx;
		    p1.limoffy = spr1y + p1.rushx.offy;
		    if (spr2y < (sol2 - 5))
		      {
			p1.simpact_ct = 10;
			actual2 = 20;
			Hurt (1);
		      }
		    else if (P2_DN)
		      {
			if (P2_RT || com_gblock)
			  {
			    p1.bimpact_ct = 10;
			    actual2 = 18;
			    fy = 1;
			  }
			else
			  {
			    p1.simpact_ct = 10;
			    actual2 = 19;
			    Hurt (1);
			  }
		      }
		    else
		      {
			if (P2_RT || com_block)
			  {
			    p1.bimpact_ct = 10;
			    actual2 = 8;
			    fy = 1;
			  }
			else
			  {
			    p1.simpact_ct = 10;
			    actual2 = 9;
			    Hurt (1);
			  }
		      }
		    flag2 = 0;
		    fballx1 = 0;
		    fball1 = 0;
		    autho_ht1 = 0;
		    rushx_stop1 = 0;
		    flag = 0;
		    actual = 0;
		    limp1 = -delayer;
		  }
	      if (flag > (p1.rushx.spd * p1.rushx.nbf))
		{
		  flag = (p1.rushx.hit - 1) * p1.rushx.spd;
		  rushx_stop1++;
		  if (rushx_stop1 + 1 > p1.rushx.rot)
		    {
		      fball1 = 0;
		      fballx1 = 0;
		      autho_ht1 = 0;
		      rushx_stop1 = 0;
		      flag = 0;
		      actual = 0;
		      limp1 = -delayer;
		    }
		}
	    }
	}
      else if ((actual == 51) && (limp1 > 0))
	{
	  flag++;
	  if (flag > ((p1.smovex.hit - 1) * p1.smovex.spd))
	    {
	      if (flag >
		  p1.smovex.spd * (p1.smovex.hit +
				   (p1.smovex.nbf - p1.smovex.hit) / 2))
		if ((spr1x + p1.smovex.offx) >
		    (spr2x + p2.width - p2.limdefx))
		  {
		    damag2 = p1.smovex.dmg;
		    p1.limoffx = spr1x + p1.smovex.offx;
		    p1.limoffy = spr1y + p1.smovex.offy;
		    if (spr2y < (sol2 - 5))
		      {
			p1.simpact_ct = 10;
			actual2 = 20;
			Hurt (1);
		      }
		    else if (P2_DN)
		      {
			if (P2_RT || com_gblock)
			  {
			    p1.bimpact_ct = 10;
			    actual2 = 18;
			    fy = 1;
			  }
			else
			  {
			    p1.simpact_ct = 10;
			    actual2 = 19;
			    Hurt (1);
			  }
		      }
		    else
		      {
			if (P2_RT || com_block)
			  {
			    p1.bimpact_ct = 10;
			    actual2 = 8;
			    fy = 1;
			  }
			else
			  {
			    p1.simpact_ct = 10;
			    actual2 = 9;
			    Hurt (1);
			  }
		      }
		    flag2 = 0;
		    fballx1 = 0;
		    fball1 = 0;
		    autho_ht1 = 0;
		    smovex_stop1 = 0;
		    flag = 0;
		    actual = 0;
		    limp1 = -delayer;
		  }
	      if (flag > (p1.smovex.spd * p1.smovex.nbf))
		{
		  flag = (p1.smovex.hit - 1) * p1.smovex.spd;
		  smovex_stop1++;
		  if (smovex_stop1 + 1 > p1.smovex.rot)
		    {
		      fballx1 = 0;
		      fball1 = 0;
		      autho_ht1 = 0;
		      smovex_stop1 = 0;
		      flag = 0;
		      actual = 0;
		      limp1 = -delayer;
		    }
		}
	    }
	}
      else if ((actual == 31) && (limp1 > 0))
	{
	  flag++;
	  if (flag == ((p1.firebx.hit - 1) * p1.firebx.spd))
	    {
	      fballx1 = 1;
	      fballx_flg1 = 0;
	      fbx1x = spr1x + fballx_cx1;
	      fbx1y = spr1y + fballx_cy1 - (fballx_h1 / 2);
	    }
	  if (flag > ((p1.firebx.spd * p1.firebx.nbf) + 10))
	    {
	      flag = 0;
	      actual = 0;
	      limp1 = -delayer;
	    }
	}
      // END OF SPECIALS
      else if ((actual == 6) && (limp1 > 0))
	{
	  flag++;
	  if (flag > ((p1.spunch.spd * p1.spunch.nbf) - 1))
	    {
	      flag = 0;
	      actual = 0;
	      limp1 = -delayer;
	    }
	}
      else if ((actual == 61) && (limp1 > 0))
	{
	  flag++;
	  if (flag > ((p1.wpunch.spd * p1.wpunch.nbf) - 1))
	    {
	      flag = 0;
	      actual = 0;
	      limp1 = -delayer;
	    }
	}
      else if ((actual == 7) && (limp1 > 0))
	{
	  flag++;
	  if (flag > ((p1.skick.spd * p1.skick.nbf) - 1))
	    {
	      flag = 0;
	      actual = 0;
	      limp1 = -delayer;
	    }
	}
      else if ((actual == 71) && (limp1 > 0))
	{
	  flag++;
	  if (flag > ((p1.wkick.spd * p1.wkick.nbf) - 1))
	    {
	      flag = 0;
	      actual = 0;
	      limp1 = -delayer;
	    }
	}
      else if ((actual == 10) && (limp1 > 0))
	{
	  flag++;
	  if (flag > ((p1.gkick.spd * p1.gkick.nbf) - 1))
	    {
	      flag = 0;
	      actual = 0;
	      limp1 = -delayer;
	    }
	}
      else if ((actual == 13) && (limp1 > 0))
	{
	  flag++;
	  if (flag > ((p1.gpunch.spd * p1.gpunch.nbf) - 1))
	    {
	      flag = 0;
	      actual = 0;
	      limp1 = -delayer;
	    }
	}
      else if (actual == 8)
	{
	  if (fx)
	    {
	      Block ();
	      fx = 0;
	    }
	  if (spr1y < (sol1 - 1))
	    spr1y = spr1y + 3;
	  flag++;
	  if ((flag > (damag1 * 10)) && (spr1y > (sol1 - 2)))
	    actual = 0;
	}
      else if (actual == 18)
	{
	  if (fx)
	    {
	      life1 = life1 - 2;
	      Block ();
	      fx = 0;
	      refresh_lifebar ();
	    }
	  if (spr1y < (sol1 - 1))
	    spr1y = spr1y + 3;
	  flag++;
	  if ((flag > (damag1 * 10)) && (spr1y > (sol1 - 2)))
	    actual = 2;
	}
      else if (actual == 9)
	{
	  if (ak2 == (p2.akick.spd * p2.akick.hit) - 3)
	    Hurt (2);
	  else if (apch2 == (p2.apunch.spd * p2.apunch.hit) - 3)
	    Hurt (2);
	  if ((fx) && (flag2 > hitfram2))
	    {
	      Hurt (2);
	      fx = 0;
	    }
	  if (spr1y < (sol1 - 1))
	    spr1y = spr1y + 3;
	  hflag++;
	  if (hflag < 17)
	    {
	      life1 = life1 - damag1;
	      refresh_lifebar ();
	    }
	  if (hflag > 25)
	    {
	      actual = 0;
	      hflag = 0;
	    }
	  if (hflag < 7)
	  {	  
	    if ((spr1x > (limg + cx)) && (spr2x < (limd + cx)))
	      {
		spr1x--;
		spr2x += 2;
	      }
	    else
	      {
		if (spr1x > (limg + cx))
		  spr1x--;
		else if (cx > 1)
		  {
		    cx--;
		  }
		if (spr2x < (limd + cx))
		  spr2x++;

	      }
	  }
	}
      else if (actual == 19)
	{
	  if (ak2 == (p2.akick.spd * p2.akick.hit) - 3)
	    Hurt (2);
	  else if (apch2 == (p2.apunch.spd * p2.apunch.hit) - 3)
	    Hurt (2);
	  if ((fx) && (flag2 > hitfram2))
	    {
	      Hurt (2);
	      fx = 0;
	    }
	  if (spr1y < (sol1 - 1))
	    spr1y = spr1y + 3;
	  hflag++;
	  if (hflag < 17)
	    {
	      life1 = life1 - damag1;
	      refresh_lifebar ();
	    }
	  ;
	  if (hflag > 25)
	    {
	      actual = 2;
	      hflag = 0;
	    }
	  if (hflag < 7)
	  {
	    if ((spr1x > (limg + cx)) && (spr2x < (limd + cx)))
	      {
		spr1x--;
		spr2x += 2;
	      }
	    else
	      {
		if (spr1x > (limg + cx))
		  spr1x--;
		else if (cx > 1)
		  {
		    cx--;
		  }
		if (spr2x < (limd + cx))
		  spr2x++;
	      }
	  }
	}
      else if (actual == 20)
	{
	  if (ak2 == (p2.akick.spd * p2.akick.hit) - 3)
	    Hurt (2);
	  else if (apch2 == (p2.apunch.spd * p2.apunch.hit) - 3)
	    Hurt (2);
	  if ((fx) && (flag2 > hitfram2))
	    {
	      Hurt (2);
	      fx = 0;
	    }
	  if (spr1y < (sol1 - 1))
	    spr1y = spr1y + 2;
	  hflag++;
	  if (hflag < 25)
	    {
	      life1 = life1 - damag1;
	      refresh_lifebar ();
	    }
	  if (hflag > 42)
	    {
	      actual = 0;
	      hflag = 0;
	    }
	  if (hflag < 15)
	  {
	    if ((spr1x > (limg + cx)) && (spr2x < (limd + cx)))
	      {
		spr1x--;
		spr2x += 2;
	      }
	    else
	      {
		if (spr1x > (limg + cx))
		  spr1x--;
		else if (cx > 1)
		  {
		    cx--;
		  }
		if (spr2x < (limd + cx))
		  spr2x++;
	      }
	  }
	}

		/******** PLAYER 2 ***********/
#if GAME_DEBUG >= 2
      TRON ("PLYER2");
#endif
      if (actual2 == 3)		// SIMPLE JUMP
	{
	  if (spr2y < jump_how2)
	    sens2 = 1;
	  if (sens2)
	    {
	      if (spr2y < (jump_how2 + 4))
		spr2y++;
	      else if (spr2y < (jump_how2 + 17))
		spr2y += 2;
	      else
		spr2y += 3;
	      if (spr2y > sol2)
		{
		  actual2 = 0;
		  spr2y = sol2;
		  sens2 = 0;
		  limak2 = 1;
		}
	    }
	  else
	    {
	      if (spr2y < (jump_how2 + 4))
		spr2y--;
	      else if (spr2y < (jump_how2 + 17))
		spr2y -= 2;
	      else
		spr2y -= 3;
	    }
	}
      else if (actual2 == 4)	// JUMP FORWARD
	{
	  if (spr2y < jump_how2)
	    sens2 = 1;
	  if (sens2)
	    {
	      if (spr2y < (jump_how2 + 4))
		spr2y++;
	      else if (spr2y < (jump_how2 + 30))
		spr2y += 2;
	      else
		spr2y += 3;
	      if (zoomwidth > 320)
		zoomwidth--;
#if TEMP == 0

	      if ((spr2x + tol2) > (spr1x + tol1))
#endif

		spr2x -= 2;
#if TEMP == 0

	      else
#endif
	      if (spr1x > (limg + cx))
		{
		  spr1x--;
		  spr2x--;

		}
	      else if (cx > 1)
		{
		  cx--;
		  spr1x--;
		  spr2x--;
		}
	      if (spr2y > sol2)
		{
		  actual2 = 0;
		  spr2y = sol2;
		  sens2 = 0;
		  limak2 = 1;
#if TEMP > 0

		  if (spr1x > spr2x)
		    {
		      inverted = !inverted;
		      spr1x = zoomscreen->w - spr1x - Player1.Jump[1]->w + 1;
		      spr2x = zoomscreen->w - spr2x - Player2.Jump[1]->w + 1;
		    }
#endif

		}
	    }
	  else
	    {
	      if (spr2y < (jump_how2 + 4))
		spr2y--;
	      else if (spr2y < (jump_how2 + 30))
		spr2y -= 2;
	      else
		spr2y -= 3;
	      if (zoomwidth > 320)
		zoomwidth--;
#if TEMP == 0

	      if ((spr2x + tol2) > (spr1x + tol1))
#endif

		spr2x -= 2;
#if TEMP == 0

	      else
#endif
	      if (spr1x > (limg + cx))
		{
		  spr1x--;
		  spr2x--;
		}
	      else if (cx > 1)
		{
		  cx--;
		  spr1x--;
		  spr2x--;
		}
	    }
	}
      else if (actual2 == 5)	// JUMP BACKWARD
	{
	  if (spr2y < jump_how2)
	    sens2 = 1;
	  if (sens2)
	    {
	      if (spr2y < (jump_how2 + 4))
		spr2y++;
	      else if (spr2y < (jump_how2 + 30))
		spr2y += 2;
	      else
		spr2y += 3;
	      if (spr2x < (limd + cx))
		spr2x += 2;
	      else if (spr1x > (limg + cx))
		for (i = 0; i < s; i++)
		  {
		    if (cx < (bkgdlim + 320 - zoomwidth))
		      {
			cx++;
			spr2x++;
		      }
		  }
	      else
		for (i = 0; i < s; i++)
		  {
		    if (cx < (bkgdlim + 320 - zoomwidth))
		      if (zoomwidth < ((200 + zoomsup) * 1.6))
			{
			  zoomwidth++;
			  spr2x++;
			}
		  }
	      if (spr2y > sol2)
		{
		  actual2 = 0;
		  spr2y = sol2;
		  sens2 = 0;
		  limak2 = 1;
		}
	    }
	  else
	    {
	      if (spr2y < (jump_how2 + 4))
		spr2y--;
	      else if (spr2y < (jump_how2 + 30))
		spr2y -= 2;
	      else
		spr2y -= 3;
	      if (spr2x < (limd + cx))
		spr2x += 2;
	      else if (spr1x > (limg + cx))
		for (i = 0; i < s; i++)
		  {
		    if (cx < (bkgdlim + 320 - zoomwidth))
		      {
			cx++;
			spr2x++;
		      }
		  }
	      else
		for (i = 0; i < s; i++)
		  {
		    if (cx < (bkgdlim + 320 - zoomwidth))
		      if (zoomwidth < ((200 + zoomsup) * 1.6))
			{
			  zoomwidth++;
			  spr2x++;
			}
		  }
	    }
	}
      else if ((actual2 == 40) && (limp2 > 0))
	{
	  flag2++;
	  if (flag2 > ((p2.rush.hit - 1) * p2.rush.spd))
	    {
	      if ((spr2x + tol2) > (spr1x + tol1))
		spr2x -= p2.rush.pix;
	      if (zoomwidth > 320)
		zoomwidth--;
	      if (flag2 >
		  p2.rush.spd * (p2.rush.hit +
				 (p2.rush.nbf - p2.rush.hit) / 2))
		if ((spr2x - (p2.rush.offx - p2.width)) <
		    (spr1x + p1.limdefx))
		  {
		    damag1 = p2.rush.dmg;
		    p2.limoffx = spr2x - (p2.rush.offx - p2.width);
		    p2.limoffy = spr2y + p2.rush.offy;
		    if (spr1y < (sol1 - 5))
		      {
			p2.simpact_ct = 10;
			actual = 20;
			Hurt (1);
		      }
		    else if (P1_DN)
		      {
			if (P1_LT)
			  {
			    p2.bimpact_ct = 10;
			    actual = 18;
			    fx = 1;
			  }
			else
			  {
			    p2.simpact_ct = 10;
			    actual = 19;
			    Hurt (1);
			  }
		      }
		    else
		      {
			if (P1_LT)
			  {
			    p2.bimpact_ct = 10;
			    actual = 8;
			    fx = 1;
			  }
			else
			  {
			    p2.simpact_ct = 10;
			    actual = 9;
			    Hurt (1);
			  }
		      }
		    flag = 0;
		    fball2 = 0;
		    autho_ht2 = 0;
		    rush_stop2 = 0;
		    flag2 = 0;
		    actual2 = 0;
		    limp2 = -delayer;
		  }
	      if (flag2 > (p2.rush.spd * p2.rush.nbf))
		{
		  flag2 = (p2.rush.hit - 1) * p2.rush.spd;
		  rush_stop2++;
		  if (rush_stop2 + 1 > p2.rush.rot)
		    {
		      fball2 = 0;
		      autho_ht2 = 0;
		      rush_stop2 = 0;
		      flag2 = 0;
		      actual2 = 0;
		      limp2 = -delayer;
		    }
		}
	    }
	}
      else if ((actual2 == 50) && (limp2 > 0))
	{
	  flag2++;
	  if (flag2 > ((p2.smove.hit - 1) * p2.smove.spd))
	    {
	      if (flag2 >
		  p2.smove.spd * (p2.smove.hit +
				  (p2.smove.nbf - p2.smove.hit) / 2))
		if ((spr2x - (p2.smove.offx - p2.width)) <
		    (spr1x + p1.limdefx))
		  {
		    damag1 = p2.smove.dmg;
		    p2.limoffx = spr2x - (p2.smove.offx - p2.width);
		    p2.limoffy = spr2y + p2.smove.offy;
		    if (spr1y < (sol1 - 5))
		      {
			p2.simpact_ct = 10;
			actual = 20;
			Hurt (1);
		      }
		    else if (P1_DN)
		      {
			if (P1_LT)
			  {
			    p2.bimpact_ct = 10;
			    actual = 18;
			    fx = 1;
			  }
			else
			  {
			    p2.simpact_ct = 10;
			    actual = 19;
			    Hurt (1);
			  }
		      }
		    else
		      {
			if (P1_LT)
			  {
			    p2.bimpact_ct = 10;
			    actual = 8;
			    fx = 1;
			  }
			else
			  {
			    p2.simpact_ct = 10;
			    actual = 9;
			    Hurt (1);
			  }
		      }
		    flag = 0;
		    fball2 = 0;
		    autho_ht2 = 0;
		    smove_stop2 = 0;
		    flag2 = 0;
		    actual2 = 0;
		    limp2 = -delayer;
		  }
	      if (flag2 > (p2.smove.spd * p2.smove.nbf))
		{
		  flag2 = (p2.smove.hit - 1) * p2.smove.spd;
		  smove_stop2++;
		  if (smove_stop2 + 1 > p2.smove.rot)
		    {
		      fball2 = 0;
		      autho_ht2 = 0;
		      smove_stop2 = 0;
		      flag2 = 0;
		      actual2 = 0;
		      limp2 = -delayer;
		    }
		}
	    }
	}
      else if ((actual2 == 30) && (limp2 > 0))
	{
	  flag2++;
	  if (flag2 == ((p2.fireb.hit - 1) * p2.fireb.spd))
	    {
	      fball2 = 1;
	      fball_flg2 = 0;
	      fb2x = spr2x + (p2.width - fball_cx2) - p2.fball.w;
	      fb2y = spr2y + fball_cy2 - (fball_h2 / 2);
	    }
	  if (flag2 > ((p2.fireb.spd * p2.fireb.nbf) + 10))
	    {
	      flag2 = 0;
	      actual2 = 0;
	      limp2 = -delayer;
	    }
	}
      // SPECIALS
      else if ((actual2 == 41) && (limp2 > 0))
	{
	  flag2++;
	  if (flag2 > ((p2.rushx.hit - 1) * p2.rushx.spd))
	    {
	      if ((spr2x + tol2) > (spr1x + tol1))
		spr2x -= p2.rushx.pix;
	      if (zoomwidth > 320)
		zoomwidth--;
	      if (flag2 >
		  p2.rushx.spd * (p2.rushx.hit +
				  (p2.rushx.nbf - p2.rushx.hit) / 2))
		if ((spr2x - (p2.rushx.offx - p2.width)) <
		    (spr1x + p1.limdefx))
		  {
		    damag1 = p2.rushx.dmg;
		    p2.limoffx = spr2x - (p2.rushx.offx - p2.width);
		    p2.limoffy = spr2y + p2.rushx.offy;
		    if (spr1y < (sol1 - 5))
		      {
			p2.simpact_ct = 10;
			actual = 20;
			Hurt (1);
		      }
		    else if (P1_DN)
		      {
			if (P1_LT)
			  {
			    p2.bimpact_ct = 10;
			    actual = 18;
			    fx = 1;
			  }
			else
			  {
			    p2.simpact_ct = 10;
			    actual = 19;
			    Hurt (1);
			  }
		      }
		    else
		      {
			if (P1_LT)
			  {
			    p2.bimpact_ct = 10;
			    actual = 8;
			    fx = 1;
			  }
			else
			  {
			    p2.simpact_ct = 10;
			    actual = 9;
			    Hurt (1);
			  }
		      }
		    flag = 0;
		    fball2 = 0;
		    fballx2 = 0;
		    autho_ht2 = 0;
		    rushx_stop2 = 0;
		    flag2 = 0;
		    actual2 = 0;
		    limp2 = -delayer;
		  }
	      if (flag2 > (p2.rushx.spd * p2.rushx.nbf))
		{
		  flag2 = (p2.rushx.hit - 1) * p2.rushx.spd;
		  rushx_stop2++;
		  if (rushx_stop2 + 1 > p2.rushx.rot)
		    {
		      fball2 = 0;
		      fballx2 = 0;
		      autho_ht2 = 0;
		      rushx_stop2 = 0;
		      flag2 = 0;
		      actual2 = 0;
		      limp2 = -delayer;
		    }
		}
	    }
	}
      else if ((actual2 == 51) && (limp2 > 0))
	{
	  flag2++;
	  if (flag2 > ((p2.smovex.hit - 1) * p2.smovex.spd))
	    {
	      if (flag2 >
		  p2.smovex.spd * (p2.smovex.hit +
				   (p2.smovex.nbf - p2.smovex.hit) / 2))
		if ((spr2x - (p2.smovex.offx - p2.width)) <
		    (spr1x + p1.limdefx))
		  {
		    damag1 = p2.smovex.dmg;
		    p2.limoffx = spr2x - (p2.smovex.offx - p2.width);
		    p2.limoffy = spr2y + p2.smovex.offy;
		    if (spr1y < (sol1 - 5))
		      {
			p2.simpact_ct = 10;
			actual = 20;
			Hurt (1);
		      }
		    else if (P1_DN)
		      {
			if (P1_LT)
			  {
			    p2.bimpact_ct = 10;
			    actual = 18;
			    fx = 1;
			  }
			else
			  {
			    p2.simpact_ct = 10;
			    actual = 19;
			    Hurt (1);
			  }
		      }
		    else
		      {
			if (P1_LT)
			  {
			    p2.bimpact_ct = 10;
			    actual = 8;
			    fx = 1;
			  }
			else
			  {
			    p2.simpact_ct = 10;
			    actual = 9;
			    Hurt (1);
			  }
		      }
		    flag = 0;
		    fball2 = 0;
		    fballx2 = 0;
		    autho_ht2 = 0;
		    smovex_stop2 = 0;
		    flag2 = 0;
		    actual2 = 0;
		    limp2 = -delayer;
		  }
	      if (flag2 > (p2.smovex.spd * p2.smovex.nbf))
		{
		  flag2 = (p2.smovex.hit - 1) * p2.smovex.spd;
		  smovex_stop2++;
		  if (smovex_stop2 + 1 > p2.smovex.rot)
		    {
		      fball2 = 0;
		      fballx2 = 0;
		      autho_ht2 = 0;
		      smovex_stop2 = 0;
		      flag2 = 0;
		      actual2 = 0;
		      limp2 = -delayer;
		    }
		}
	    }
	}
      else if ((actual2 == 31) && (limp2 > 0))
	{
	  flag2++;
	  if (flag2 == ((p2.firebx.hit - 1) * p2.firebx.spd))
	    {
	      fballx2 = 1;
	      fballx_flg2 = 0;
	      fbx2x = spr2x + (p2.width - fballx_cx2) - p2.fballx.w;
	      fbx2y = spr2y + fballx_cy2 - (fballx_h2 / 2);
	    }
	  if (flag2 > ((p2.firebx.spd * p2.firebx.nbf) + 10))
	    {
	      flag2 = 0;
	      actual2 = 0;
	      limp2 = -delayer;
	    }
	}
      // END OF SPECIALS
      else if ((actual2 == 6) && (limp2 > 0))
	{
	  flag2++;
	  if (flag2 > ((p2.spunch.spd * p2.spunch.nbf) - 1))
	    {
	      flag2 = 0;
	      actual2 = 0;
	      limp2 = -delayer;
	    }
	}
      else if ((actual2 == 61) && (limp2 > 0))
	{
	  flag2++;
	  if (flag2 > ((p2.wpunch.spd * p2.wpunch.nbf) - 1))
	    {
	      flag2 = 0;
	      actual2 = 0;
	      limp2 = -delayer;
	    }
	}
      else if ((actual2 == 7) && (limp2 > 0))
	{
	  flag2++;
	  if (flag2 > ((p2.skick.spd * p2.skick.nbf) - 1))
	    {
	      flag2 = 0;
	      actual2 = 0;
	      limp2 = -delayer;
	    }
	}
      else if ((actual2 == 71) && (limp2 > 0))
	{
	  flag2++;
	  if (flag2 > ((p2.wkick.spd * p2.wkick.nbf) - 1))
	    {
	      flag2 = 0;
	      actual2 = 0;
	      limp2 = -delayer;
	    }
	}
      else if ((actual2 == 10) && (limp2 > 0))
	{
	  flag2++;
	  if (flag2 > ((p2.gkick.spd * p2.gkick.nbf) - 1))
	    {
	      flag2 = 0;
	      actual2 = 0;
	      limp2 = -delayer;
	    }
	}
      else if ((actual2 == 13) && (limp2 > 0))
	{
	  flag2++;
	  if (flag2 > ((p2.gpunch.spd * p2.gpunch.nbf) - 1))
	    {
	      flag2 = 0;
	      actual2 = 0;
	      limp2 = -delayer;
	    }
	}
      else if (actual2 == 8)
	{
	  if (fy)
	    {
	      Block ();
	      fy = 0;
	    }
	  if (spr2y < (sol2 - 1))
	    spr2y = spr2y + 3;
	  flag2++;
	  if ((flag2 > (damag2 * 10)) && (spr2y > (sol2 - 2)))
	    actual2 = 0;
	}
      else if (actual2 == 18)
	{
	  if (fy)
	    {
	      life2 = life2 - 2;
	      Block ();
	      fy = 0;
	      refresh_lifebar ();
	    }
	  if (spr2y < (sol2 - 1))
	    spr2y = spr2y + 3;
	  flag2++;
	  if ((flag2 > (damag2 * 10)) && (spr2y > (sol2 - 2)))
	    actual2 = 2;
	}
      else if (actual2 == 9)
	{
	  if (ak == (p1.akick.hit * p1.akick.spd) - 3)
	    Hurt (2);
	  else if (apch == (p1.apunch.hit * p1.apunch.spd) - 3)
	    Hurt (2);
	  if ((fy) && (flag > hitfram1))
	    {
	      Hurt (2);
	      fy = 0;
	    }
	  if (spr2y < (sol2 - 1))
	    spr2y = spr2y + 3;
	  hflag2++;
	  if (hflag2 < 17)
	    {
	      life2 = life2 - damag2;
	      refresh_lifebar ();
	    }
	  if (hflag2 > 25)
	    {
	      actual2 = 0;
	      hflag2 = 0;
	    }
	  if (hflag2 < 7)
	  {
	    if ((spr2x < (limd + cx)) && (spr1x > (limg + cx)))
	      {
		spr2x++;
		spr1x -= 2;
	      }
	    else
	      {
		if (spr2x < (limd + cx))
		  spr2x++;
		else if (cx < (bkgdlim + 320 - zoomwidth))
		  {
		    cx++;
		  }
		if (spr1x > (cx + limg))
		  spr1x--;
	      }
	  }
	}
      else if (actual2 == 19)
	{
	  if (ak == (p1.akick.hit * p1.akick.spd) - 3)
	    Hurt (2);
	  else if (apch == (p1.apunch.hit * p1.apunch.spd) - 3)
	    Hurt (2);
	  if ((fy) && (flag > hitfram1))
	    {
	      Hurt (2);
	      fy = 0;
	    }
	  if (spr2y < (sol2 - 1))
	    spr2y = spr2y + 3;
	  hflag2++;
	  if (hflag2 < 17)
	    {
	      life2 = life2 - damag2;
	      refresh_lifebar ();
	    }
	  if (hflag2 > 25)
	    {
	      actual2 = 2;
	      hflag2 = 0;
	    }
	  if (hflag2 < 7)
	  {
	    if ((spr2x < (limd + cx)) && (spr1x > (limg + cx)))
	      {
		spr2x++;
		spr1x -= 2;
	      }
	    else
	      {
		if (spr2x < (limd + cx))
		  spr2x++;
		else if (cx < (bkgdlim + 320 - zoomwidth))
		  {
		    cx++;
		  }
		if (spr1x > (cx + limg))
		  spr1x--;
	      }
	  }
	}
      else if (actual2 == 20)
	{
	  if (ak == (p1.akick.hit * p1.akick.spd) - 3)
	    Hurt (2);
	  else if (apch == (p1.apunch.hit * p1.apunch.spd) - 3)
	    Hurt (2);
	  if ((fy) && (flag > hitfram1))
	    {
	      Hurt (2);
	      fy = 0;
	    }
	  if (spr2y < (sol2 - 1))
	    spr2y = spr2y + 2;
	  hflag2++;
	  if (hflag2 < 25)
	    {
	      life2 = life2 - damag2;
	      refresh_lifebar ();
	    }
	  if (hflag2 > 42)
	    {
	      actual2 = 0;
	      hflag2 = 0;
	    }
	  if (hflag2 < 15)
	  {
	    if ((spr2x < (limd + cx)) && (spr1x > (limg + cx)))
	      {
		spr2x++;
		spr1x -= 2;
	      }
	    else
	      {
		if (spr2x < (limd + cx))
		  spr2x++;
		else if (cx < (bkgdlim + 320 - zoomwidth))
		  {
		    cx++;
		  }
		if (spr1x > (cx + limg))
		  spr1x--;
	      }
	  }
	}

		/************** UPDATE FIREBALLS **************/
#if GAME_DEBUG >= 2
      TRON ("UPDATE FIREBALLS");
#endif
      if (fball1)
	{
	  fb1x += p1.fball.pix;
	  fball_flg1++;
	  if (fball_flg1 > (p1.fball.nbf * p1.fball.spd) - 1)
	    fball_flg1 = 0;
	  if (fball2)
	    {
	      if ((fb1x + p1.fball.w) > (fb2x + (p2.fball.w / 3)))
		{
		  fball1 = 0;
		  fball2 = 0;
		  p1.limoffx = fb1x + p1.fball.offx;
		  p1.limoffy = fb1y + p1.fball.offy;
		  p1.bimpact_ct = 10;
		  Hurt (1);
		}
	    }
	  else if (fballx2)
	    {
	      if ((fb1x + p1.fball.w) > (fbx2x + (p2.fballx.w / 3)))
		{
		  fball1 = 0;
		  fballx2 = 0;
		  p1.limoffx = fb1x + p1.fball.offx;
		  p1.limoffy = fb1y + p1.fball.offy;
		  p1.bimpact_ct = 10;
		  Hurt (1);
		}
	    }
	  else if ((fb1x + p1.fball.w) > (spr2x + (p2.width / 3)))
	    {
	      if (fb1x < (spr2x + (p2.width / 2)))
		{
		  if (autho_ht1 && (spr2y > (sol2 - 30))
		      && (fb1x > (spr1x + fball_cx1 + 10)))
		    {
		      damag2 = p1.fireb.dmg;
		      p1.limoffx = fb1x + p1.fball.offx;
		      p1.limoffy = fb1y + p1.fball.offy;
		      if (spr2y < (sol2 - 5))
			{
			  p1.simpact_ct = 10;
			  actual2 = 20;
			  Hurt (1);
			}
		      else if (P2_DN)
			{
			  if (P2_RT || com_gblock)
			    {
			      p1.bimpact_ct = 10;
			      actual2 = 18;
			      fy = 1;
			    }
			  else
			    {
			      p1.simpact_ct = 10;
			      actual2 = 19;
			      Hurt (1);
			    }
			}
		      else
			{
			  if (P2_RT || com_block)
			    {
			      p1.bimpact_ct = 10;
			      actual2 = 8;
			      fy = 1;
			    }
			  else
			    {
			      p1.simpact_ct = 10;
			      actual2 = 9;
			      Hurt (1);
			    }
			}
		      flag2 = 0;
		      fball1 = 0;
		      autho_ht1 = 0;
		    }
		}
	      else if (fb1x > (cx + zoomwidth))
		fball1 = 0;
	    }
	}
      if (fball2)
	{
	  fb2x -= p2.fball.pix;
	  fball_flg2++;
	  if (fball_flg2 > (p2.fball.nbf * p2.fball.spd) - 1)
	    fball_flg2 = 0;
	  if (fball1)
	    {
	      if (fb2x < (fb1x + (2 * p1.fball.w / 3)))
		{
		  fball1 = 0;
		  fball2 = 0;
		  p2.limoffx = fb2x - (p2.fball.w - p2.fball.offx);
		  p2.limoffy = fb2y + p2.fball.offy;
		  p2.bimpact_ct = 10;
		  Hurt (1);
		}
	    }
	  else if (fballx1)
	    {
	      if (fb2x < (fbx1x + (2 * p1.fballx.w / 3)))
		{
		  fballx1 = 0;
		  fball2 = 0;
		  p2.limoffx = fb2x - (p2.fball.w - p2.fball.offx);
		  p2.limoffy = fb2y + p2.fball.offy;
		  p2.bimpact_ct = 10;
		  Hurt (1);
		}
	    }
	  else if (fb2x < (spr1x + (2 * p1.width / 3)))
	    {
	      if ((fb2x + p2.fball.w) > (spr1x - (p1.width / 2)))
		{
		  if (autho_ht2 && (spr1y > (sol1 - 30))
		      && (fb2x <
			  (spr2x + (p2.width - fball_cx2) - p2.fball.w - 10)))
		    {
		      damag1 = p2.fireb.dmg;
		      p2.limoffx = fb2x - (p2.fball.w - p2.fball.offx);
		      p2.limoffy = fb2y + p2.fball.offy;
		      if (spr1y < (sol1 - 5))
			{
			  p2.simpact_ct = 10;
			  actual = 20;
			  Hurt (1);
			}
		      else if (P1_DN)
			{
			  if (P1_LT)
			    {
			      p2.bimpact_ct = 10;
			      actual = 18;
			      fx = 1;
			    }
			  else
			    {
			      p2.simpact_ct = 10;
			      actual = 19;
			      Hurt (1);
			    }
			}
		      else
			{
			  if (P1_LT)
			    {
			      p2.bimpact_ct = 10;
			      actual = 8;
			      fx = 1;
			    }
			  else
			    {
			      p2.simpact_ct = 10;
			      actual = 9;
			      Hurt (1);
			    }
			}
		      flag = 0;
		      fball2 = 0;
		      autho_ht2 = 0;
		    }
		}
	      else if (fb2x < (cx - 20))
		fball2 = 0;
	    }
	}
#if GAME_DEBUG >= 2
      TRON ("NOW FOR SPECIALS");
#endif
      // NOW FOR SPECIALS
      if (fballx1)
	{
	  fbx1x += p1.fballx.pix;
	  fballx_flg1++;
	  if (fballx_flg1 > (p1.fballx.nbf * p1.fballx.spd) - 1)
	    fballx_flg1 = 0;
	  if (fball2)
	    {
	      if ((fbx1x + p1.fballx.w) > (fb2x + (p2.fball.w / 3)))
		{
		  fballx1 = 0;
		  fball2 = 0;
		  p1.limoffx = fbx1x + p1.fballx.offx;
		  p1.limoffy = fbx1y + p1.fballx.offy;
		  p1.bimpact_ct = 10;
		  Hurt (1);
		}
	    }
	  else if (fballx2)
	    {
	      if ((fbx1x + p1.fballx.w) > (fbx2x + (p2.fballx.w / 3)))
		{
		  fballx1 = 0;
		  fballx2 = 0;
		  p1.limoffx = fbx1x + p1.fballx.offx;
		  p1.limoffy = fbx1y + p1.fballx.offy;
		  p1.bimpact_ct = 10;
		  Hurt (1);
		}
	    }
	  else if ((fbx1x + p1.fballx.w) > (spr2x + (p2.width / 3)))
	    {
	      if (fbx1x < (spr2x + (p2.width / 2)))
		{
		  if (autho_ht1 && (spr2y > (sol2 - 30))
		      && (fbx1x > (spr1x + fballx_cx1 + 10)))
		    {
		      damag2 = p1.firebx.dmg;
		      p1.limoffx = fbx1x + p1.fballx.offx;
		      p1.limoffy = fbx1y + p1.fballx.offy;
		      if (spr2y < (sol2 - 5))
			{
			  p1.simpact_ct = 10;
			  actual2 = 20;
			  Hurt (1);
			}
		      else if (P2_DN)
			{
			  if (P2_RT || com_gblock)
			    {
			      p1.bimpact_ct = 10;
			      actual2 = 18;
			      fy = 1;
			    }
			  else
			    {
			      p1.simpact_ct = 10;
			      actual2 = 19;
			      Hurt (1);
			    }
			}
		      else
			{
			  if (P2_RT || com_block)
			    {
			      p1.bimpact_ct = 10;
			      actual2 = 8;
			      fy = 1;
			    }
			  else
			    {
			      p1.simpact_ct = 10;
			      actual2 = 9;
			      Hurt (1);
			    }
			}
		      flag2 = 0;
		      fballx1 = 0;
		      autho_ht1 = 0;
		    }
		}
	      else if (fb1x > (cx + zoomwidth))
		fballx1 = 0;
	    }
	}
      if (fballx2)
	{
	  fbx2x -= p2.fballx.pix;
	  fballx_flg2++;
	  if (fballx_flg2 > (p2.fballx.nbf * p2.fballx.spd) - 1)
	    fballx_flg2 = 0;
	  if (fball1)
	    {
	      if (fbx2x < (fb1x + (2 * p1.fball.w / 3)))
		{
		  fball1 = 0;
		  fballx2 = 0;
		  p2.limoffx = fbx2x - (p2.fballx.w - p2.fballx.offx);
		  p2.limoffy = fbx2y + p2.fballx.offy;
		  p2.bimpact_ct = 10;
		  Hurt (1);
		}
	    }
	  else if (fballx1)
	    {
	      if (fbx2x < (fbx1x + (2 * p1.fballx.w / 3)))
		{
		  fballx1 = 0;
		  fballx2 = 0;
		  p2.limoffx = fbx2x - (p2.fballx.w - p2.fballx.offx);
		  p2.limoffy = fbx2y + p2.fballx.offy;
		  p2.bimpact_ct = 10;
		  Hurt (1);
		}
	    }
	  else if (fbx2x < (spr1x + (2 * p1.width / 3)))
	    {
	      if ((fbx2x + p2.fballx.w) > (spr1x - (p1.width / 2)))
		{
		  if (autho_ht2 && (spr1y > (sol1 - 30))
		      && (fbx2x <
			  (spr2x + (p2.width - fballx_cx2) -
			   p2.fballx.w - 10)))
		    {
		      damag1 = p2.firebx.dmg;
		      p2.limoffx = fbx2x - (p2.fballx.w - p2.fballx.offx);
		      p2.limoffy = fbx2y + p2.fballx.offy;
		      if (spr1y < (sol1 - 5))
			{
			  p2.simpact_ct = 10;
			  actual = 20;
			  Hurt (1);
			}
		      else if (P1_DN)
			{
			  if (P1_LT)
			    {
			      p2.bimpact_ct = 10;
			      actual = 18;
			      fx = 1;
			    }
			  else
			    {
			      p2.simpact_ct = 10;
			      actual = 19;
			      Hurt (1);
			    }
			}
		      else
			{
			  if (P1_LT)
			    {
			      p2.bimpact_ct = 10;
			      actual = 8;
			      fx = 1;
			    }
			  else
			    {
			      p2.simpact_ct = 10;
			      actual = 9;
			      Hurt (1);
			    }
			}
		      flag = 0;
		      fballx2 = 0;
		      autho_ht2 = 0;
		    }
		}
	      else if (fb2x < (cx - 20))
		fballx2 = 0;
	    }
	}

		/************** CHECKING PLAYER 1 INPUTS*****************/
#if GAME_DEBUG >= 2
      TRON ("CHECKING PLAYER 1 INPUTS");
#endif
      if ((actual < 3) || (actual > 100))	// if actual position not
	// prioritary
	{
	  actual = 0;
	  poll_joystick ();
	  if (P1_LT && P1_UP)
	    {
	      actual = 4;
	    }
	  else if (P1_RT && P1_UP)
	    {
	      actual = 5;
	    }
	  else if (P1_UP)
	    {
	      actual = 3;
	    }
	  else if (P1_DN)
	    {
	      actual = 2;
	    }
	  else if (P1_LT)
	    {
	      if (spr1x > (limg + cx))
		spr1x -= 2;
	      else if (spr2x < (limd + cx))
		for (i = 0; i < s; i++)
		  {
		    if (cx > 1)
		      {
			cx--;
			spr1x--;
		      }
		  }
	      else
		for (i = 0; i < s; i++)
		  {
		    if (cx > 1)
		      if (zoomwidth < ((200 + zoomsup) * 1.6))
			{
			  cx--;
			  spr1x--;
			  zoomwidth++;
			}
		  }
	      flag--;
	      if (flag < 0)
		flag = ((p1.walk.spd * p1.walk.nbf) - 1);
	      actual = 111;
	    }
	  else if (P1_RT)
	    {
	      if (zoomwidth > 320)
		{
		  zoomwidth--;
		  cx++;
		}
	      if ((spr1x + tol1) < (spr2x + tol2))
		spr1x += 2;
	      else if (spr2x < (limd + cx))
		{
		  spr1x++;
		  spr2x++;
		}
	      else if (cx < (bkgdlim + 320 - zoomwidth))
		{
		  cx++;
		  spr1x++;
		  spr2x++;
		}
	      flag++;
	      if (flag > ((p1.walk.spd * p1.walk.nbf) - 1))
		flag = 0;
	      actual = 1;
	    }
	  if (((actual < 2) || (actual > 100)) && (limp1 > 0)
	      && !p1.fireb.succ && !p1.rush.succ && !p1.smove.succ
	      && !p1.firebx.succ && !p1.rushx.succ && !p1.smovex.succ)
	    {
	      if (P1_SP)
		{
		  Paf (2);
		  actual = 6;
		  flag = 0;
		  fy = 1;
		}
	      else if (P1_WP)
		{
		  Paf (1);
		  actual = 61;
		  flag = 0;
		  fy = 1;
		}
	      else if (P1_SK)
		{
		  Paf (4);
		  actual = 7;
		  flag = 0;
		  fy = 1;
		}
	      else if (P1_WK)
		{
		  Paf (3);
		  actual = 71;
		  flag = 0;
		  fy = 1;
		}
	    }
	  if ((actual == 2) && (limp1 > 0))
	    {
	      if (P1_WK || P1_SK)
		{
		  Paf (1);
		  actual = 10;
		  flag = 0;
		  fy = 1;
		}
	      else if (P1_SP || P1_WP)
		{
		  Paf (4);
		  actual = 13;
		  flag = 0;
		  fy = 1;
		}
	    }
	  if (p1.fireb.flg)
	    {
	      if (check_kstatus1 (p1.fireb.seq[0]) && !p1.fireb.succ)
		{
		  p1.fireb.succ = 1;
		  p1.fireb.nb = 0;
		}
	      else if (p1.fireb.succ)
		{
		  if ((last_status1 ==
		       p1.fireb.seq[(int)p1.fireb.nb])
		      || (last_status1 == p1.fireb.seq[p1.fireb.nb + 1]))
		    {
		      if (check_kstatus1 (p1.fireb.seq[p1.fireb.nb + 1]))
			{
			  p1.fireb.nb++;
			  p1.fireb.succ = p1.fireb.nb + 1;
			  if (p1.fireb.succ == p1.fireb.end)
			    {
			      //aqui
			      playwav (DEF_hadoken1);
			      p1.fireb.succ = 0;
			      actual = 30;
			      flag = 0;
			      autho_ht1 = 1;
			      spower1 += 10;
			      refresh_powerbar ();
			    }
			}
		      else if (!check_kstatus1 (last_status1))
			p1.fireb.succ = 0;
		    }
		  else
		    p1.fireb.succ = 0;
		}
	      else
		p1.fireb.succ = 0;
	    }
	  else
	    p1.fireb.succ = 0;
	  if (p1.rush.flg)
	    {
	      if (check_kstatus1 (p1.rush.seq[0]) && !p1.rush.succ)
		{
		  p1.rush.succ = 1;
		  p1.rush.nb = 0;
		}
	      else if (p1.rush.succ)
		{
		  if ((last_status1 == p1.rush.seq[(int)p1.rush.nb])
		      || (last_status1 == p1.rush.seq[p1.rush.nb + 1]))
		    {
		      if (check_kstatus1 (p1.rush.seq[p1.rush.nb + 1]))
			{
			  p1.rush.nb++;
			  p1.rush.succ = p1.rush.nb + 1;
			  if (p1.rush.succ == p1.rush.end)
			    {
			      //aqui
			      playwav (DEF_copter1);
			      p1.rush.succ = 0;
			      actual = 40;
			      flag = 0;
			      autho_ht1 = 1;
			      spower1 += 10;
			      refresh_powerbar ();
			    }
			}
		      else if (!check_kstatus1 (last_status1))
			p1.rush.succ = 0;
		    }
		  else
		    p1.rush.succ = 0;
		}
	      else
		p1.rush.succ = 0;
	    }
	  else
	    p1.rush.succ = 0;
	  if (p1.smove.flg)
	    {
	      if (check_kstatus1 (p1.smove.seq[0]) && !p1.smove.succ)
		{
		  p1.smove.succ = 1;
		  p1.smove.nb = 0;
		}
	      else if (p1.smove.succ)
		{
		  if ((last_status1 == p1.smove.seq[(int)p1.smove.nb])
		      || (last_status1 == p1.smove.seq[p1.smove.nb + 1]))
		    {
		      if (check_kstatus1 (p1.smove.seq[p1.smove.nb + 1]))
			{
			  p1.smove.nb++;
			  p1.smove.succ = p1.smove.nb + 1;
			  if (p1.smove.succ == p1.smove.end)
			    {
			      playwav (DEF_dragon1);
			      p1.smove.succ = 0;
			      actual = 50;
			      flag = 0;
			      autho_ht1 = 1;
			      spower1 += 10;
			      refresh_powerbar ();
			    }
			}
		      else if (!check_kstatus1 (last_status1))
			p1.smove.succ = 0;
		    }
		  else
		    p1.smove.succ = 0;
		}
	      else
		p1.smove.succ = 0;
	    }
	  else
	    p1.smove.succ = 0;
	  // NOW FOR SPECIALS
	  if (p1.firebx.flg && p1.firebx.sflg)
	    {
	      if (check_kstatus1 (p1.firebx.seq[0]) && !p1.firebx.succ)
		{
		  p1.firebx.succ = 1;
		  p1.firebx.nb = 0;
		}
	      else if (p1.firebx.succ)
		{
		  if ((last_status1 == p1.firebx.seq[(int)p1.firebx.nb])
		      || (last_status1 == p1.firebx.seq[p1.firebx.nb + 1]))
		    {
		      if (check_kstatus1 (p1.firebx.seq[p1.firebx.nb + 1]))
			{
			  p1.firebx.nb++;
			  p1.firebx.succ = p1.firebx.nb + 1;
			  if (p1.firebx.succ == p1.firebx.end)
			    {
			      if (p1.firebx.spec)
				{
				  p1.firebx.sflg = 0;
				  spower1 = 0;
				  refresh_powerbar ();
				}
			      //aqui
			      playwav (DEF_hadokenx1);
			      p1.firebx.succ = 0;
			      actual = 31;
			      flag = 0;
			      autho_ht1 = 1;
			      spower1 += 10;
			      refresh_powerbar ();
			    }
			}
		      else if (!check_kstatus1 (last_status1))
			p1.firebx.succ = 0;
		    }
		  else
		    p1.firebx.succ = 0;
		}
	      else
		p1.firebx.succ = 0;
	    }
	  else
	    p1.firebx.succ = 0;
	  if (p1.rushx.flg && p1.rushx.sflg)
	    {
	      if (check_kstatus1 (p1.rushx.seq[0]) && !p1.rushx.succ)
		{
		  p1.rushx.succ = 1;
		  p1.rushx.nb = 0;
		}
	      else if (p1.rushx.succ)
		{
		  if ((last_status1 == p1.rushx.seq[(int)p1.rushx.nb])
		      || (last_status1 == p1.rushx.seq[p1.rushx.nb + 1]))
		    {
		      if (check_kstatus1 (p1.rushx.seq[p1.rushx.nb + 1]))
			{
			  p1.rushx.nb++;
			  p1.rushx.succ = p1.rushx.nb + 1;
			  if (p1.rushx.succ == p1.rushx.end)
			    {
			      if (p1.rushx.spec)
				{
				  p1.rushx.sflg = 0;
				  spower1 = 0;
				  refresh_powerbar ();
				}
			      //aqui
			      playwav (DEF_copterx1);
			      p1.rushx.succ = 0;
			      actual = 41;
			      flag = 0;
			      autho_ht1 = 1;
			      spower1 += 10;
			      refresh_powerbar ();
			    }
			}
		      else if (!check_kstatus1 (last_status1))
			p1.rushx.succ = 0;
		    }
		  else
		    p1.rushx.succ = 0;
		}
	      else
		p1.rushx.succ = 0;
	    }
	  else
	    p1.rushx.succ = 0;
	  if (p1.smovex.flg && p1.smovex.sflg)
	    {
	      if (check_kstatus1 (p1.smovex.seq[0]) && !p1.smovex.succ)
		{
		  p1.smovex.succ = 1;
		  p1.smovex.nb = 0;
		}
	      else if (p1.smovex.succ)
		{
		  if ((last_status1 == p1.smovex.seq[(int)p1.smovex.nb])
		      || (last_status1 == p1.smovex.seq[p1.smovex.nb + 1]))
		    {
		      if (check_kstatus1 (p1.smovex.seq[p1.smovex.nb + 1]))
			{
			  p1.smovex.nb++;
			  p1.smovex.succ = p1.smovex.nb + 1;
			  if (p1.smovex.succ == p1.smovex.end)
			    {
			      if (p1.smovex.spec)
				{
				  p1.smovex.sflg = 0;
				  spower1 = 0;
				  refresh_powerbar ();
				}
			      //aqui
			      playwav (DEF_dragonx1);
			      p1.smovex.succ = 0;
			      actual = 51;
			      flag = 0;
			      autho_ht1 = 1;
			      spower1 += 10;
			      refresh_powerbar ();
			    }
			}
		      else if (!check_kstatus1 (last_status1))
			p1.smovex.succ = 0;
		    }
		  else
		    p1.smovex.succ = 0;
		}
	      else
		p1.smovex.succ = 0;
	    }
	  else
	    p1.smovex.succ = 0;
	  /*
	   * static updating  
	   */
	  if (actual == 0)
	    {
	      flag++;
	      if (flag > ((p1.statik.spd * p1.statik.nbf) - 1))
		flag = 0;
	      if (spr1y < (sol1 - 1))
		spr1y = spr1y + 3;
	    }
	  if (!P1_WK && !P1_SK && !P1_SP && !P1_WP && (limp1 < 1))
	    limp1++;
	}
      // UPDATE AIR-KICK ***********/
      if ((actual > 2) && (actual < 6) && (apch == 0))
	{
	  poll_joystick ();
	  if (ak && limak)
	    {
	      ak++;
	      if (ak > 25)
		{
		  ak = 0;
		  limak = 0;
		}
	    }
	  else if (limak)
	    if (P1_WK || P1_SK)
	      {
		ak = 1;
		Paf (3);
		fy = 1;
	      }
	}
      else
	ak = 0;
      // UPDATE AIR-PUNCH ***********/
      if ((actual > 2) && (actual < 6) && (ak == 0))
	{
	  poll_joystick ();
	  if (apch && limak)
	    {
	      apch++;
	      if (apch > 25)
		{
		  apch = 0;
		  limak = 0;
		}
	    }
	  else if (limak)
	    if (P1_SP || P1_WP)
	      {
		apch = 1;
		Paf (1);
		fy = 1;
	      }
	}
      else
	apch = 0;
		/***** END OF CHECK/UPDATE PLAYER 1 ******************************/
		/***** CHECK PLAYER 2 INPUT ************************/
#if GAME_DEBUG >= 2
      TRON ("CHECK PLAYER 2 INPUT");
#endif
      if (((actual2 < 3) || (actual2 > 100)) && gmode)	// if actual
	// position
	// not prioritary
	{
	  i = rand () % 100;
	  g = 0;
	  h = rand () % 100;
	  com_block = 0;
	  com_gblock = 0;
	  if ((actual2 == 2)
	      && ((spr1x + p1.limoffx) > (spr2x + p2.width - p2.limdefx)))
	    {
	      if (spr2y < (sol2 - 1))
		spr2y = spr2y + 3;
	      if ((actual == 6) || (actual == 7) || (actual == 61)
		  || (actual == 71) || ak || apch
		  || (actual == 10) || (actual == 13)
		  || (actual == 40) || ((fball1 == 1)
					&& ((fb1x + p1.fball.w) > spr2x)))
		{
		  if (h < defense)
		    {
		      actual2 = 18;
		      flag2 = 0;
		      com_gblock = 1;
		    }
		  else
		    com_gblock = 0;
		}
	    }
	  if ((actual2 == 2) && (limp2 > 0)
	      && ((spr2x - (p2.gpunch.offx - p2.width)) <
		  (spr1x + p1.limdefx)) && (com_gblock == 0))
	    {
	      if (spr2y < (sol2 - 1))
		spr2y = spr2y + 3;
	      if (h < offense)
		{
		  if (i < 50)
		    {
		      Paf (4);
		      actual2 = 10;
		      flag2 = 0;
		      fx = 1;
		    }
		  else
		    {
		      Paf (1);
		      actual2 = 13;
		      flag2 = 0;
		      fx = 1;
		    }
		}
	    }
	  else if (actual2 == 2)
	    actual2 = 0;
	  else if ((actual2 < 2) || (actual2 > 100))
	    {
	      if ((spr1x + p1.limoffx) > (spr2x + p2.width - p2.limdefx))	// if reachable
		{
		  if ((actual == 6) || (actual == 7) || (actual == 61)
		      || (actual == 71) || ak || apch
		      || (actual == 40) || ((fball1 == 1)
					    && ((fb1x + p1.fball.w) > spr2x)))
		    {
		      if (h < defense)
			{
			  com_gblock = 0;
			  com_block = 1;
			}
		      else
			com_block = 0;
		    }
		  else if ((actual == 10) || (actual == 13))
		    {
		      if (h < defense)
			{
			  com_gblock = 1;
			}
		      else
			com_gblock = 0;
		    }
		}
	      h = rand () % 100;
	      g = 0;
	      if ((((spr2x - (p2.spunch.offx - p2.width))
		    < (spr1x + p1.limdefx))
		   && ((spr2x - (p2.wpunch.offx -
				 p2.width)) < (spr1x + p1.limdefx)))
		  && (((spr2x - (p2.skick.offx - p2.width)) <
		       (spr1x + p1.limdefx))
		      && ((spr2x - (p2.wkick.offx -
				    p2.width)) < (spr1x + p1.limdefx))))
		{
		  // if offensive
		  if (h < offense)
		    {
		      g = 0;
		      if ((limp2 > 0) && !p2.fireb.succ && !p2.rush.succ
			  && !p2.smove.succ && !p2.firebx.succ
			  && !p2.rushx.succ && !p2.smovex.succ)
			{
			  if (i < 5)
			    {
			      actual2 = 0;
			      ai_static = 100;
			      g = 1;
			    }
			  else if ((i < 20)
				   &&
				   ((spr2x
				     - (p2.spunch.offx - p2.width)) <
				    (spr1x + p1.limdefx)))
			    {
			      Paf (2);
			      actual2 = 6;
			      flag2 = 0;
			      fx = 1;
			    }
			  else if ((i < 35)
				   &&
				   ((spr2x
				     - (p2.wpunch.offx - p2.width)) <
				    (spr1x + p1.limdefx)))
			    {
			      Paf (1);
			      actual2 = 61;
			      flag2 = 0;
			      fx = 1;
			    }
			  else if ((i < 60)
				   &&
				   ((spr2x
				     - (p2.skick.offx - p2.width)) <
				    (spr1x + p1.limdefx)))
			    {
			      Paf (4);
			      actual2 = 7;
			      flag2 = 0;
			      fx = 1;
			    }
			  else if ((i < 80)
				   &&
				   ((spr2x
				     - (p2.wkick.offx - p2.width)) <
				    (spr1x + p1.limdefx)))
			    {
			      Paf (3);
			      actual2 = 71;
			      flag2 = 0;
			      fx = 1;
			    }
			  else if (i < 95)
			    {
			      if ((i < 85) && p2.smovex.flg && p2.smovex.sflg)
				{
				  if (p2.smovex.spec)
				    {
				      p2.smovex.sflg = 0;
				      spower2 = 0;
				      refresh_powerbar ();
				    }
				  //aqui
				  playwav (DEF_dragonx2);
				  p2.smovex.succ = 0;
				  actual2 = 51;
				  flag2 = 0;
				  autho_ht2 = 1;
				  spower2 += 10;
				  refresh_powerbar ();
				}
			      else if ((p2.smove.flg)
				       &&
				       ((spr2x
					 - (p2.smove.offx -
					    p2.width)) <
					(spr1x + p1.limdefx)))
				{
				  //aqui
				  playwav (DEF_dragon2);
				  p2.smove.succ = 0;
				  actual2 = 50;
				  flag2 = 0;
				  autho_ht2 = 1;
				  spower2 += 10;
				  refresh_powerbar ();
				}
			      else
				{
				  g = 1;
				  ai_walkb = 100;
				}
			    }
			  else
			    {
			      g = 1;
			      ai_walkb = 100;
			    }
			}
		      else if (actual == 111)
			{
			  g = 0;
			  actual2 = 2;
			  // ai_walkb = 100 ;
			}
		    }
		  else		// if not offensive
		    {
		      i = rand () % 100;
		      if (i < 97)
			{
			  ai_walkb = 100;
			  actual2 = 111;
			  g = 1;
			}
		      else
			actual2 = 5;
		    }
		}
	      else
		g = 1;
	      i = rand () % 100;
	      h = rand () % 100;
	      // if not reachable
	      if (g)
		{
		  if (ai_static)	// if walking
		    {
		      ai_static--;
		      actual2 = 0;
		    }
		  else if (ai_walkf)	// if walking
		    {
		      ai_walkf--;
		      if (zoomwidth > 320)
			zoomwidth--;
		      if ((spr2x + tol2) > (spr1x + tol1))
			spr2x -= 2;
		      else if (spr1x > (limg + cx))
			{
			  spr1x--;
			  spr2x--;
			}
		      else if (cx > 1)
			{
			  cx--;
			  spr1x--;
			  spr2x--;
			}
		      flag2++;
		      if (flag2 > ((p2.walk.spd * p2.walk.nbf) - 1))
			flag2 = 0;
		      actual2 = 1;
		    }
		  else if (ai_walkb)
		    {
		      ai_walkb--;
		      if (spr2x < (limd + cx))
			spr2x += 2;
		      else if (spr1x > (limg + cx))
			for (i = 0; i < s; i++)
			  {
			    if (cx < (bkgdlim + 320 - zoomwidth))
			      {
				cx++;
				spr2x++;
			      }
			  }
		      else
			for (i = 0; i < s; i++)
			  {
			    if (cx < (bkgdlim + 320 - zoomwidth))
			      if (zoomwidth < ((200 + zoomsup) * 1.6))
				{
				  zoomwidth++;
				  spr2x++;
				}
			  }
		      flag2--;
		      if (flag2 < 0)
			flag2 = ((p2.walk.spd * p2.walk.nbf) - 1);
		      actual2 = 111;
		    }
		  else		// if not walking
		  if (h < offense)	// if offensive
		    {
		      if (i < 25)
			{
			  actual2 = 1;
			  ai_walkf = 100;
			}
		      else if (i < 30)
			actual2 = 4;
		      else if ((limp2 > 0) && !p2.fireb.succ && !p2.rush.succ)
			{
			  if (i < 65)
			    {
			      if ((i < 45) && p2.firebx.flg && p2.firebx.sflg)
				{
				  if (p2.firebx.spec)
				    {
				      p2.firebx.sflg = 0;
				      spower2 = 0;
				      refresh_powerbar ();
				    }
				  //aqui
				  playwav (DEF_hadokenx2);
				  p2.firebx.succ = 0;
				  actual2 = 31;
				  flag2 = 0;
				  autho_ht2 = 1;
				  spower2 += 10;
				  refresh_powerbar ();
				}
			      else if (p2.fireb.flg)
				{
				  //aqui
				  playwav (DEF_hadoken2);
				  p2.fireb.succ = 0;
				  actual2 = 30;
				  flag2 = 0;
				  autho_ht2 = 1;
				  spower2 += 10;
				  refresh_powerbar ();
				}
			      else
				actual2 = 4;
			    }
			  else
			    {
			      if ((i < 80) && p2.rushx.flg && p2.rushx.sflg)
				{
				  if (p2.rushx.spec)
				    {
				      p2.rushx.sflg = 0;
				      spower2 = 0;
				      refresh_powerbar ();
				    }
				  //aqui
				  playwav (DEF_copterx2);
				  p2.rushx.succ = 0;
				  actual2 = 41;
				  flag2 = 0;
				  autho_ht2 = 1;
				  spower2 += 10;
				  refresh_powerbar ();
				}
			      else if (p2.rush.flg)
				{
				  //aqui
				  playwav (DEF_copter2);
				  p2.rush.succ = 0;
				  actual2 = 40;
				  flag2 = 0;
				  autho_ht2 = 1;
				  spower2 += 10;
				  refresh_powerbar ();
				}
			      else
				actual2 = 4;
			    }
			}
		    }
		  else
		    // if not offensive
		    {
		      i = rand () % 100;
		      if (i < 50)
			{
			  actual2 = 0;
			  ai_static = 100;
			}
		      else if (i < 65)
			{
			  actual2 = 5;
			}
		      else if (i < 80)
			{
			  actual2 = 3;
			}
		      else if (i < 85)
			{
			  actual2 = 1;
			  ai_walkf = 100;
			}
		      else
			{
			  actual2 = 111;
			  ai_walkb = 100;
			}
		    }
		}
	    }
	}
      /*2 Players game */
#if GAME_DEBUG >= 2
      TRON ("2 Players game");
#endif
      if (((actual2 < 3) || (actual2 > 100)) && !gmode)
	{
	  actual2 = 0;
	  // poll_joystick() ;
	  if ((P2_RT) && (P2_UP))
	    {
	      actual2 = 5;
	    }
	  else if ((P2_LT) && (P2_UP))
	    {
	      actual2 = 4;
	    }
	  else if (P2_UP)
	    {
	      actual2 = 3;
	    }
	  else if (P2_DN)
	    {
	      actual2 = 2;
	    }
	  else if (P2_LT)
	    {
	      if (zoomwidth > 320)
		zoomwidth--;
	      if ((spr2x + tol2) > (spr1x + tol1))
		spr2x -= 2;
	      else if (spr1x > (limg + cx))
		{
		  spr1x--;
		  spr2x--;
		}
	      else if (cx > 1)
		{
		  cx--;
		  spr1x--;
		  spr2x--;
		}
	      flag2++;
	      if (flag2 > ((p2.walk.spd * p2.walk.nbf) - 1))
		flag2 = 0;
	      actual2 = 1;
	    }
	  else if (P2_RT)
	    {
	      if (spr2x < (limd + cx))
		spr2x += 2;
	      else if (spr1x > (limg + cx))
		for (i = 0; i < s; i++)
		  {
		    if (cx < (bkgdlim + 320 - zoomwidth))
		      {
			cx++;
			spr2x++;
		      }
		  }
	      else
		for (i = 0; i < s; i++)
		  {
		    if (cx < (bkgdlim + 320 - zoomwidth))
		      if (zoomwidth < ((200 + zoomsup) * 1.6))
			{
			  zoomwidth++;
			  spr2x++;
			}
		  }
	      flag2--;
	      if (flag2 < 0)
		flag2 = ((p2.walk.spd * p2.walk.nbf) - 1);
	      actual2 = 111;
	    }
	  if (((actual2 < 2) || (actual2 > 100)) && (limp2 > 0)
	      && !p2.fireb.succ && !p2.rush.succ && !p2.smove.succ
	      && !p2.firebx.succ && !p2.rushx.succ && !p2.smovex.succ)
	    {
	      if (P2_SP)
		{
		  Paf (2);
		  actual2 = 6;
		  flag2 = 0;
		  fx = 1;
		}
	      else if (P2_WP)
		{
		  Paf (1);
		  actual2 = 61;
		  flag2 = 0;
		  fx = 1;
		}
	      else if (P2_SK)
		{
		  Paf (4);
		  actual2 = 7;
		  flag2 = 0;
		  fx = 1;
		}
	      else if (P2_WK)
		{
		  Paf (3);
		  actual2 = 71;
		  flag2 = 0;
		  fx = 1;
		}
	    }
	  if ((actual2 == 2) && (limp2 > 0))
	    {
	      if ((P2_WK) || (P2_SK))
		{
		  Paf (4);
		  actual2 = 10;
		  flag2 = 0;
		  fx = 1;
		}
	      else if ((P2_WP) || (P2_SP))
		{
		  Paf (1);
		  actual2 = 13;
		  flag2 = 0;
		  fx = 1;
		}
	    }
	  if (p2.fireb.flg)
	    {
	      if (check_kstatus2 (p2.fireb.seq[0]) && !p2.fireb.succ)
		{
		  p2.fireb.succ = 1;
		  p2.fireb.nb = 0;
		}
	      else if (p2.fireb.succ)
		{
		  if ((last_status2 == p2.fireb.seq[(int)p2.fireb.nb])
		      || (last_status2 == p2.fireb.seq[p2.fireb.nb + 1]))
		    {
		      if (check_kstatus2 (p2.fireb.seq[p2.fireb.nb + 1]))
			{
			  p2.fireb.nb++;
			  p2.fireb.succ = p2.fireb.nb + 1;
			  if (p2.fireb.succ == p2.fireb.end)
			    {
			      //aqui
			      playwav (DEF_hadoken2);
			      p2.fireb.succ = 0;
			      actual2 = 30;
			      flag2 = 0;
			      autho_ht2 = 1;
			      spower2 += 10;
			      refresh_powerbar ();
			    }
			}
		      else if (!check_kstatus2 (last_status2))
			p2.fireb.succ = 0;
		    }
		  else
		    p2.fireb.succ = 0;
		}
	      else
		p2.fireb.succ = 0;
	    }
	  else
	    p2.fireb.succ = 0;
	  if (p2.rush.flg)
	    {
	      if (check_kstatus2 (p2.rush.seq[0]) && !p2.rush.succ)
		{
		  p2.rush.succ = 1;
		  p2.rush.nb = 0;
		}
	      else if (p2.rush.succ)
		{
		  if ((last_status2 == p2.rush.seq[(int)p2.rush.nb])
		      || (last_status2 == p2.rush.seq[p2.rush.nb + 1]))
		    {
		      if (check_kstatus2 (p2.rush.seq[p2.rush.nb + 1]))
			{
			  p2.rush.nb++;
			  p2.rush.succ = p2.rush.nb + 1;
			  if (p2.rush.succ == p2.rush.end)
			    {
			      //aqui
			      playwav (DEF_copter2);
			      p2.rush.succ = 0;
			      actual2 = 40;
			      flag2 = 0;
			      autho_ht2 = 1;
			      spower2 += 10;
			      refresh_powerbar ();
			    }
			}
		      else if (!check_kstatus2 (last_status2))
			p2.rush.succ = 0;
		    }
		  else
		    p2.rush.succ = 0;
		}
	      else
		p2.rush.succ = 0;
	    }
	  else
	    p2.rush.succ = 0;
	  if (p2.smove.flg)
	    {
	      if (check_kstatus2 (p2.smove.seq[0]) && !p2.smove.succ)
		{
		  p2.smove.succ = 1;
		  p2.smove.nb = 0;
		}
	      else if (p2.smove.succ)
		{
		  if ((last_status2 == p2.smove.seq[(int)p2.smove.nb])
		      || (last_status2 == p2.smove.seq[p2.smove.nb + 1]))
		    {
		      if (check_kstatus2 (p2.smove.seq[p2.smove.nb + 1]))
			{
			  p2.smove.nb++;
			  p2.smove.succ = p2.smove.nb + 1;
			  if (p2.smove.succ == p2.smove.end)
			    {
			      //aqui
			      playwav (DEF_dragon2);
			      p2.smove.succ = 0;
			      actual2 = 50;
			      flag2 = 0;
			      autho_ht2 = 1;
			      spower2 += 10;
			      refresh_powerbar ();
			    }
			}
		      else if (!check_kstatus2 (last_status2))
			p2.smove.succ = 0;
		    }
		  else
		    p2.smove.succ = 0;
		}
	      else
		p2.smove.succ = 0;
	    }
	  else
	    p2.smove.succ = 0;
	  // NOW FOR SPECIALS
	  if (p2.firebx.flg && p2.firebx.sflg)
	    {
	      if (check_kstatus2 (p2.firebx.seq[0]) && !p2.firebx.succ)
		{
		  p2.firebx.succ = 1;
		  p2.firebx.nb = 0;
		}
	      else if (p2.firebx.succ)
		{
		  if ((last_status2 == p2.firebx.seq[(int)p2.firebx.nb])
		      || (last_status2 == p2.firebx.seq[p2.firebx.nb + 1]))
		    {
		      if (check_kstatus2 (p2.firebx.seq[p2.firebx.nb + 1]))
			{
			  p2.firebx.nb++;
			  p2.firebx.succ = p2.firebx.nb + 1;
			  if (p2.firebx.succ == p2.firebx.end)
			    {
			      if (p2.firebx.spec)
				{
				  p2.firebx.sflg = 0;
				  spower2 = 0;
				  refresh_powerbar ();
				}
			      //aqui
			      playwav (DEF_hadokenx2);
			      p2.firebx.succ = 0;
			      actual2 = 31;
			      flag2 = 0;
			      autho_ht2 = 1;
			      spower2 += 10;
			      refresh_powerbar ();
			    }
			}
		      else if (!check_kstatus2 (last_status2))
			p2.firebx.succ = 0;
		    }
		  else
		    p2.firebx.succ = 0;
		}
	      else
		p2.firebx.succ = 0;
	    }
	  else
	    p2.firebx.succ = 0;
	  if (p2.rushx.flg && p2.rushx.sflg)
	    {
	      if (check_kstatus2 (p2.rushx.seq[0]) && !p2.rushx.succ)
		{
		  p2.rushx.succ = 1;
		  p2.rushx.nb = 0;
		}
	      else if (p2.rushx.succ)
		{
		  if ((last_status2 == p2.rushx.seq[(int)p2.rushx.nb])
		      || (last_status2 == p2.rushx.seq[p2.rushx.nb + 1]))
		    {
		      if (check_kstatus2 (p2.rushx.seq[p2.rushx.nb + 1]))
			{
			  p2.rushx.nb++;
			  p2.rushx.succ = p2.rushx.nb + 1;
			  if (p2.rushx.succ == p2.rushx.end)
			    {
			      if (p2.rushx.spec)
				{
				  p2.rushx.sflg = 0;
				  spower2 = 0;
				  refresh_powerbar ();
				}
			      //aqui
			      playwav (DEF_copterx2);
			      p2.rushx.succ = 0;
			      actual2 = 41;
			      flag2 = 0;
			      autho_ht2 = 1;
			      spower2 += 10;
			      refresh_powerbar ();
			    }
			}
		      else if (!check_kstatus2 (last_status2))
			p2.rushx.succ = 0;
		    }
		  else
		    p2.rushx.succ = 0;
		}
	      else
		p2.rushx.succ = 0;
	    }
	  else
	    p2.rushx.succ = 0;
	  if (p2.smovex.flg && p2.smovex.sflg)
	    {
	      if (check_kstatus2 (p2.smovex.seq[0]) && !p2.smovex.succ)
		{
		  p2.smovex.succ = 1;
		  p2.smovex.nb = 0;
		}
	      else if (p2.smovex.succ)
		{
		  if ((last_status2 == p2.smovex.seq[(int)p2.smovex.nb])
		      || (last_status2 == p2.smovex.seq[p2.smovex.nb + 1]))
		    {
		      if (check_kstatus2 (p2.smovex.seq[p2.smovex.nb + 1]))
			{
			  p2.smovex.nb++;
			  p2.smovex.succ = p2.smovex.nb + 1;
			  if (p2.smovex.succ == p2.smovex.end)
			    {
			      if (p2.smovex.spec)
				{
				  p2.smovex.sflg = 0;
				  spower2 = 0;
				  refresh_powerbar ();
				}
			      //aqui
			      playwav (DEF_dragonx2);
			      p2.smovex.succ = 0;
			      actual2 = 51;
			      flag2 = 0;
			      autho_ht2 = 1;
			      spower2 += 10;
			      refresh_powerbar ();
			    }
			}
		      else if (!check_kstatus2 (last_status2))
			p2.smovex.succ = 0;
		    }
		  else
		    p2.smovex.succ = 0;
		}
	      else
		p2.smovex.succ = 0;
	    }
	  else
	    p2.smovex.succ = 0;
	}

      /*
       * static updating  
       */
      if (actual2 == 0)
	{
	  flag2++;
	  if (flag2 > ((p2.statik.spd * p2.statik.nbf) - 1))
	    flag2 = 0;
	  if (spr2y < (sol2 - 1))
	    spr2y = spr2y + 3;
	}
      if (!(P2_WP) && !(P2_WK) && !(P2_SP) && !(P2_SK) && (limp2 < 1))
	limp2++;
      // UPDATE AIR-KICK ***********/
      if ((actual2 > 2) && (actual2 < 6) && (apch2 == 0))
	{
	  if (ak2 && limak2)
	    {
	      ak2++;
	      if (ak2 > 25)
		{
		  ak2 = 0;
		  limak2 = 0;
		}
	    }
	  else
	    {
	      if (limak2)
		if ((P2_WK) || (P2_SK))
		  {
		    ak2 = 1;
		    Paf (3);
		    fx = 1;
		  }
	    }
	}
      else
	ak2 = 0;
      // UPDATE AIR-PUNCH ***********/
      if ((actual2 > 2) && (actual2 < 6) && (ak2 == 0))
	{
	  if (apch2 && limak2)
	    {
	      apch2++;
	      if (apch2 > 25)
		{
		  apch2 = 0;
		  limak2 = 0;
		}
	    }
	  else
	    {
	      if (limak2)
		if ((P2_SP) || (P2_WP))
		  {
		    apch2 = 1;
		    Paf (1);
		    fx = 1;
		  }
	    }
	}
      else
	apch2 = 0;
      // AIR AI !!!
      if ((actual2 > 2) && (actual2 < 6) && gmode && (ak2 == 0)
	  && (apch2 == 0))
	{
	  i = rand () % 100;
	  h = rand () % 100;
	  com_block = 0;
	  com_gblock = 0;
	  if ((spr1x + p1.akick.w) > spr2x)
	    {
	      if ((actual == 6) || (actual == 7) || (actual == 61)
		  || (actual == 71) || ak || apch || (actual == 40))
		{
		  if (h < defense)
		    {
		      com_gblock = 0;
		      com_block = 1;
		    }
		  else
		    com_block = 0;
		}
	    }
	  if (((spr2x - (p2.akick.w - p2.width)) < (spr1x + p1.width)))
	    {
	      // if offensive
	      if (i < offense)
		{
		  ak2 = 1;
		}
	      else
		ak2 = 0;
	    }
	}

		/**** CHECK PLAYER 1 ATTACKS*******/
#if GAME_DEBUG >= 2
      TRON ("CHECK PLAYER 1 ATTACKS");
#endif
      if ((actual == 6) && (flag == ((p1.spunch.hit - 1) * p1.spunch.spd)))	// PUNCH
	{
	  if (((spr1x + p1.spunch.offx) >
	       (spr2x + p2.width - p2.limdefx))
	      && ((spr1y + p1.spunch.offy + 10) > (spr2y + p2.limdefy)))
	    {
	      poll_joystick ();
	      damag2 = p1.spunch.dmg;
	      p1.limoffx = spr1x + p1.spunch.offx;
	      p1.limoffy = spr1y + p1.spunch.offy;
	      if (spr2y < (sol2 - 5))
		{
		  actual2 = 20;
		  hitfram1 = (p1.spunch.hit - 1) * p1.spunch.spd;
		  p1.wimpact_ct = 10;
		}
	      else if (P2_DN || com_gblock)
		{
		  if (P2_RT || com_gblock)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 18;
		    }
		  else
		    {
		      actual2 = 19;
		      hitfram1 = (p1.spunch.hit - 1) * p1.spunch.spd;
		      p1.wimpact_ct = 10;
		    }
		}
	      else
		{
		  if (P2_RT || com_block)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 8;
		    }
		  else
		    {
		      actual2 = 9;
		      hitfram1 = (p1.spunch.hit - 1) * p1.spunch.spd;
		      p1.wimpact_ct = 10;
		    }
		}
	      flag2 = 0;
	    }
	}
      else if ((actual == 61) && (flag == ((p1.wpunch.hit - 1) * p1.wpunch.spd)))	// WPUNCH
	{
	  if (((spr1x + p1.wpunch.offx) >
	       (spr2x + p2.width - p2.limdefx))
	      && ((spr1y + p1.wpunch.offy + 10) > (spr2y + p2.limdefy)))
	    {
	      damag2 = p1.wpunch.dmg;
	      p1.limoffx = spr1x + p1.wpunch.offx;
	      p1.limoffy = spr1y + p1.wpunch.offy;
	      if (P2_DN || com_gblock)
		{
		  if (P2_RT || com_gblock)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 18;
		    }
		  else
		    {
		      p1.wimpact_ct = 10;
		      actual2 = 19;
		      hitfram1 = (p1.wpunch.hit - 1) * p1.wpunch.spd;
		    }
		}
	      else
		{
		  if (P2_RT || com_block)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 8;
		    }
		  else
		    {
		      p1.wimpact_ct = 10;
		      actual2 = 9;
		      hitfram1 = (p1.wpunch.hit - 1) * p1.wpunch.spd;
		    }
		}
	      flag2 = 0;
	    }
	}
      else if ((actual == 7) && (flag == ((p1.skick.hit - 1) * p1.skick.spd)))	// KICK
	{
	  if (((spr1x + p1.skick.offx) > (spr2x + p2.width - p2.limdefx))
	      && ((spr1y + p1.skick.offy + 10) > (spr2y + p2.limdefy)))
	    {
	      damag2 = p1.skick.dmg;
	      p1.limoffx = spr1x + p1.skick.offx;
	      p1.limoffy = spr1y + p1.skick.offy;
	      if (spr2y < (sol2 - 5))
		{
		  p1.wimpact_ct = 10;
		  actual2 = 20;
		  hitfram1 = (p1.skick.hit - 1) * p1.skick.spd;
		}
	      else if (P2_DN || com_gblock)
		{
		  if (P2_RT || com_gblock)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 18;
		    }
		  else
		    {
		      p1.wimpact_ct = 10;
		      actual2 = 19;
		      hitfram1 = (p1.skick.hit - 1) * p1.skick.spd;
		    }
		}
	      else
		{
		  if (P2_RT || com_block)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 8;
		    }
		  else
		    {
		      p1.wimpact_ct = 10;
		      actual2 = 9;
		      hitfram1 = (p1.skick.hit - 1) * p1.skick.spd;
		    }
		}
	      flag2 = 0;
	    }
	}
      else if ((actual == 71) && (flag == ((p1.wkick.hit - 1) * p1.wkick.spd)))	// WKICK
	{
	  if (((spr1x + p1.wkick.offx) > (spr2x + p2.width - p2.limdefx))
	      && ((spr1y + p1.wkick.offy + 10) > (spr2y + p2.limdefy)))
	    {
	      damag2 = p1.wkick.dmg;
	      p1.limoffx = spr1x + p1.wkick.offx;
	      p1.limoffy = spr1y + p1.wkick.offy;
	      if (P2_DN || com_gblock)
		{
		  if (P2_RT || com_gblock)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 18;
		    }
		  else
		    {
		      p1.wimpact_ct = 10;
		      actual2 = 19;
		      hitfram1 = (p1.wkick.hit - 1) * p1.wkick.spd;
		    }
		}
	      else
		{
		  if (P2_RT || com_block)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 8;
		    }
		  else
		    {
		      p1.wimpact_ct = 10;
		      actual2 = 9;
		      hitfram1 = (p1.wkick.hit - 1) * p1.wkick.spd;
		    }
		}
	      flag2 = 0;
	    }
	}
      else if ((actual == 10) && (flag == ((p1.gkick.hit - 1) * p1.gkick.spd)) && ((actual2 > 5) || (actual2 < 3)))	// GKICK
	{
	  if (((spr1x + p1.gkick.offx) > (spr2x + p2.width - p2.limdefx))
	      && ((spr1y + p1.gkick.offy + 10) > (spr2y + p2.limdefy)))
	    {
	      damag2 = p1.gkick.dmg;
	      p1.limoffx = spr1x + p1.gkick.offx;
	      p1.limoffy = spr1y + p1.gkick.offy;
	      if (P2_DN || com_gblock)
		{
		  if (P2_RT || com_gblock)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 18;
		    }
		  else
		    {
		      p1.wimpact_ct = 10;
		      actual2 = 19;
		      hitfram1 = (p1.gkick.hit - 1) * p1.gkick.spd;
		    }
		}
	      else if (flag < (p1.gkick.hit * p1.gkick.spd))
		{
		  p1.wimpact_ct = 10;
		  actual2 = 20;
		  hitfram1 = (p1.gkick.hit - 1) * p1.gkick.spd;
		}
	      flag2 = 0;
	    }
	}
      else if ((actual == 13) && (flag == ((p1.gpunch.hit - 1) * p1.gpunch.spd)) && ((actual2 > 5) || (actual2 < 3)))	// GPUNCH
	{
	  if (((spr1x + p1.gpunch.offx) >
	       (spr2x + p2.width - p2.limdefx))
	      && ((spr1y + p1.gpunch.offy + 10) > (spr2y + p2.limdefy)))
	    {
	      damag2 = p1.gpunch.dmg;
	      p1.limoffx = spr1x + p1.gpunch.offx;
	      p1.limoffy = spr1y + p1.gpunch.offy;
	      if (P2_DN || com_gblock)
		{
		  if (P2_RT || com_gblock)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 18;
		    }
		  else
		    {
		      p1.wimpact_ct = 10;
		      actual2 = 19;
		      hitfram1 = (p1.gpunch.hit - 1) * p1.gpunch.spd;
		    }
		}
	      else if (flag < (p1.gpunch.hit * p1.gpunch.spd))
		{
		  p1.wimpact_ct = 10;
		  actual2 = 9;
		  hitfram1 = (p1.gpunch.hit - 1) * p1.gpunch.spd;
		}
	      flag2 = 0;
	    }
	}
      else if (ak == (p1.akick.hit - 1) * p1.akick.spd)	// AIR-KICK
	{
	  if (((spr1x + p1.akick.offx) > (spr2x + p2.width - p2.limdefx))
	      && ((spr1y + p1.akick.offy + 10) > (spr2y + p2.limdefy)))
	    {
	      damag2 = p1.akick.dmg;
	      p1.limoffx = spr1x + p1.akick.offx;
	      p1.limoffy = spr1y + p1.akick.offy;
	      if (spr2y < (sol2 - 5))
		{
		  p1.wimpact_ct = 10;
		  actual2 = 20;
		  hitfram1 = (p1.akick.hit - 1) * p1.akick.spd;
		}
	      else if (P2_DN || com_gblock)
		{
		  if (P2_RT || com_gblock)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 18;
		    }
		  else if (spr1y > (sol1 - 20))
		    {
		      p1.wimpact_ct = 10;
		      actual2 = 19;
		      hitfram1 = (p1.akick.hit - 1) * p1.akick.spd;
		    }
		}
	      else
		{
		  if (P2_RT || com_block)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 8;
		    }
		  else
		    {
		      p1.wimpact_ct = 10;
		      actual2 = 9;	/*
					 * hitfram1 =
					 * (p1.akick.hit-1)*p1.akick.spd ; 
					 */
		    }
		}
	      flag2 = 0;
	    }
	}
      else if (apch == (p1.apunch.hit - 1) * p1.apunch.spd)	// AIR-PUNCH
	{
	  if (((spr1x + p1.apunch.offx) >
	       (spr2x + p2.width - p2.limdefx))
	      && ((spr1y + p1.apunch.offy + 10) > (spr2y + p2.limdefy)))
	    {
	      damag2 = p1.apunch.dmg;
	      p1.limoffx = spr1x + p1.apunch.offx;
	      p1.limoffy = spr1y + p1.apunch.offy;
	      if (spr2y < (sol2 - 5))
		{
		  p1.wimpact_ct = 10;
		  actual2 = 20;
		  hitfram1 = (p1.apunch.hit - 1) * p1.apunch.spd;
		}
	      else if (P2_DN || com_gblock)
		{
		  if (P2_RT || com_gblock)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 18;
		    }
		  else if (spr1y > (sol1 - 20))
		    {
		      p1.wimpact_ct = 10;
		      actual2 = 19;
		      hitfram1 = (p1.apunch.hit - 1) * p1.apunch.spd;
		    }
		}
	      else
		{
		  if (P2_RT || com_block)
		    {
		      p1.bimpact_ct = 10;
		      actual2 = 8;
		    }
		  else
		    {
		      p1.wimpact_ct = 10;
		      actual2 = 9;	/*
					 * hitfram1 =
					 * (p1.apunch.hit-1)*p1.apunch.spd ; 
					 */
		    }
		}
	      flag2 = 0;
	    }
	}

		/**** CHECK PLAYER 2 ATTACKS *******/
#if GAME_DEBUG >= 2
      TRON ("CHECK PLAYER 2 ATTACKS");
#endif
      if ((actual2 == 6) && (flag2 == ((p2.spunch.hit - 1) * p2.spunch.spd)))	// PUNCH
	{
	  if (((spr2x - (p2.spunch.offx - p2.width)) <
	       (spr1x + p1.limdefx))
	      && ((spr2y + p2.spunch.offy + 10) > (spr1y + p1.limdefy)))
	    {
	      damag1 = p2.spunch.dmg;
	      p2.limoffx = spr2x - (p2.spunch.offx - p2.width);
	      p2.limoffy = spr2y + p2.spunch.offy;
	      if (spr1y < (sol1 - 5))
		{
		  p2.wimpact_ct = 10;
		  actual = 20;
		  hitfram2 = (p2.spunch.hit - 1) * p2.spunch.spd;
		}
	      else if (P1_DN)
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 18;
		    }
		  else
		    {
		      p2.wimpact_ct = 10;
		      actual = 19;
		      hitfram2 = (p2.spunch.hit - 1) * p2.spunch.spd;
		    }
		}
	      else
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 8;
		    }
		  else
		    {
		      p2.wimpact_ct = 10;
		      actual = 9;
		      hitfram2 = (p2.spunch.hit - 1) * p2.spunch.spd;
		    }
		}
	      flag = 0;
	    }
	}
      else if ((actual2 == 61) && (flag2 == ((p2.wpunch.hit - 1) * p2.wpunch.spd)))	// WPUNCH
	{
	  if (((spr2x - (p2.wpunch.offx - p2.width)) <
	       (spr1x + p1.limdefx))
	      && ((spr2y + p2.wpunch.offy + 10) > (spr1y + p1.limdefy)))
	    {
	      damag1 = p2.wpunch.dmg;
	      p2.limoffx = spr2x - (p2.wpunch.offx - p2.width);
	      p2.limoffy = spr2y + p2.wpunch.offy;
	      if (P1_DN)
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 18;
		    }
		  else
		    {
		      p2.wimpact_ct = 10;
		      actual = 19;
		      hitfram2 = (p2.wpunch.hit - 1) * p2.wpunch.spd;
		    }
		}
	      else
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 8;
		    }
		  else
		    {
		      p2.wimpact_ct = 10;
		      actual = 9;
		      hitfram2 = (p2.wpunch.hit - 1) * p2.wpunch.spd;
		    }
		}
	      flag = 0;
	    }
	}
      else if ((actual2 == 7) && (flag2 == ((p2.skick.hit - 1) * p2.skick.spd)))	// KICK
	{
	  if (((spr2x - (p2.skick.offx - p2.width)) <
	       (spr1x + p1.limdefx))
	      && ((spr2y + p2.skick.offy + 10) > (spr1y + p1.limdefy)))
	    {
	      damag1 = p2.skick.dmg;
	      p2.limoffx = spr2x - (p2.skick.offx - p2.width);
	      p2.limoffy = spr2y + p2.skick.offy;
	      if (spr1y < (sol1 - 5))
		{
		  p2.wimpact_ct = 10;
		  actual = 20;
		  hitfram2 = (p2.skick.hit - 1) * p2.skick.spd;
		}
	      else if (P1_DN)
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 18;
		    }
		  else
		    {
		      p2.wimpact_ct = 10;
		      actual = 19;
		      hitfram2 = (p2.skick.hit - 1) * p2.skick.spd;
		    }
		}
	      else
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 8;
		    }
		  else
		    {
		      p2.wimpact_ct = 10;
		      actual = 9;
		      hitfram2 = (p2.skick.hit - 1) * p2.skick.spd;
		    }
		}
	      flag = 0;
	    }
	}
      else if ((actual2 == 71) && (flag2 == ((p2.wkick.hit - 1) * p2.wkick.spd)))	// WKICK
	{
	  if (((spr2x - (p2.wkick.offx - p2.width)) <
	       (spr1x + p1.limdefx))
	      && ((spr2y + p2.wkick.offy + 10) > (spr1y + p1.limdefy)))
	    {
	      damag1 = p2.wkick.dmg;
	      p2.limoffx = spr2x - (p2.wkick.offx - p2.width);
	      p2.limoffy = spr2y + p2.wkick.offy;
	      if (P1_DN)
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 18;
		    }
		  else
		    {
		      p2.wimpact_ct = 10;
		      actual = 19;
		      hitfram2 = (p2.wkick.hit - 1) * p2.wkick.spd;
		    }
		}
	      else
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 8;
		    }
		  else
		    {
		      p2.wimpact_ct = 10;
		      actual = 9;
		      hitfram2 = (p2.wkick.hit - 1) * p2.wkick.spd;
		    }
		}
	      flag = 0;
	    }
	}
      else if ((actual2 == 10) && (flag2 == ((p2.gkick.hit - 1) * p2.gkick.spd)) && ((actual > 5) || (actual < 3)))	// GKICK
	{
	  if (((spr2x - (p2.gkick.offx - p2.width)) <
	       (spr1x + p1.limdefx))
	      && ((spr2y + p2.gkick.offy + 10) > (spr1y + p1.limdefy)))
	    {
	      damag1 = p2.gkick.dmg;
	      p2.limoffx = spr2x - (p2.gkick.offx - p2.width);
	      p2.limoffy = spr2y + p2.gkick.offy;
	      if (P1_DN)
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 18;
		    }
		  else
		    {
		      p2.wimpact_ct = 10;
		      actual = 19;
		      hitfram2 = (p2.gkick.hit - 1) * p2.gkick.spd;
		    }
		}
	      else if (flag2 < (p2.gkick.hit * p2.gkick.spd))
		{
		  p2.wimpact_ct = 10;
		  actual = 20;
		  hitfram2 = (p2.gkick.hit - 1) * p2.gkick.spd;
		}
	      flag = 0;
	    }
	}
      else if ((actual2 == 13) && (flag2 == ((p2.gpunch.hit - 1) * p2.gpunch.spd)) && ((actual > 5) || (actual < 3)))	// GPUNCH
	{
	  if (((spr2x - (p2.gpunch.offx - p2.width)) <
	       (spr1x + p1.limdefx))
	      && ((spr2y + p2.gpunch.offy + 10) > (spr1y + p1.limdefy)))
	    {
	      damag1 = p2.gpunch.dmg;
	      p2.limoffx = spr2x - (p2.gpunch.offx - p2.width);
	      p2.limoffy = spr2y + p2.gpunch.offy;
	      if (P1_DN)
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 18;
		    }
		  else
		    {
		      p2.wimpact_ct = 10;
		      actual = 19;
		      hitfram2 = (p2.gpunch.hit - 1) * p2.gpunch.spd;
		    }
		}
	      else if (flag2 < (p2.gpunch.hit * p2.gpunch.spd))
		{
		  p2.wimpact_ct = 10;
		  actual = 9;
		  hitfram2 = (p2.gpunch.hit - 1) * p2.gpunch.spd;
		}
	      flag = 0;
	    }
	}
      else if (ak2 == (p2.akick.hit - 1) * p2.akick.spd)	// AIR-KICK
	{
	  if (((spr2x - (p2.akick.offx - p2.width)) <
	       (spr1x + p1.limdefx))
	      && ((spr2y + p2.akick.offy + 10) > (spr1y + p1.limdefy)))
	    {
	      damag1 = p2.akick.dmg;
	      p2.limoffx = spr2x - (p2.akick.offx - p2.width);
	      p2.limoffy = spr2y + p2.akick.offy;
	      if (spr1y < (sol1 - 5))
		{
		  p2.wimpact_ct = 10;
		  actual = 20;
		  hitfram2 = (p2.akick.hit - 1) * p2.akick.spd;
		}
	      else if (P1_DN)
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 18;
		    }
		  else if (spr2y > (sol2 - 20))
		    {
		      p2.wimpact_ct = 10;
		      actual = 19;
		      hitfram2 = (p2.akick.hit - 1) * p2.akick.spd;
		    }
		}
	      else
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 8;
		    }
		  else
		    {
		      p2.wimpact_ct = 10;
		      actual = 9;	/*
					 * hitfram2 =
					 * (p2.akick.hit-1)*p2.akick.spd ; 
					 */
		    }
		}
	      flag = 0;
	    }
	}
      else if (apch2 == (p2.apunch.hit - 1) * p2.apunch.spd)	// AIR-PUNCH
	{
	  if (((spr2x - (p2.apunch.offx - p2.width)) <
	       (spr1x + p1.limdefx))
	      && ((spr2y + p2.apunch.offy + 10) > (spr1y + p1.limdefy)))
	    {
	      damag1 = p2.apunch.dmg;
	      p2.limoffx = spr2x - (p2.apunch.offx - p2.width);
	      p2.limoffy = spr2y + p2.apunch.offy;
	      if (spr1y < (sol1 - 5))
		{
		  p2.wimpact_ct = 10;
		  actual = 20;
		  hitfram2 = (p2.apunch.hit - 1) * p2.apunch.spd;
		}
	      else if (P1_DN)
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 18;
		    }
		  else if (spr2y > (sol2 - 20))
		    {
		      p2.wimpact_ct = 10;
		      actual = 19;
		      hitfram2 = (p2.apunch.hit - 1) * p2.apunch.spd;
		    }
		}
	      else
		{
		  if (P1_LT)
		    {
		      p2.bimpact_ct = 10;
		      actual = 8;
		    }
		  else
		    {
		      p2.wimpact_ct = 10;
		      actual = 9;	/*
					 * hitfram2 =
					 * (p2.apunch.hit-1)*p2.apunch.spd ; 
					 */
		    }
		}
	      flag = 0;
	    }
	}


		/***** CHECK AND UPDATE LIFEBARS **********/
      if (spower1 >= 60)
	{
	  spower1 = 60;
	  p1.firebx.sflg = p1.rushx.sflg = p1.smovex.sflg = 1;
	  refresh_powerbar ();
	}
      if (spower2 >= 60)
	{
	  spower2 = 60;
	  p2.firebx.sflg = p2.rushx.sflg = p2.smovex.sflg = 1;
	  refresh_powerbar ();
	}

#if GAME_DEBUG >= 2
      TRON ("CHECK AND UPDATE LIFEBARS");
#endif

#if TIMEOVER > 0

      refresh_timer ();
#endif
      refresh_vflags ();
      displays ();

		/**** END OF ROUND START OUTWIN/PERFECT/TIMEOVER****/
#if GAME_DEBUG >= 2
      TRON ("END OF ROUND START OUTWIN/PERFECT/TIMEOVER");
#endif
      if ((life1 <= 0) || (life2 <= 0))
	{
	  nbpar++;
	  fx = fy = 1;
	  flag = flag2 = 1;
	  cy = (200 + zoomsup) - ZMRATIO;
	  if (cy < 1)
	    cy = 1;
	  if (retrace_count > 2)
	    retrace_count = 0;
	  poll_joystick ();
	  if (life1 <= 0)
	    {
	      vic2++;
	      playwav (female1 ? DEF_female : DEF_argh);

	      for (flag = 1; flag < 160; ++flag)
		{
		  if (animated)
		    bkgd_counter++;
		  retrace_count--;

		  if (retrace_count > 0)
		    ;
		  else
		    {
		      blit_Bkgd ();
#if GAME_DEBUG > 0
		      show_fpc ();
#endif

		      if (spr2y < sol2)
			GJump2 (5, spr2x, spr2y);
		      else
			GStatic2 ((flag2 / p2.statik.spd), spr2x, spr2y);

		      GDead ((flag / 20), spr1x, spr1y);

		      if (foreground)
			masked_blit (Frgd, zoomscreen,
				     0, 0, 0, 0, (bkgdlim + 320),
				     (200 + zoomsup));

		      cy = (200 + zoomsup) - ZMRATIO;
		      if (cy < 1)
			cy = 1;
		      if ((cy < zoomsup) && (cy > 0))
			stretch_blit (zoomscreen, virtscreen,
				      cx, cy, zoomwidth, ZMRATIO, 0, 0,
				      320, 200);
		      else
			blit (zoomscreen, virtscreen, cx, zoomsup, 0, 0,
			      320, 200);
		      // vsync() ;
		      blit_KOF91 ();
#if GAME_DEBUG > 0
		      show_fpc ();
#endif
		      while (!retrace_count)
			;
		    }

		  flag2++;
		  if (flag2 > ((p2.statik.spd * p2.statik.nbf) - 1))
		    flag2 = 0;
		  if (spr2y < sol2)
		    spr2y++;

		  if (spr1y < sol1)
		    spr1y++;

		  if (spr1x > (limg + cx))
		    spr1x--;
		  else if (spr2x < (limd + cx))
		    for (i = 0; i < s; i++)
		      {
			if (cx > 1)
			  {
			    cx--;
			    spr1x--;
			  }
		      }
		  else
		    for (i = 0; i < s; i++)
		      {
			if (cx > 1)
			  if (zoomwidth < ((200 + zoomsup) * 1.6))
			    {
			      cx--;
			      spr1x--;
			      zoomwidth++;
			    }
		      }
		}
#if TIMEOVER > 0
	      if (GET_t_clock () <= 1)
		{
		  masked_blit (Carton_TO, virtscreen, 0, 0,
			       (320 - Carton_TO->w) / 2,
			       (200 - Carton_TO->h) / 2, Carton_TO->w,
			       Carton_TO->h);
		  blit_KOF91 ();
#if GAME_DEBUG > 0
		  show_fpc ();
#endif
		}
	      else
		{
#endif
		  masked_blit (Carton_KO, virtscreen, 0, 0,
			       (320 - Carton_KO->w) / 2,
			       (200 - Carton_KO->h) / 2, Carton_KO->w,
			       Carton_KO->h);
		  blit_KOF91 ();
#if GAME_DEBUG > 0
		  show_fpc ();
#endif

#if TIMEOVER > 0

		}
	      if (GET_t_clock () <= 1)
		playwav (DEF_to);
	      else
#endif

		playwav (DEF_ko);
	      load_win2 ();
	      rest (30);
	      retrace_count = 0;
	      for (flag = 1; flag < (p2.outwin.nbf * p2.outwin.spd); ++flag)
		{
		  if (animated)
		    bkgd_counter++;
		  retrace_count--;
		  if (retrace_count > 0)
		    ;
		  else
		    {
		      blit_Bkgd ();
#if GAME_DEBUG > 0
		      show_fpc ();
#endif
		      GDead (7, spr1x, spr1y);
		      GKick2 ((flag / p2.outwin.spd),
			      (spr2x - (p2.outwin.w - p2.width)), spr2y, 3);
		      if (foreground)
			masked_blit (Frgd, zoomscreen, 0, 0, 0, 0,
				     (bkgdlim + 320), (200 + zoomsup));
		      cy = (200 + zoomsup) - ZMRATIO;
		      if (cy < 1)
			cy = 1;
		      if ((cy < zoomsup) && (cy > 0))
			stretch_blit (zoomscreen, virtscreen,
				      cx, cy, zoomwidth, ZMRATIO, 0, 0,
				      320, 200);
		      else
			blit (zoomscreen, virtscreen, cx, zoomsup, 0, 0,
			      320, 200);
		      blit_KOF91 ();
#if GAME_DEBUG > 0
		      show_fpc ();
#endif
		      while (!retrace_count)
			;
		    }
		}
	      if ((cy < zoomsup) && (cy > 0))
		stretch_blit (zoomscreen, virtscreen,
			      cx, cy, zoomwidth, ZMRATIO, 0, 0, 320, 200);
	      else
		blit (zoomscreen, virtscreen, cx, zoomsup, 0, 0, 320, 200);
	      angle = (Carton_WN->w / Carton_WN->h);
	      for (i = Carton_WN->w; i > (Carton_WN->w / 3); i = i - 20)
		{

#if PERFECT == 1
		  if (life2 >= 500)
		    {
		      draw_sprite (virtscreen, B_prfct,
				   160 - B_prfct->w / 2, 100 - B_prfct->h);
		      if (i == Carton_WN->w)
			playwav (DEF_prfct);
		    }
#endif
		  blit_KOF91 ();
		  TRON ("slowdown_end_round-1");
		  //rest(slowdown_end_roud);
		  adjusts (F_SLOWDOWN_END_ROUND);
#if GAME_DEBUG > 0
		  show_fpc ();
#endif
		  if ((cy < zoomsup) && (cy > 0))
		    stretch_blit (zoomscreen, virtscreen,
				  cx, cy, zoomwidth, ZMRATIO, 0, 0, 320, 200);
		  else
		    blit (zoomscreen, virtscreen, cx, zoomsup, 0, 0,
			  320, 200);
		  stretch_sprite (virtscreen, Carton_WN, 290 - i, 30, i,
				  i / angle);
		}
	    }
	  else
	    {
	      vic1++;
	      playwav (female2 ? DEF_female : DEF_argh);

	      for (flag2 = 1; flag2 < 160; ++flag2)
		{
		  if (animated)
		    bkgd_counter++;
		  retrace_count--;
		  if (retrace_count > 0)
		    ;
		  else
		    {
		      blit_Bkgd ();
#if GAME_DEBUG > 0
		      show_fpc ();
#endif
		      if (spr1y < sol1)
			GJump (5, spr1x, spr1y);
		      else
			GStatic ((flag / p1.statik.spd), spr1x, spr1y);
		      GDead2 ((flag2 / 20),
			      (spr2x - (p2.ko.w - p2.width)), spr2y);
		      if (foreground)
			masked_blit (Frgd, zoomscreen, 0, 0, 0, 0,
				     (bkgdlim + 320), (200 + zoomsup));
		      cy = (200 + zoomsup) - ZMRATIO;
		      if (cy < 1)
			cy = 1;
		      if ((cy < zoomsup) && (cy > 0))
			stretch_blit (zoomscreen, virtscreen,
				      cx, cy, zoomwidth, ZMRATIO, 0, 0,
				      320, 200);
		      else
			blit (zoomscreen, virtscreen, cx, zoomsup, 0, 0,
			      320, 200);
		      vsync ();
		      blit_KOF91 ();
#if GAME_DEBUG > 0
		      show_fpc ();
#endif
		      while (!retrace_count)
			;
		    }

		  flag++;
		  if (flag > ((p1.statik.spd * p1.statik.nbf) - 1))
		    flag = 0;
		  if (spr2y < sol2)
		    spr2y++;
		  if (spr1y < sol1)
		    spr1y++;
		  if (spr2x < (limd + cx))
		    spr2x++;
		  else if (spr1x > (limg + cx))
		    for (i = 0; i < s; i++)
		      {
			if (cx < (bkgdlim + 320 - zoomwidth))
			  {
			    cx++;
			    spr2x++;
			  }
		      }
		  else
		    for (i = 0; i < s; i++)
		      {
			if (cx < (bkgdlim + 320 - zoomwidth))
			  if (zoomwidth < ((200 + zoomsup) * 1.6))
			    {
			      zoomwidth++;
			      spr2x++;
			    }
		      }
		}

#if TIMEOVER > 0
	      if (GET_t_clock () <= 1)
		{
		  masked_blit (Carton_TO, virtscreen, 0, 0,
			       (320 - Carton_TO->w) / 2,
			       (200 - Carton_TO->h) / 2, Carton_TO->w,
			       Carton_TO->h);
		  blit_KOF91 ();
#if GAME_DEBUG > 0
		  show_fpc ();
#endif
		}
	      else
		{
#endif
		  masked_blit (Carton_KO, virtscreen, 0, 0,
			       (320 - Carton_KO->w) / 2,
			       (200 - Carton_KO->h) / 2, Carton_KO->w,
			       Carton_KO->h);
		  blit_KOF91 ();
#if GAME_DEBUG > 0
		  show_fpc ();
#endif
#if TIMEOVER > 0

		}
	      if (GET_t_clock () <= 1)
		playwav (DEF_to);
	      else
#endif

		playwav (DEF_ko);
	      load_win1 ();
	      rest (30);
	      retrace_count = 0;

	      for (flag = 1; flag < (p1.outwin.nbf * p1.outwin.spd); ++flag)
		{
		  if (animated)
		    bkgd_counter++;
		  retrace_count--;
		  if (retrace_count > 0)
		    ;
		  else
		    {
		      blit_Bkgd ();
#if GAME_DEBUG > 0
		      show_fpc ();
#endif
		      GDead2 (7, (spr2x - (p2.ko.w - p2.width)), spr2y);
		      GKick ((flag / p1.outwin.spd), spr1x, spr1y, 3);
		      if (foreground)
			masked_blit (Frgd, zoomscreen,
				     0, 0, 0, 0, (bkgdlim + 320),
				     (200 + zoomsup));
		      cy = (200 + zoomsup) - ZMRATIO;
		      if (cy < 1)
			cy = 1;
		      if ((cy < zoomsup) && (cy > 0))
			stretch_blit (zoomscreen, virtscreen,
				      cx, cy, zoomwidth, ZMRATIO, 0, 0,
				      320, 200);
		      else
			blit (zoomscreen, virtscreen, cx, zoomsup, 0, 0,
			      320, 200);
		      blit_KOF91 ();
#if GAME_DEBUG > 0
		      show_fpc ();
#endif
		      while (!retrace_count)
			;
		    }
		}
	      angle = (Carton_WN->w / Carton_WN->h);
	      for (i = Carton_WN->w; i > (Carton_WN->w / 3); i = i - 20)
		{
#if PERFECT == 1
		  if (life1 >= 500)
		    {
		      draw_sprite (virtscreen, B_prfct,
				   160 - B_prfct->w / 2, 100 - B_prfct->h);
		      if (i == Carton_WN->w)
			playwav (DEF_prfct);
		    }
#endif
		  blit_KOF91 ();
		  TRON ("slowdown_end_round-2");
		  //rest(slowdown_end_round);
		  adjusts (F_SLOWDOWN_END_ROUND);
#if GAME_DEBUG > 0
		  show_fpc ();
#endif
		  if ((cy < zoomsup) && (cy > 0))
		    stretch_blit (zoomscreen, virtscreen,
				  cx, cy, zoomwidth, ZMRATIO, 0, 0, 320, 200);
		  else
		    blit (zoomscreen, virtscreen, cx, zoomsup, 0, 0,
			  320, 200);

		  stretch_sprite (virtscreen, Carton_WN, 30, 30, i,
				  i / angle);

		}
	    }
	  blit_KOF91 ();
#if GAME_DEBUG > 0
	  show_fpc ();
#endif
	  //rest( 1500 );
	  poll_joystick ();
	  clear_keybuf ();

	  // Se o player 1 ganhar
	  if (vic1 > 1)
	    {
	      blit_KOF91 ();
#if GAME_DEBUG > 0
	      show_fpc ();
#endif
	      //rest( 500 );
	      rectfill (virtscreen, 0, 0, 320, 200, 0);

	      // aqui carrega pegamos a imagem de fundo
	      // e carregamos e mostramos :)  

	      sprintf (file2, "sprites" DIR_BAR "npattern.pcx");
	      tface1 = xload_pcx (file2, Pal);

	      draw_sprite (virtscreen, tface1, 0, 0);


	      // aqui desenhamos a selbig do personagem vencedor
	      stretch_sprite (virtscreen, Carton_WN,
			      (320 - 120) - 20, 10, 120, 20);
	      //draw_sprite( virtscreen, Carton_WN,
	      //                       ( 320 - Carton_WN->w ) / 2, 10 );

	      // Claudemir, vou escrever um texto na tela  :)

	      sprintf (passeur, "sprites" DIR_BAR "%s" DIR_BAR "char.ini",
		       charname[sel1]);

	      text_centre_shadow (virtscreen, font,
				  ini_get_str (passeur, charname[sel1],
					       "say_win", "..."),
				  (int) virtscreen->w / 2, 180, makecol (255,
									 255,
									 255));


	      if (Face1 != NULL)
		// aqui desenhamos a selbig de vencedor
		draw_sprite (virtscreen, Face1, (320 - Face1->w) / 2,
			     Carton_WN->h);

	      blit_KOF91 ();
#if GAME_DEBUG > 0
	      show_fpc ();
#endif
	      poll_joystick ();
	      clear_keybuf ();

	      while (!(P2_SP) && !(P2_SK)
		     && !(P2_WP) && !(P2_WK)
		     && !(key[KEY_ENTER]) && !P1_WK && !P1_SK
		     && !P1_WP && !P1_SP)
		{
		  poll_joystick ();
		}		/* whie */

	      // destroy_midi (music) ;
	      if (animated)
		close_fli ();
	      destroy_bitmap (zoomscreen);

	      if (gmode == 2)
		{
		  defeated[(int)last_sel2] = 1;
		  story = 1;
		}

	      /*
	       * goto StartMenu; 
	       */
	      return 1;

	    }
	  else if (vic2 > 1)	// if player 2 win ...
	    {
	      blit_KOF91 ();
#if GAME_DEBUG > 0
	      show_fpc ();
#endif
	      //rest( 500 );


	      /*              if (gmode) Carton_WN = load_bitmap("sprites" DIR_BAR "lose.pcx", 0);
	       */
	      if (gmode)
		Carton_WN = xload_pcx ("sprites" DIR_BAR "lose.pcx", 0);

	      rectfill (virtscreen, 0, 0, 320, 200, 0);
	      draw_sprite (virtscreen, Carton_WN,
			   (320 - Carton_WN->w) / 2, 20);
	      if (Face2 != NULL)
		{
		  if (sel1 == sel2)
		    {
		      set_trans_blender (0, 255, 0, 0);
		      draw_lit_sprite (virtscreen, Face2,
				       (320 - Face1->w) / 2,
				       30 + Carton_WN->h, 100);

		    }
		  else
		    {
		      draw_sprite (virtscreen, Face2,
				   (320 - Face1->w) / 2, 30 + Carton_WN->h);
		    }
		}
	      sprintf (passeur, "sprites" DIR_BAR "%s"
		       DIR_BAR "char.ini", charname[sel2]);

	      text_centre_shadow (virtscreen, font,
				  ini_get_str (passeur, charname[sel2],
					       "say_win", "..."),
				  virtscreen->w / 2, 180, makecol (255, 255,
								   255));

	      blit_KOF91 ();
#if GAME_DEBUG > 0
	      show_fpc ();
#endif
	      poll_joystick ();
	      clear_keybuf ();
	      while (!(P2_SP) && !(P2_SK)
		     && !(P2_WP) && !(P2_WK)
		     && !(key[KEY_ENTER]) && !P1_WK && !P1_SK
		     && !P1_WP && !P1_SP)
		{
		  poll_joystick ();
		}
	      // destroy_midi (music) ;
	      close_fli ();
	      destroy_bitmap (zoomscreen);
	      if (gmode == 2)
		story = 1;
	      /*
	       * goto StartMenu; 
	       */
	      return 1;
	    }

	  /*END ROUND AFTER THE ANIMATIONS/PERFECT/OUTWIN/TIMEOVER */
#if GAME_DEBUG >= 2
	  TRON ("END ROUND AFTER THE ANIMATIONS/PERFECT/OUTWIN/TIMEOVER");
#endif
	  fight_round++;
	  life1 = life2 = 500;
	  refresh_lifebar ();
	  actual = actual2 = 0;
	  zoomwidth = 320;

#if TEMP > 0

	  inverted = 0;
#endif

#if TIMEOVER > 0

	  PUT_t_clock (xget_config_int ("timer", "t_clock", 0));
	  refresh_timer ();
#endif

	  p1.wimpact_ct = p2.wimpact_ct = 0;
	  p1.simpact_ct = p2.simpact_ct = 0;
	  p1.bimpact_ct = p2.bimpact_ct = 0;
	  flag = flag2 = 1;
	  fball1 = fball2 = 0;
	  p1.fireb.succ = p2.fireb.succ = 0;
	  p1.rush.succ = p2.rush.succ = 0;
	  rush_stop1 = rush_stop2 = 0;
	  smove_stop1 = smove_stop2 = 0;
	  spr1x = (cx + 75 - (p1.width / 2));
		  spr1y = sol1; //ESTRANHO 
	  spr2x = (cx + 250 - (p2.width / 2));
		  spr2y = sol2; //ESTANHO2
	  deb = 1;
#if TEMP > 0

	  inverted = 0;
#endif

	  initdisplay ();
	  introround (fight_round);

	}			/* ((life1 <= 0) || (life2 <= 0)) { DETECT ENDGAME } */

		/****** END OF THE 'REAL GAME' LOOP ******/
#if GAME_DEBUG >= 2
      TRON ("END OF THE REAL GAME LOOP");
#endif
    }				/* while (ok)  THE BIG BATLE LOOP */

  return 0;
}

/**********************************************/

void K_pause_game (void)
{
  BITMAP *saved_screen;
  BITMAP *pause_screen;
  BITMAP *pause_sprite;
  BITMAP *pause_bord;
  int i;
  TRON ("PAUSE GAME");
  if (key[KEY_F1])
    skp_flag = !skp_flag;

#if SCREENSHOT > 0
  if (key[KEY_F2])
    screenshot ();
#endif

  if (key[KEY_F12])
    console_monitor ();

  if (key[KEY_ESC])
    {

      displays ();

      x = 1;
      y = 1;
      pause_sprite = create_bitmap (200, 60);
      rectfill (pause_sprite, 0, 0, 200, 60, makecol (0, 50, 0));
      pause_bord = xload_pcx ("sprites" DIR_BAR "monitor.pcx", Pal);

      saved_screen = create_bitmap (320, 200);
      pause_screen = create_bitmap (320, 200);

      stretch_blit (screen, saved_screen, 0, 0, screen_width, screen_height,
		    0, 0, 320, 200);

      stretch_blit (screen, pause_screen, 0, 0, screen_width, screen_height,
		    0, 0, 320, 200);

      /* FX - open Window */
      for (i = 0; i <= 60; i = i + pause_bord->w)
	{
	  draw_sprite (pause_screen, saved_screen, 0, 0);
	  blit (pause_bord, pause_screen, 0, 0, i, 50,
		pause_bord->w, pause_sprite->h);

	  blit (pause_bord, pause_screen, 0, 0, 320 - i - pause_bord->w, 50,
		pause_bord->w, pause_sprite->h);

	  stretch_blit (pause_screen, screen, 0, 0, 320, 200,
			0, 0, screen_width, screen_height);
	  rest (10);
	}

      while (x)
	{

	  draw_sprite (pause_screen, saved_screen, 0, 0);
	  set_trans_blender (255, 255, 255, 170);
	  draw_trans_sprite (pause_screen, pause_sprite, 60, 50);

	  blit (pause_bord, pause_screen, 0, 0, 60, 50,
		pause_bord->w, pause_sprite->h);

	  blit (pause_bord, pause_screen, 0, 0,
		pause_sprite->w - pause_bord->w + 60, 50,
		pause_bord->w, pause_sprite->h);

	  text_centre_shadow (pause_screen, font, "CONTINUE",
			      160, 65, (y ? sel_player1 : alt_color));

	  text_centre_shadow (pause_screen, font, "QUIT TO MAIN MENU",
			      160, 80, (y ? alt_color : sel_player1));

	  stretch_blit (pause_screen, screen, 0, 0, 320, 200,
			0, 0, screen_width, screen_height);


	  if ((P2_SP) || (P2_SK)
	      || (P2_WP) || (P2_WK)
	      || (key[KEY_ENTER]) || P1_WK || P1_SK || P1_WP || P1_SP)
	    {

	      if (!y)
		ok = 0;
	      x = 0;

	    }			/* if ((P2_SP... */

	  if (P2_UP || P2_DN
	      || P2_LT || P2_RT || P1_UP || P1_DN || P1_RT || P1_LT)
	    {

	      Bing ();
	      rest (90);
	      y = !y;
	      clear_keybuf ();
	    }			/* if (P2_UP... */

	}			/* while (x) */

      /* FX - close Window */
      for (i = 60; i >= 0; i = i - pause_bord->w)
	{
	  draw_sprite (pause_screen, saved_screen, 0, 0);
	  blit (pause_bord, pause_screen, 0, 0, i, 50,
		pause_bord->w, pause_sprite->h);

	  blit (pause_bord, pause_screen, 0, 0, 320 - i - pause_bord->w, 50,
		pause_bord->w, pause_sprite->h);

	  stretch_blit (pause_screen, screen, 0, 0, 320, 200,
			0, 0, screen_width, screen_height);
	  rest (10);
	}

      draw_sprite (pause_screen, saved_screen, 0, 0);
      stretch_blit (pause_screen, screen, 0, 0, 320, 200,
		    0, 0, screen_width, screen_height);

      clear_keybuf ();
      destroy_bitmap (pause_sprite);
      destroy_bitmap (saved_screen);
      destroy_bitmap (pause_screen);
      destroy_bitmap (pause_bord);
    }

}				/* void K_pause_game(void) */

// EOF
