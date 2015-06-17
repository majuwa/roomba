#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

#include <avr/io.h>

/**
 * initializes USART for PC (USART 0)
 */
void usart_init_pc ( void );

/**
 * initializes USART for Roomba (USART 3)
 */
void usart_init_roomba ( void );

/**
 * sends one byte to PC via USART, USART must be initialized for PC before
 *
 * @param byte the unint8_t byte to be send
 */
void send_byte_pc ( uint8_t byte );

/**
 * sends one byte to Roomba via USART, USART must be initialized for Roomba before
 *
 * @param byte the unint8_t byte to be send
 */
void send_byte_roomba ( uint8_t byte );

/**
 * recieves one byte from PC via USART, USART must be initialized for PC before
 *
 * @return uint8_t byte from PC
 */
uint8_t receive_byte_pc ( void );

/**
 * recieves one byte from Roomba via USART, USART must be initialized for Roomba before
 *
 * @return uint8_t byte from Roomba
 */
uint8_t receive_byte_roomba ( void );

/**
 * sends a string to PC via USART, USART must be initialized for PC before
 *
 * @param string a char* of string to be send, must end with binary 0
 */
void send_string_pc ( char* string );

#endif
