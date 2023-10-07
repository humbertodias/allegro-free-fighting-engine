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

#ifndef _INIFILES_H
#define _INIFILES_H

extern char sret[];

int listfiles(char xret[1000][30], const char *directory, const char *sufixname);

typedef struct _L_SECTION {

    char variable[40];
    char value[255];

} _L_SECTION;

typedef struct _INI_section {

    char namesection[40];
    int len;
    _L_SECTION reg[500];

} INI_section;

typedef struct _INI_inifile {

    char namefile[40];
    int len;
    char section[40][500];

} INI_inifile;

/* Protos ......*/
char *ini_get_str(char *sname_file, char *section, char *svalue,
		  char *sdefault);

int ini_get_int(char *sname_file, char *section, char *svalue,
		int idefault);

INI_section ini_get_vars(char *sname_file, char *section);

INI_inifile ini_get_section(char *sname_file);

void getcolorstr(int *r,int *g,int *b, char color[20]);

#endif
/*******************************************************/

// EOF
