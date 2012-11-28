/*

keypress.h

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#ifndef _KEYPRESS_H
#define _KEYPRESS_H

#define KPSTATE_NULL        0
#define KPSTATE_DOWN        1
#define KPSTATE_UP          2
#define KPSTATE_PRESSED     3

/* Initializes a space storing all keys' states */
void kpCreateSpace();
/* Returns button's state */
char kpGetState(unsigned int button);
void kpLoop();
void kpFreeSpace();

#endif // keypress.h
