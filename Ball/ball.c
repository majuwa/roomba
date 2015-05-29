#include "ball.h"
#include "tools.h"
#include "roomba.h"

int16_t angle = 45;

int16_t get_angle()
{
    int8_t data_byte[2];
    int16_t angle_value;
    read_values_signed ( 20,data_byte,2 );
    angle_value = concat_bytes_signed ( data_byte[0], data_byte[1] );
    angle_value *= 3;
    return (angle_value < 0) ? (360 + angle_value) : angle_value;
}

void initialize_ball()
{
    get_angle();
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
    number2String(wall_sensor, string);
    set_Display(string);
    return 0;// wall_sensor;
}

int8_t check_side(uint16_t cliffL, uint16_t cliffR)
{
    int16_t difference = ( int16_t ) ( cliffL - cliffR );

    if ( difference > 400 ) {
        return CLIFF_L;
    } else if ( difference < -400 ) {
        return CLIFF_R;
    } else {
        return NO_CLIFF;
    }
}

int16_t calc_new_angle (int8_t cliff)
{
	if(!cliff)
		return 0;
		
	int16_t angle_change = get_angle();
	angle = (angle + angle_change) % 360;
	
	//char string[6];
	//number2String(angle, string);
	//set_Display(string);
	
	if(cliff == CLIFF_L)
	{
		return -((2*angle) % 180);
	}
	
	if(cliff == CLIFF_R)
	{
		return ((2*angle) % 180);
	}
	
	return 0;
}
int8_t is_pong(uint8_t value){
	switch(value){
		case LIGHT_BUMPER_LEFT:
			set_Display("LEFT");
			break;
		case LIGHT_BUMPER_FRONT_LEFT:
			set_Display("L-Le");
			break;
		case LIGHT_BUMPER_CENTER_LEFT | LIGHT_BUMPER_CENTER_RIGHT:
			set_Display("Cent");
			break;
		case LIGHT_BUMPER_FRONT_RIGHT:
			set_Display("L-Ri");
			break;
		case LIGHT_BUMPER_RIGHT:
			set_Display("Rigt");
			break;
		default:
			return 0;
		}
		return 1;
}
void drive_ball (int16_t velocity)
{
	uint8_t packetIDs [] = {29, 30,13,45};
    uint8_t datas [6];
    multiple_sensors ( packetIDs, datas, 6, 4 );
    int8_t cliff =  check_side(concat_bytes ( datas[0], datas[1] ),concat_bytes ( datas[2], datas[3] ));
	int16_t new_angle = calc_new_angle(cliff);
    if(datas[4]){
		char* string1 = "GOAL";
		set_Display(string1);
		stop();
		return;
	}
	if(is_pong(datas[5])){
		stop();
		return;
		//turn(180);
	}
	/*
	char string[6];
	number2String(new_angle, string);
	set_Display(string);
	
	if(new_angle)
	{
		stop();
		turn(new_angle);
		drive(velocity);
	}*/
	my_msleep(30);
}
