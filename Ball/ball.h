#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED
#include <avr/io.h>

#define WALL_VALUE 200
#define CLIFF_L -1
#define CLIFF_R 1
#define NO_CLIFF 0

int16_t get_angle ( void );

void initialize_ball ( void );

int8_t is_wall ( void );

void checkSide ( void );

#endif
