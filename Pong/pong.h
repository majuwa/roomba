#ifndef PONG_H_INCLUDED
#define PONG_H_INCLUDED
#include <avr/io.h>

#define PONG_SPEED 150
#define PONG_RESET 5
void initialize_pong(void);
void drive_pong();
void getData();
#endif
