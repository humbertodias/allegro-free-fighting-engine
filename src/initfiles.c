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
#include "stdio.h"

#include "config.h"
#include <string.h>
#include "allegro.h"

#include "macros.h"
#include "initfiles.h"

#if SO==LINUX
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#if SO==DOS
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

/***************************************************************************/
int listfiles(char xret[1000][30], const char *directory, const char *sufixname)
{
    DIR *dirp;
    struct dirent *dp;
    int i;

    dirp = opendir( directory );
    i=0;
    while ( (dp = readdir(dirp)) ) {
        if( strlen( sufixname )==0 ){
	if( !strchr(dp->d_name,'.') ){	
	    sprintf( xret[i++],"%s",dp->d_name);
	  }
	}
	else
	{
	  if( strcmp(dp->d_name,".")
             && strcmp(dp->d_name,"..")
             && strstr(dp->d_name, sufixname)
            )
          {
  	    sprintf( xret[i++],"%s",dp->d_name);
          }
	}
    }
    closedir(dirp);

//    sprintf( log_mess,"dir:%s sufixname:%s",directory,sufixname);
//    TRON( log_mess );
//
//    for(f=0;f<i;f++){
//      sprintf( log_mess, "xret[%d]=%s",f,xret[f]);
//      TRON( log_mess );
//    }
    
    return i;
}
/************************************************************************/
char *ini_get_str(char *sname_file, char *section, char *svalue,
		  char *sdefault)
{
    FILE *inifile;
    char sread[255], ch='\0';
    char term1[255], term2[255], term3[255];
    char t_section[255];
    size_t l_section, i, xx;

    l_section = strlen(section) + 2;

    sprintf(t_section, "[%s]", section);

    memset(sread, '\0', 255);

    if ((inifile = fopen(sname_file, "r")) == NULL) {
	printf("IO Error.\n");
	exit(1);
    }

    if (strlen(section)) {
	while (!feof(inifile))	// INI search section
	{
	    for (i = 0; i < l_section - 1; i++)
		sread[i] = sread[i + 1];

	    ch = getc(inifile);
	    sread[l_section - 1] = ch;
	    if (!strcmp(sread, t_section)) {
		ch = getc(inifile);	// Jump the ']' 
		break;
	    }
	}
    } else {
	ch = getc(inifile);
    }

    if (!feof(inifile)) {
	while (!feof(inifile) && ch != '[')	// loop while not end of section 
	{

	    //skip remarks in the .ini file ....
	    if (ch == '#')
		while (!feof(inifile) && (ch = getc(inifile) != 10));

	    // read lines of section
	    if (ch!=10 && ch!=13 && !isspace(ch)) {

		xx = 0;
		memset(term1, '\0', 255);
		while (!feof(inifile) && ('=' != ch) && (ch!=10 && ch!=13)) {

		    if (isalnum(ch) || ch == '_')
			term1[xx++] = ch;

		    if (ch == 32) {
			while (!feof(inifile) && ch == 32)
			    ch = getc(inifile);

			if (ch != '=') {
			    term1[xx++] = 32;
			    term1[xx++] = ch;
			}

		    } else {
			ch = getc(inifile);
		    }
		}

		memset(term2, '\0', 255);
		if (ch == '=')
		    term2[0] = ch;

		ch = getc(inifile);
		xx = 0;
		memset(term3, '\0', 255);

		// skip the initial spaces 
		if (ch == ' ')
		    while (!feof(inifile) && isspace(ch = getc(inifile)));;

		while (!feof(inifile) && (ch!=10 && ch!=13)) {
		    term3[xx++] = ch;
		    ch = getc(inifile);
		}

		if ((strcmp(term2, "=")) ||
		    (strchr(term1, '#') != NULL) ||
		    (strchr(term3, '#') != NULL)) {
		    strcpy(sret, sdefault);

		} else {
		    if (!strcmp(term1, svalue)) {
			strcpy(sret, term3);
			return sret;

		    }

		}
	    }
	    ch = getc(inifile);
	}
    }

    fclose(inifile);
    strcpy(sret, sdefault);
    return sret;
}

/************************************************************************/

int ini_get_int(char *sname_file, char *section, char *svalue,
		int idefault)
{
    char xdefault[255];

    sprintf(xdefault, "%d", idefault);
    return atoi(ini_get_str(sname_file, section, svalue, xdefault));
}

