@echo off
goto geramake
@echo off
IF X==%DJGPP%X goto varnf
echo Variable DJGPP OK!
echo.

gcc > nul
IF NOT errorlevel 1 goto gccnf
echo gcc OK!
echo.

make -f error > nul
IF NOT errorlevel 1 goto makenf
echo make OK!
echo.

:geramake
rem *************************************************
echo ################################################################# >makefile
echo # DOS makefile of FFE  >>makefile
echo #    Original makefile by Duncan Mac-Vicar (duncan@linuxfan.com) >>makefile
echo # >>makefile
echo # Rick Leite (ric@asbyte.com.br) >>makefile
echo. >>makefile
echo LIBS=alleg >>makefile
echo INCLUDE=/usr/local/include >>makefile
echo LOAD=/usr/X11R6/lib        >>makefile
echo EXEC=../kof2003              >>makefile
echo GH=config.h global.h macros.h defs.h >>makefile
echo OBJS=batle.o display.o inits.o intro.o loads.o logger.o main.o palettes.o monitor.o moves.o mp3.o refresh.o screenshot.o startmenu.o wavsounds.o initfiles.o sprites.o effects.o options.o>>makefile
echo. >>makefile
echo CC=gcc >>makefile
echo CFLAGS=-g -O6 >>makefile
echo. >>makefile
echo include makefile.dos >>makefile
echo #define SO DOS > so.inc

rem *************************************************
echo type:
echo   make all
goto fim

:varnf
echo ERROR: djgpp environment variable not found..
goto fim

:gccnf
echo ERROR: djgpp gcc compiler not found..
goto fim

:makenf

:fim
make clean
