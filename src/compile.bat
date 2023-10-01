gcc *.c -o kof91x6.exe -lalleg
INC=.
gcc *.c -o kof91x6 -I. `allegro-config --cflags --libs --static release`


sudo apt install liballegro4-dev