/*********************************************/
INI_section ini_get_vars(char *sname_file, char *section)
{
    FILE *inifile;
    char sread[255], ch='\0';
    char term1[255], term2[255], term3[255];
    char t_section[255];
    size_t l_section, i='\0', xx;
    INI_section pret;

    l_section = strlen(section) + 2;

    sprintf(t_section, "[%s]", section);

    memset(sread, '\0', 255);

    if ((inifile = fopen(sname_file, "r")) == NULL) {
	printf("IO Error.\n");
	exit(1);
    }

    if (strlen(section)) {
	while (!feof(inifile))	// INI search section
	{
	    for (i = 0; i < l_section - 1; i++)
		sread[i] = sread[i + 1];
	    ch = getc(inifile);
	    sread[l_section - 1] = ch;
	    if (!strcmp(sread, t_section)) {
		ch = getc(inifile);	// Jump the ']' 
		break;
	    }
	}
    } else {
	ch = getc(inifile);
    }

    if (!feof(inifile)) {
	i = 0;
	while (!feof(inifile) && ch != '[')	// loop while not end of section 
	{

	    //skip remarks in the .ini file ....
	    if (ch == '#')
		while (!feof(inifile) && ((ch = getc(inifile)) != 10));;

	    // read lines of section
	    if (isalnum(ch) && !isspace(ch)) {

		xx = 0;
		memset(term1, '\0', 255);
		while (!feof(inifile) && ('=' != ch) && (isprint(ch))) {

		    if (isalnum(ch) || ch == '_')
			term1[xx++] = ch;

		    if (ch == 32) {
			while (!feof(inifile) && ch == 32)
			    ch = getc(inifile);

			if (ch != '=') {
			    term1[xx++] = 32;
			    term1[xx++] = ch;
			    ch = getc(inifile);

			}

		    } else {
			ch = getc(inifile);
		    }
		}

		memset(term2, '\0', 255);
		if (ch == '=')
		    term2[0] = ch;

		ch = getc(inifile);
		xx = 0;
		memset(term3, '\0', 255);

		// skip the initial spaces 
		if (ch == ' ')
		    while (!feof(inifile) && isspace(ch = getc(inifile)));;

		while (!feof(inifile) && (isprint(ch))) {
		    term3[xx++] = ch;
		    ch = getc(inifile);
		}

		if ((!strcmp(term2, "=")) &&
		    (strchr(term1, '#') == NULL) &&
		    (strchr(term3, '#') == NULL)) {

		    sprintf(pret.reg[i].variable, "%s", term1);
		    sprintf(pret.reg[i++].value, "%s", term3);

		}
	    }
	    ch = getc(inifile);

	}
    }
    fclose(inifile);
    sprintf(pret.namesection, "%s", section);
    pret.len = i;
    return pret;
}

/*********************************************/

INI_inifile ini_get_section(char *sname_file)
{
    FILE *inifile;
    char ch;
    int i = 0, xx = 0;
    INI_inifile line;

    sprintf(line.namefile, "%s", sname_file);

    if ((inifile = fopen(sname_file, "r")) == NULL) {
	printf("IO Error.\n");
	exit(1);
    }

    while (!feof(inifile)) {
	ch = getc(inifile);
	if (ch == '[') {
	    ch = getc(inifile);
	    while ((!feof(inifile)) && (ch != 10) && ch != ']') {
		line.section[i][xx++] = ch;
		ch = getc(inifile);
	    }
	    xx = 0;
	    i++;
	}
    }
    line.len = i;
    fclose(inifile);
    return line;
}

/*********************************************/
void getcolorstr(int *r,int *g,int *b, char color[20]) {
	static char rgb[4][5];
	static int i, l, ll;


	l=0; ll=0;
        memset( rgb,'\0', 4*5 );

	for(i=0;color[i];i++){
		if (isspace(color[i])) continue;

		if(isdigit(color[i]) ){
			rgb[l][ll]=color[i];
			ll++;
		}else{
			rgb[l][ll]='\0';
			l++;
			ll=0;
		}
	}
	*r=(int)atoi(rgb[0]);
	*g=(int)atoi(rgb[1]);
	*b=(int)atoi(rgb[2]);
}
/********************************************************************/

// EOF
