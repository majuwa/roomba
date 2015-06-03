#include <usart.h>

void usart_init_roomba()
{
        //set double spped (S.224)
        UCSR3A |= ( 1 << U2X3 );

        //set asynchronous (S.225) + set parity off (S. 226) + set one stopbit (S. 226) + set character size = 8bit (S. 226)
        UCSR3C |= ( 0 << UMSEL30 ) | ( 0 << UMSEL31 ) | ( 0 << UPM30 ) | ( 0<< UPM31 ) | ( 0 << USBS3 ) | ( 1 << UCSZ30 ) | ( 1 << UCSZ31 );

        //character size + enable reciever and transmitter
        UCSR3B |= ( 0 << UCSZ32 ) | ( 1 << RXEN3 ) | ( 1 << TXEN3 );

        //set baud rate (S.227) [115200]
        UBRR3H = 0x00;
        UBRR3L = 0x10;

}

void send_byte_roomba ( uint8_t byte )
{
        while ( ! ( UCSR3A & ( 1 << UDRE3 ) ) ) {}
        UDR3 = byte;
        while ( ! ( UCSR3A & ( 1<< TXC3 ) ) ) {}
}

uint8_t receive_byte_roomba ( void )
{
        uint8_t recieve_complete = 0;

        while ( !recieve_complete ) {
                recieve_complete = UCSR3A & ( 1<<RXC3 );
        }

        return UDR3;
}

void usart_init_pc()
{
        //set double spped (S.224)
        UCSR0A |= ( 1 << U2X0 );

        //set asynchronous (S.225) + set parity off (S. 226) + set one stopbit (S. 226) + set character size = 8bit (S. 226)
        UCSR0C |= ( 0 << UMSEL00 ) | ( 0 << UMSEL01 ) | ( 0 << UPM00 ) | ( 0<< UPM01 ) | ( 0 << USBS0 ) | ( 1 << UCSZ00 ) | ( 1 << UCSZ01 );

        //character size + enable reciever and transmitter
        UCSR0B |= ( 0 << UCSZ02 ) | ( 1 << RXEN0 ) | ( 1 << TXEN0 );

        //set baud rate (S.227) [115200]
        UBRR0H = 0x00;
        UBRR0L = 0x10;

}

void send_byte_pc ( uint8_t byte )
{
        while ( ! ( UCSR0A & ( 1 << UDRE0 ) ) ) {}
        UDR0 = byte;
        while ( ! ( UCSR0A & ( 1<< TXC0 ) ) ) {}
}

uint8_t receive_byte_pc ( void )
{
        uint8_t recieve_complete = 0;

        while ( !recieve_complete ) {
                recieve_complete = UCSR0A & ( 1<<RXC0 );
        }

        return UDR0;
}

void send_string_pc ( char* string )
{
        for ( uint8_t i = 0; string[i] != '\0'; i++ ) {
                send_byte_pc ( string[i] );
        }
        send_byte_pc ( '\r' );
        send_byte_pc ( '\n' );
}
