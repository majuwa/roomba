#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#include <avr/io.h>
#include <util/delay_basic.h>
#include <stdio.h>
#include <stdlib.h>
#define WAIT 500

/**
 * waits for a certain time and does nothing
 *
 * @param ms milliseconds to wait
 */
void my_msleep ( uint16_t ms );

/**
 * initializes timer (sets CTC mode and CLK/1024) and sets interrupt time
 *
 * @param ms milliseconds for timer value
 */
void initializeTimer ( uint16_t ms );

/**
 * concatenates two bytes (high an low byte)
 *
 * @param high the high byte
 * @param low the low byte
 * @return uint16_t value of concatenated bytes
 */
uint16_t concat_bytes ( uint8_t high, uint8_t low );

int16_t concat_bytes_signed ( int8_t high, int8_t low );

/**
 * splits an int16_t value into two int8_t bytes
 *
 * @param value the 16bit value
 * @param result pointer to int8_t bytes, must have reseved space for two bytes
 */
void split_bytes ( int16_t value, int8_t* result );

/**
 * transforms a number into a string for output on display
 *
 * @param value the 16bit number
 * @param result char pointer for result string, must have reserved space for 6 bytes
 */
void number2String ( uint16_t value, char* result );

void number2StringSigned ( int16_t value, char* result );

/**
 * transforms a number into a hex-string for output on display
 *
 * @param value the 16bit number
 * @param result char pointer for result string, must have reserved space for 6 bytes
 */
void number2Hex ( uint16_t value, char* result );

/**
 * concatenates two strings for "%name : %value"-output
 *
 * @param name string of attribute-name
 * @param value string of attribute-value
 * @param result char pointer for result string, must have reserved enough space
 */
void string_concat ( char* name, char* value, char* result );

int16_t rand_tools(int16_t start, int16_t end);
int16_t mod(int16_t value, int16_t div);
#endif
