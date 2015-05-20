#include <avr/io.h>
#include "usart.h"
#include "roomba.h"
#include "tools.h"
#include "pong.h"
int main(int argc, char* argv[]){
	usart_init_roomba();
	roomba_init();
	while(!read_button(BUTTON_CLEAN));
	initialize_ball();
	while(1){
		int8_t wall = is_wall();
		char string[6];
		number2String(wall,string);
		set_Display(string);
	}
	return 0;
}
