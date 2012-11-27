About
================

*Test Stuff*, a simple game.

This is \`get-a-key-go-to-exit-keep-alive\'. Completely in 2D.
There are no any story; you playing as the red square,
looking for keys around on a level. Your enemies are:
 - *Idiot*: blue square with rotating arrows, can move only up or down.
 - *The Drunken Bot*: looks like player, but it's green and it's smiles to you.
Walks in random direction, the same way like player.
 - *A turret*: shoots in random direction with a constant interval.
Bullets can fly through the walls.
- *The Boss*: randomly moves left or right at the top of level,
fires from two guns. Starts raging, when his health becomes low.

There are no savegames between levels or worlds, so if you'll lost
all lives and die somewhere at 15th level (for example), you will
be returned to the first level. Athe game begin you have five lives;
collecting of a big green square will give you one more live (aka 1-UP).

Nobody (and nothing) is able to slay you, when
you're on the start; you can shoot only in boss-level,
and your bullets can damage only the Boss.

Controls
================

*Arrow keys* - moving;
*Space* - shoot (in boss level only). Releasing of space causes shot too.

Compiling
=============

Type *'make'* to build *TS*. In case of linkage fault you can compile it by typing the following:
`cc *.c -o TestStuff -lSDL -lSDLmain -lSDL_image -lGL -lGLU -ldl -lm -pthread -s -O2`.

License
=======

*Test Stuff* is a free software and comes under the terms of GNU General Public License v2.
