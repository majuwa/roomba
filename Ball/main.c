#include <avr/io.h>
#include "usart.h"
#include "roomba.h"
#include "tools.h"
#include "pong.h"
int main(int argc, char* argv[]){
	usart_init_roomba();
	roomba_init();
	while(!read_button(BUTTON_CLEAN));
	initialize_pong();
	while(1)
		drive_pong();
	return 0;
}
