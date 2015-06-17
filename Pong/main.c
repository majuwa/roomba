#include <avr/io.h>
#include "../lib/usart.h"
#include "../lib/roomba.h"
#include "../lib/tools.h"
#include "pong.h"
#include "../lib/RFM12B.h"
int main ( int argc, char* argv[] )
{
        usart_init_roomba();
        roomba_init();
        Initialize ( 2,20 );
        while ( !read_button ( BUTTON_CLEAN ) );

        initialize_pong();
        while ( 1 )
                drive_pong();
        return 0;
}
