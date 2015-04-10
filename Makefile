OBJS=boss.o cleanup.c drunkenbot.o idiot.o items.o level.o main.o player.o reset.o turret.o bullet.o draw.o gameover.o init.o keypress.o loop.o menu.o portal.o text.o utils.o
CFLAGS=-s -O2 -Wall
CC=gcc
LIBS=-lSDL -lSDLmain -lSDL_image -lGL -lGLU
TestStuff: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)
