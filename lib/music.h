#ifndef MUSIC_H_INCLUDED
#define MUSIC_H_INCLUDED

#include "usart.h"
#include <avr/io.h>
#include "tools.h"

#define R 30

#define C4 60
#define CIS4 61
#define D4 62
#define DIS4 63
#define E4 64
#define F4 65
#define FIS4 66
#define G4 67
#define GIS4 68
#define A4 69
#define AIS4 70
#define B4 71
#define C5 72
#define CIS5 73
#define D5 74
#define DIS5 75
#define E5 76
#define F5 77
#define FIS5 78
#define G5 79
#define GIS5 80
#define A5 81
#define AIS5 82
#define B5 83
#define C6 84
#define CIS6 85
#define D6 86
#define DIS6 87
#define E6 88
#define F6 89
#define FIS6 90

#define MEASURE 160
#define HALF MEASURE/2
#define Q MEASURE/4
#define E MEASURE/8
#define ED MEASURE*3/16
#define S MEASURE/16
 
#define MEASURE_TIME MEASURE/64

void save_songs(void);

void play_songs(void);

#endif
