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
void drive_pong(void);
/**
 * Move the pong roomba in a straight line using the P controller
 */
void drive_pong_straight(int16_t diff_r, int16_t diff_l, int8_t direction);
/**
 * receive user-data over radio
 */
void getData(void);
/**
 * Set ID of pong-roomba via minute- and hour-buttons
 */
void setID(void);
#endif
