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
