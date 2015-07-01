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
	my_msleep(500);
	set_max_points(read_user_input_single());
	
	set_Display("OK  ");
	my_msleep(1000);
	set_Display("   ");
        /*
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
	initialize_ball();

        while ( 1 ) {
                drive ( 500 );
                drive_ball ( 500 );
        }
	/*
	while(1){
	  uint8_t data;
	  read_values(13, &data,1);
	  if(!data)
	    set_Display("0    ");
	  else
	    set_Display("1   ");
	}*/
        return 0;
}
