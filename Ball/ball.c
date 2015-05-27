#include "ball.h"
#include "tools.h"
#include "roomba.h"

int16_t angle = 0;

int16_t get_angle(){
	int8_t data_byte[2];
	int16_t angle_value;
	read_values_signed(20,data_byte,2);
	angle_value = concat_bytes_signed(data_byte[0], data_byte[1]);
	return angle_value * 3;
}

void initialize_ball(){
	get_angle();
}

int8_t is_wall(){
	uint8_t data_byte[2];
	uint16_t wall_sensor;
	read_values(27,data_byte,2);
	wall_sensor = concat_bytes(data_byte[0], data_byte[1]);
	if(wall_sensor>WALL_VALUE)
		return 1;
	else
		return 0;
}

int8_t checkSide(){
	uint8_t packetIDs [] = {29, 30};
	uint8_t datas [4];
	multiple_sensors(packetIDs, datas, 4, 2);
	uint16_t cliffL = concat_bytes(datas[0], datas[1]);
	uint16_t cliffR = concat_bytes(datas[2], datas[3]);
	int16_t difference = (int16_t) (cliffLeft - cliffRight);
	
	if(difference > 200)
		return CLIFF_L;
	else if (difference < -200)
		return CLIFF_R;
	else
		return NO_CLIFF;
}
