#include "pong.h"
#include "roomba.h"
#include "tools.h"
int16_t max_r;
uint16_t start_counter;
uint16_t end_counter;
uint8_t id;
uint8_t points;
uint8_t points_enemy;
void pong_show_points(){
      char t[6];
      number2String(points,t);
      set_Display(t);
}
void initialize_pong()
{
	points = 0;
        start_counter = coder_values_r();
        uint16_t return_value = 0;
        uint8_t ir_value;
	id = IR_0;
	/*
        set_Display ( "ID??" );
        do {
                read_values ( 17,&ir_value,1 );
                id = ir_value;
        } while ( ir_value != IR_0 || ir_value != IR_0 << 1 );*/
        do {
                read_values ( 17, &ir_value, 1 );
                if ( ir_value == IR_UP )
                        drive ( 100 );
                else
                        stop();
        } while ( ir_value != IR_RIGHT );
        end_counter = coder_values_r();
        max_r = ( int16_t ) ( end_counter - start_counter );
	pong_show_points();
}
void drive_pong()
{
        uint16_t value = coder_values_r();
        int16_t difference = ( int16_t ) ( value -start_counter );

        uint8_t ir_value;
        read_values ( 17, &ir_value, 1 );

        if ( difference>max_r ) {
                stop();
                my_msleep ( 200 );
                drive_roomba_clicks ( PONG_RESET,-PONG_SPEED );
        } else if ( difference < 0 ) {
                stop();
                my_msleep ( 200 );
                drive_roomba_clicks ( PONG_RESET,PONG_SPEED );
        } else {
                if ( ir_value == IR_UP )
                        drive ( PONG_SPEED );
                else if ( ir_value == IR_DOWN )
                        drive ( - PONG_SPEED );
		else if(ir_value == id){
		  points++;
		  pong_show_points();
		}
                else
                        stop();
        }
        pong_show_points();
}
