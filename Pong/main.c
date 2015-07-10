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

        setID();
        initialize_pong();

        while ( 1 )
                drive_pong();


        return 0;
}
