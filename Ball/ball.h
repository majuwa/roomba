#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED
#include <avr/io.h>

#define WALL_VALUE 200
#define CLIFF_L -1
#define CLIFF_R 1
#define NO_CLIFF 0

#define LIGHT_BUMPER_LEFT 1
#define LIGHT_BUMPER_FRONT_LEFT 2
#define LIGHT_BUMPER_CENTER_LEFT 4
#define LIGHT_BUMPER_CENTER_RIGHT 8
#define LIGHT_BUMPER_FRONT_RIGHT 16
#define LIGHT_BUMPER_RIGHT 32

int16_t get_angle ( void );

void initialize_ball ( void );

int8_t is_wall ( void );

int8_t check_side ( uint16_t cliffL, uint16_t cliffR );

int16_t calc_new_angle ( int8_t cliff );

void drive_ball ( int16_t velocity );

int16_t is_pong(uint16_t* value);

#endif
