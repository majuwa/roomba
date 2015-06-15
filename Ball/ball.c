#include "ball.h"
#include "tools.h"
#include "roomba.h"
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
        turn ( rand() %40-20 );
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
	if(cliffL < 2000 && cliffR < 2000)
	  return NO_CLIFF;
	else if(cliffL > 3000 || cliffR > 3000)
	  return NO_CLIFF;
        if ( difference > 400 ) {/*
		char t_string[6] = {0}; 
		number2Hex(difference,t_string);
		set_Display(t_string);
		stop();
		my_msleep(20000);*/
                return CLIFF_L;
        } else if ( difference < -400 ) {/*
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
        angle = mod(( angle + angle_change ) , 360);



        if ( cliff == CLIFF_L ) {
                return - (mod( ( 2*angle ),180));
        }

        if ( cliff == CLIFF_R ) {
                return ( mod( 2*angle ,180 ));
        }

        return 0;
}
int16_t is_pong ( uint16_t* value )
{
        if ( value[0] > 150 ) {
                //set_Display ( "LEFT" );
                return -rand_tools ( 40,60 );
        } else if ( value[1] > 100 ) {
                //set_Display ( "F-Lef" );
                return -rand_tools ( 90, 150 );
        } else if ( value[2] > 170 || value[3] > 150 ) {
                //set_Display ( "Cent" );
                return rand_tools ( 170,190 );
        } else if ( value[4] >100 ) {
                //set_Display ( "F-Righ" );
                return rand_tools ( 90,150 );
        } else if ( value[5] > 150 ) {
                //set_Display ( "Righ" );
                return rand_tools ( 40,60 );
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
	if(cliff){
	  stop();
	  drive_roomba(10,velocity);
	  multiple_sensors(packetIDs,datas,18,10);
	}
	
        int16_t new_angle = calc_new_angle ( cliff );
        if ( concat_bytes ( datas[0], datas[1] )> 3000 ) {
                char* string1 = "GOAL";
                set_Display ( string1 );
                stop();
		my_msleep(2000);
		//turn(angle);
		drive_roomba_clicks(1000, angle > 0 ? -100 : 100);
                return;
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
               turn ( turn_value );
        }
        if ( datas[17] & BUMP_RIGHT || datas[17] & BUMP_LEFT )
                stop();
	/*
        char test[6];
	number2String(angle,test);
	set_Display(test);
	*/
        if(new_angle)
        {
		 char t_string[6] = {0}; 
		number2StringSigned(new_angle,t_string);
		set_Display(t_string);
		stop();
		my_msleep(2000);
        	stop();
        	turn(new_angle);
        	//set_Display("    ");
        }
        my_msleep ( 30 );
}
