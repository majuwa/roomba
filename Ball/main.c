#include <avr/io.h>
#include "../lib/usart.h"
#include "../lib/roomba.h"
#include "../lib/tools.h"
#include "ball.h"
#include "../lib/RFM12B.h"
#include "../lib/music.h"
int main ( int argc, char* argv[] )
{
        usart_init_roomba();
        roomba_init();

        set_max_points();

        initialize_ball();

        drive ( BALL_SPEED );

        while ( 1 ) {
                drive_ball();
        }

        return 0;
}
