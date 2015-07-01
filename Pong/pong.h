#ifndef PONG_H_INCLUDED
#define PONG_H_INCLUDED
#include <avr/io.h>

#define PONG_SPEED 100
#define PONG_RESET 5
/**
 * Intialize the pong
 * -> set coder_max und coder_min
 */
void initialize_pong(void);
/**
 * Drive the pong with a ir-remote
 */
void drive_pong();
/*
 * receive user-data over radio
 */
void getData();
#endif
