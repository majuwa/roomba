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
        while ( !read_button ( BUTTON_CLEAN ) );/*
	while(1){
	
	  drive(100);
	  uint8_t value;
	  read_values(13, &value,1);
	  if(value){
	    stop();
	    while(1);
	  }

	}*/
	
        Initialize ( 1,20 );
	

        while ( 1 ) {
                drive ( 500 );
                drive_ball ( 500 );
        }
        return 0;
}
