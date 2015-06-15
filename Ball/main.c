#include <avr/io.h>
#include "usart.h"
#include "roomba.h"
#include "tools.h"
#include "pong.h"
#include "ball.h"

int main ( int argc, char* argv[] )
{
        usart_init_roomba();
        roomba_init();
        while ( !read_button ( BUTTON_CLEAN ) );
        // initialize_ball();


        while ( 1 ) {
                /*
                      drive ( 300 );
                      drive_ball ( 300 );
                      */
                for ( uint16_t i = 90; i<361; i+=90 ) {
                        turn ( i);
                        char test[6];
                        number2String ( get_angle(),test );
                        set_Display ( test );
			while ( !read_button ( BUTTON_CLEAN ) );
                }
        }
        return 0;
}
