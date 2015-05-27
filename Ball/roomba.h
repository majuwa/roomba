#ifndef ROOMBA_H_INCLUDED
#define ROOMBA_H_INCLUDED

#define BUTTON_CLEAN 0x01
#define BUTTON_SPOT 0x02
#define BUTTON_DOCK 0x04
#define BUTTON_MINUTE 0x08
#define BUTTON_HOUR 0x10
#define BUTTON_DAY 0x20
#define BUTTON_SCHEDULE 0x40
#define BUTTON_CLOCK 0x80

#define BUMP_RIGHT 0x01
#define BUMP_LEFT 0x02

#define IR_RIGHT 143
#define IR_LEFT 141
#define IR_UP 139
#define IR_DOWN	140
#define IR_ON_OFF 138

#define IR_0 129

#define CLICKS_PER_MM 2.25
#define WHEEL_2_CENTER 110

#include <avr/io.h>

void read_values_signed ( uint8_t packetid, int8_t* data, uint8_t length );

/**
 * initializes Roomba (sets start and safe mode)
 */
void roomba_init ( void );

/**
 * sets 4-segment display of Roomba
 *
 * @param data string to show on display, only first 4 characters are considered
 */
void set_Display ( char *data );

/**
 * checks whether a certain button is pressed
 *
 * @param button bit pattern of button
 * @return 1 if button is pressed, 0 if not pressed
 */
uint8_t read_button ( uint8_t button );

/**
 * reads values of sensors etc. of Roomba
 *
 * @param packetid packet id of questioned sensor
 * @param data pointer for result values
 * @param length length (#bytes) of result value
 */
void read_values ( uint8_t packetid, uint8_t * data, uint8_t length );

/**
 * starts driving mode of Roomba
 *
 * @param velocity driving velocity
 */
void drive ( int16_t velocity );

/**
 * starts driving mode of Roomba
 *
 * @param velocityRight driving velocity of right wheel
 * @param velocityLeft driving velocity of left wheel
 */
void drive_wheels ( int16_t velocityRight, int16_t velocityLeft );

/**
 * stops driving of Roomba
 */
void stop ( void );

/**
 * Roomba drives for a certain distance with a certain speed using my_msleep
 *
 * @param distance driving distance in millimeters
 * @param velocity driving velocity
 */
void drive_roomba ( uint16_t distance, int16_t velocity );

/**
 * Roomba drives for a certain distance with a certain speed using encoder counts
 *
 * @param distance driving distance in millimeters
 * @param velocity driving velocity
 */
void drive_roomba_clicks ( uint16_t distance, int16_t velocity );

/**
 * Roomba drives forward and backward or turns left or right on place, using IR arrow-keys
 *
 * @param ir_code IR code of arrow keys
 * @param velocity driving velocity
 */
void drive_roomba_code ( uint8_t ir_code, int16_t velocity );

/**
 * User input via IR remote (numbers)
 *
 * @return input value
 */
uint16_t read_user_input ( void );

/**
 * Roomba drives in a circle a with certain radius and velocity
 *
 * @param radius radius of circle, lower than 2000
 * @param velocity driving velocity
 */
void drive_circle ( uint16_t radius, uint16_t velocity );

/**
 * Roomba drives in a circle a with certain radius and velocity for a certain time
 *
 * @param radius radius of circle, lower than 2000
 * @param velocity driving velocity
 * @param time duration of drive
 */
void drive_circle_time ( uint16_t radius, uint16_t velocity, uint16_t time );

/**
 * Roomba drives a rectangle with a certain length an width
 *
 * @param length length of rectangle in mm
 * @param width width of rectangle in mm
 */
void drive_rectangle ( uint16_t length, uint16_t width );

/**
 * Roomba turns through a certain amount of degrees
 *
 * @param degree degree for turning, negative values for turning right, positive for turning left
 */
void turn ( int16_t degree );

/**
 * When Roomba collides with an obstacle, it will set back 10cm and turns some degrees
 *
 * @param degree degrees for turning
 * @param value of bump sensors
 * @return 1 if Roomba has collided, 0 if not
 */
uint8_t backup_collision ( uint16_t degree, uint8_t bump );

/**
 * Reads values from multiple Roomba sensors
 *
 * @param packetIDs the packetIds of the sensors
 * @param datas array for sensor values
 * @param dataSize size of data array
 * @param packentCount number of packetIDs
 */
void multiple_sensors ( uint8_t* packetIDs, uint8_t* datas, uint8_t dataSize, uint8_t packetCount );

/**
 * Roomba follows a line circuit using P-method
 *
 * @param cliffLeft value of left cliff sensor
 * @param cliffRight value of right cliff sensor
 * @param velocity standard driving velocity
 * @return 1 if correction was necessary, 0 if not
 */
uint8_t p_follow_line ( uint16_t cliffLeft, uint16_t cliffRight, int16_t velocity );

/**
 * Roomba follows a line circuit using PID-method
 *
 * @param cliffLeft value of left cliff sensor
 * @param cliffRight value of right cliff sensor
 * @param velocity standard driving velocity
 * @return 1 if correction was necessary, 0 if not
 */
uint8_t pid_follow_line ( uint16_t cliffLeft, uint16_t cliffRight, int16_t velocity );

#endif
