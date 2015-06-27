#include "music.h"

void save_songs() {
	int song0 [] = {A4, Q, A4, Q, A4, Q, F4, ED, C5, S, A4, Q, F4, ED, C5, S, A4, HALF};
	int song1 [] = {E5, Q, E5, Q, E5, Q, F5, ED, C5, S, GIS4, Q, F4, ED, C5, S, A4, HALF};
	int song2 [] = {A5, Q, A4, ED, A4, S, A5, Q, GIS5, E, G5, E, FIS5, S, F5, S, FIS5, S};
	int song3 [] = {R, E, AIS4, E, DIS5, Q, D5, E, CIS5, E, C5, S, B4, S, C5, E};
	int song4 [] = {R, E, F4, E, GIS4, Q, F4, ED, GIS4, S, C5, Q, A4, ED, C5, S, E5, HALF};
	int song5 [] = {R, E, F4, E, GIS4, Q, F4, ED, C5, S};
	
	send_byte_roomba(140);
	send_byte_roomba(0);
	send_byte_roomba(9);
	for(int i = 0; i < 18; i++) {
		send_byte_roomba(song0[i]);
	}
	
	send_byte_roomba(140);
	send_byte_roomba(1);
	send_byte_roomba(9);
	for(int i = 0; i < 18; i++) {
		send_byte_roomba(song1[i]);
	}
	
	send_byte_roomba(140);
	send_byte_roomba(2);
	send_byte_roomba(9);
	for(int i = 0; i < 18; i++) {
		send_byte_roomba(song2[i]);
	}
	
	send_byte_roomba(140);
	send_byte_roomba(3);
	send_byte_roomba(8);
	for(int i = 0; i < 16; i++) {
		send_byte_roomba(song3[i]);
	}
	
	send_byte_roomba(140);
	send_byte_roomba(4);
	send_byte_roomba(9);
	for(int i = 0; i < 19; i++) {
		send_byte_roomba(song4[i]);
	}
}

void play_songs() {
	send_byte_roomba(141);
	send_byte_roomba(0);
	my_msleep(MEASURE_TIME*3000);
	/*
	send_byte_roomba(141);
	send_byte_roomba(1);
	my_msleep(MEASURE_TIME*3000);
	
	send_byte_roomba(141);
	send_byte_roomba(2);
	my_msleep(MEASURE_TIME*2500);
	
	send_byte_roomba(141);
	send_byte_roomba(3);
	my_msleep(MEASURE_TIME*1500);
	
	send_byte_roomba(141);
	send_byte_roomba(4);
	my_msleep(MEASURE_TIME*2500);
	
	send_byte_roomba(141);
	send_byte_roomba(2);
	my_msleep(MEASURE_TIME*2500);
	
	send_byte_roomba(141);
	send_byte_roomba(3);
	my_msleep(MEASURE_TIME*2000);
	*/
}

