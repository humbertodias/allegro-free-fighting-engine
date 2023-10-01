##############################################################
# Linux Makefile of FFE 
#    Original Makefile by Duncan Mac-Vicar (duncan@linuxfan.com)
#  
# Rick Leite (ric@asbyte.com.br)
# Claudemir P. da Luz Jr. <claudemir@virtuallink.com.b>

LIBS=nux-gnu/liballeg.so alleg_unsharable a
INCLUDE=/usr/local/include
LOAD=/usr/X11R6/lib
EXEC=../kof2003
GH=config.h global.h macros.h defs.h
OBJS=batle.o display.o inits.o intro.o loads.o logger.o main.o palettes.o monitor.o moves.o mp3.o refresh.o screenshot.o startmenu.o initfiles.o wavsounds.o sprites.o effects.o options.o

#CC=/bin/gcc
# TODO - MINE
CC=clang
#CFLAGS=-g -W -O3 -static
#CFLAGS=-g -Wall -Werror -O6 -Werror=deprecated-declarations -Werror -Winvalid-source-encoding -Wheader-guard
CFLAGS=-g -Wall -Werror -O3 -Werror=deprecated-declarations -Werror -Winvalid-source-encoding -Wheader-guard

include makefile.lnx
