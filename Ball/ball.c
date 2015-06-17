#include "ball.h"
#include "tools.h"
#include "roomba.h"
#include "usart.h"
#include <stdlib.h>
int16_t angle = 0;

int16_t get_angle()
{
        int8_t data_byte[2];
        int16_t angle_value;
        read_values_signed ( 20,data_byte,2 );
        angle_value = concat_bytes_signed ( data_byte[0], data_byte[1] );
        angle_value *= 3;
        return ( angle_value < 0 ) ? ( 360 + angle_value ) : angle_value;
}

void initialize_ball()
{
        get_angle();
        srand ( coder_values_r() );
        turn2 ( rand() %40-20,300 );
}

int8_t is_wall()
{
        uint8_t data_byte[2];
        uint16_t wall_sensor;
        read_values ( 27,data_byte,2 );
        wall_sensor = concat_bytes ( data_byte[0], data_byte[1] );
        //if ( wall_sensor>WALL_VALUE ) {
        //return 1;
        //} else {
        //return 0;
        //}
        char string [6];
        number2String ( wall_sensor, string );
        set_Display ( string );
        return 0;// wall_sensor;
}

int8_t check_side ( uint16_t cliffL, uint16_t cliffR )
{
        int16_t difference = ( int16_t ) ( cliffL - cliffR );
        if ( cliffL < 1000 && cliffR < 1000 )
                return NO_CLIFF;
        else if ( cliffL > 3000 || cliffR > 3000 )
                return NO_CLIFF;
        if ( difference > 400 ) {
                /*
                	char t_string[6] = {0};
                	number2Hex(difference,t_string);
                	set_Display(t_string);
                	stop();
                	my_msleep(20000);*/
                return CLIFF_L;
        } else if ( difference < -400 ) {
                /*
                  char t_string[6] = {0};
                	number2Hex(difference,t_string);
                	set_Display(t_string);
                	stop();
                	my_msleep(20000);*/
                return CLIFF_R;
        } else {
                return NO_CLIFF;
        }
}

int16_t calc_new_angle ( int8_t cliff )
{
        if ( !cliff )
                return 0;

        int16_t angle_change = get_angle();
        angle = mod ( ( angle + angle_change ) , 360 );



        if ( cliff == CLIFF_L ) {
                return - 2 * ( angle % 90 );
        }

        if ( cliff == CLIFF_R ) {
                return 2* ( 90- ( angle%90 ) );
        }

        return 0;
}
int16_t is_pong ( uint16_t* value )
{
        if ( value[2] > 600 || value[3] > 600 ) {
                //set_Display ( "Cent" );
                return rand_tools ( 170,190 );
        } else if ( value[1] > 400 ) {
                //set_Display ( "F-Lef" );
                return -rand_tools ( 120, 160 );
        } else if ( value[4] >400 ) {
                //set_Display ( "F-Righ" );
                return rand_tools ( 120,160 );
        } else if ( value[0] > 600 ) {
                //set_Display ( "LEFT" );
                return -rand_tools ( 90,120 );
        } else if ( value[5] > 400 ) {
                //set_Display ( "Righ" );
                return rand_tools ( 90,120 );
        } else {
                return 0;
        }
}
void drive_ball ( int16_t velocity )
{
        uint8_t packetIDs [] = {29, 30,13,46,47,48,49,50,51,7};
        uint8_t datas [18];
        uint16_t l_bumpers[6];
        multiple_sensors ( packetIDs, datas, 18, 10 );
        char string[6] = {0};
        int8_t cliff =  check_side ( concat_bytes ( datas[0], datas[1] ),concat_bytes ( datas[2], datas[3] ) );
        number2Hex ( concat_bytes ( datas[0], datas[1] ),string );
        set_Display ( string );
        if ( cliff ) {
                stop();
                drive_roomba_clicks ( 30,velocity );
                multiple_sensors ( packetIDs,datas,18,10 );

                number2Hex ( concat_bytes ( datas[0], datas[1] ),string );
                set_Display ( string );
        }

        int16_t new_angle = calc_new_angle ( cliff );
        if ( concat_bytes ( datas[0], datas[1] ) > 3000 ) {
                start_after_won();
                return;
        } else if ( new_angle ) {

                char t_string[6] = {0};
                number2StringSigned ( angle,t_string );
                //set_Display ( t_string );
                stop();
                turn2 ( new_angle, 200 );
                //set_Display("    ");
        }
        int8_t j = 0;
        for ( int8_t i= 0; i< 6; i++ ) {
                l_bumpers[i] = concat_bytes ( datas[2*i+5],datas[2*i+6] );
        }
        /*
        char test[6];
        number2String(l_bumpers[1],test);
        set_Display(test)
        ;*/
        int16_t turn_value = is_pong ( l_bumpers );
        if ( turn_value!=0 ) {
                stop();
                turn2 ( turn_value, 200 );
                set_Display ( "pong" );
        }
        if ( datas[17] & BUMP_RIGHT || datas[17] & BUMP_LEFT )
                stop();
        /*
        char test[6];
        number2String ( angle,test );
        set_Display ( test );*/

       // my_msleep ( 30 );
}
void start_after_won()
{
        char* string1 = "GOAL";
        set_Display ( string1 );
        stop();
        uint8_t ir_value;/*
	send_byte_roomba(151);
	send_byte_roomba(IR_0);*/
        do {
                read_values ( 17, &ir_value, 1 );
                if ( ir_value == IR_RIGHT )
                        drive ( 100 );
                else if ( ir_value == IR_LEFT )
                        turn2 ( 5,100 );
                else
                        stop();
        } while ( ir_value != IR_ON_OFF );
}

