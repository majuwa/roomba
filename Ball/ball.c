#include "ball.h"
#include "tools.h"
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
