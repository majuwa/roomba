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
    initialize_ball();
    while ( 1 ) {
		drive(150);
		drive_ball(150);
    }
    return 0;
}
