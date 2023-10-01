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
#ifndef _MOVES_H
#define _MOVES_H

void GStatic ( int gueu, int x , int y ) ;
void GSingle ( int gueu, int x , int y ) ;
void GWalk ( int gueu, int x , int y ) ;
void GPunch ( int gueu, int x , int y ) ;
void GWPunch ( int gueu, int x , int y ) ;
void GKick ( int gueu, int x , int y , int z ) ;
void GWKick ( int gueu, int x , int y ) ;
void GAKick ( int gueu, int x , int y ) ;
void GAPunch ( int gueu, int x , int y ) ;
void GJump ( int gueu, int x , int y ) ;
void GGKick ( int gueu, int x , int y ) ;
void GGPunch ( int gueu, int x , int y ) ;
void GFireB ( int gueu, int x , int y ) ;
void GRush ( int gueu, int x , int y ) ;
void GSMove ( int gueu, int x , int y ) ;
void GFireBX ( int gueu, int x , int y ) ;
void GRushX ( int gueu, int x , int y ) ;
void GSMoveX ( int gueu, int x , int y ) ;
void GHurt ( int gueu, int x , int y ) ;
void GGHurt ( int gueu, int x , int y ) ;
void GKO ( int gueu, int x , int y ) ;
void GDead ( int gueu, int x , int y ) ;

void GStatic2 ( int gueu, int x , int y ) ;
void GSingle2 ( int gueu, int x , int y ) ;
void GWalk2 ( int gueu, int x , int y ) ;
void GPunch2 ( int gueu, int x , int y ) ;
void GWPunch2 ( int gueu, int x , int y ) ;
void GKick2 ( int gueu, int x , int y , int z ) ;
void GWKick2 ( int gueu, int x , int y ) ;
void GAKick2 ( int gueu, int x , int y ) ;
void GAPunch2 ( int gueu, int x , int y ) ;
void GJump2 ( int gueu, int x , int y ) ;
void GGKick2 ( int gueu, int x , int y ) ;
void GGPunch2 ( int gueu, int x , int y ) ;
void GFireB2 ( int gueu, int x , int y ) ;
void GRush2 ( int gueu, int x , int y ) ;
void GSMove2 ( int gueu, int x , int y ) ;
void GFireBX2 ( int gueu, int x , int y ) ;
void GRushX2 ( int gueu, int x , int y ) ;
void GSMoveX2 ( int gueu, int x , int y ) ;
void GHurt2 ( int gueu, int x , int y ) ;
void GGHurt2 ( int gueu, int x , int y ) ;
void GKO2 ( int gueu, int x , int y, int z ) ;
void GDead2 ( int gueu, int x , int y ) ;

void GFBall ( int gueu, int x , int y ) ;
void GFBall2 ( int gueu, int x , int y ) ;

void GFBallX ( int gueu, int x , int y ) ;
void GFBallX2 ( int gueu, int x , int y ) ;

void draw_player( BITMAP *poser, int x, int y ) ;

#endif

// EOF

