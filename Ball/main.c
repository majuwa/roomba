#include <avr/io.h>
#include "../lib/usart.h"
#include "../lib/roomba.h"
#include "../lib/tools.h"
#include "ball.h"
#include "../lib/RFM12B.h"

int main ( int argc, char* argv[] )
{
        usart_init_roomba();
        roomba_init();
        while ( !read_button ( BUTTON_CLEAN ) );
        initialize_ball();

        Initialize ( 1,20 );


        while ( 1 ) {
                drive ( 500 );
                drive_ball ( 500 );
        }
        return 0;
}
