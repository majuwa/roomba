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
	do{
	id_2 = read_button(BUTTON_MINUTE);
	id_3 = read_button(BUTTON_HOUR);
	}while(!id_2 && !id_3);
	
	
        if(id_2){
	  Initialize ( 2,20 );
	  set_Display("ID-2 ");
	}
	else{
	  Initialize(3,20);
	  set_Display("ID-3 ");
	}
        initialize_pong();
	set_Display("----");
        while ( 1 )
                drive_pong();
        return 0;
}
