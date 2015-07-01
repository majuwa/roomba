#include <avr/io.h>
#include "../lib/usart.h"
#include "../lib/roomba.h"
#include "../lib/tools.h"
#include "pong.h"
#include "../lib/RFM12B.h"
int main ( int argc, char* argv[] )
{
        uint8_t id_2;
        uint8_t id_3;

        usart_init_roomba();
        roomba_init();
        do {
                if ( read_button ( BUTTON_MINUTE ) ) {
                        id_2 = 1;
                        id_3 = 0;
                        set_Display ( "Pl-1" );
                } else if ( read_button ( BUTTON_HOUR ) ) {
                        id_3 = 1;
                        set_Display ( "Pl-2" );
                        id_2 = 0;
                }
        } while ( !read_button ( BUTTON_CLEAN ) ||  !(id_3 | id_2));

	set_Display("init");
        if ( id_2 ) {
                Initialize ( 2,20 );

        } else {
                Initialize ( 3,20 );

        }
        initialize_pong();
        set_Display ( "----" );
        while ( 1 )
                drive_pong();
        return 0;
}
