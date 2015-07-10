#include "pong.h"
#include "../lib/roomba.h"
#include "../lib/tools.h"
#include "../lib/RFM12B.h"
#include <string.h>

int16_t max_r;
int16_t max_l;
uint16_t start_r;
uint16_t start_l;
uint16_t end_r;
uint16_t end_l;

int8_t moving = 0;
char my_points[2] = {0};

const float kp = 0.5;


void initialize_pong()
{
        start_r = coder_values_r();
        start_l = coder_values_l();

        uint8_t ir_value;
        set_Display ( "INIT" );

        do {
                read_values ( 17, &ir_value, 1 );
                uint16_t value_r = coder_values_r();
                uint16_t value_l = coder_values_l();

                int16_t diff_r = ( int16_t ) ( value_r - start_r );
                int16_t diff_l = ( int16_t ) ( value_l - start_l );

                if ( ir_value == IR_UP ) {
                        drive_pong_straight ( diff_r, diff_l, 1 );
                } else if ( ir_value != IR_UP ) {
                        drive_pong_straight ( diff_r, diff_l, 0 );
                }
        } while ( ir_value != IR_RIGHT );

        end_r = coder_values_r();
        end_l = coder_values_l();

        max_r = ( int16_t ) ( end_r - start_r );
        max_l = ( int16_t ) ( end_l - start_l );
        set_Display ( "BALL" );
}


void drive_pong()
{
        uint16_t value_r = coder_values_r();
        uint16_t value_l = coder_values_l();

        int16_t diff_r = ( int16_t ) ( value_r - start_r );
        int16_t diff_l = ( int16_t ) ( value_l - start_l );

        uint8_t ir_value;
        read_values ( 17, &ir_value, 1 );

        if ( diff_r > max_r || diff_l > max_l ) {
                stop();
                my_msleep ( 200 );
                drive_roomba_clicks ( PONG_RESET, -PONG_SPEED );
        } else if ( diff_r < 0 || diff_l < 0 ) {
                stop();
                my_msleep ( 200 );
                drive_roomba_clicks ( PONG_RESET, PONG_SPEED );
        } else {
                int8_t direction = 0;

                if ( ir_value == IR_UP )
                        direction = 1;
                else if ( ir_value == IR_DOWN )
                        direction = -1;

                drive_pong_straight ( diff_r, diff_l, direction );
        }

        getData();
}

void drive_pong_straight ( int16_t diff_r, int16_t diff_l, int8_t direction )
{
        int16_t difference = diff_l - diff_r;

        int16_t vel_l = PONG_SPEED;
        int16_t vel_r = PONG_SPEED;

        switch ( direction ) {
        case 0:
                if ( moving ) {
                        stop();
                        moving = 0;
                }
                break;

        case 1:
                vel_l -= difference * kp;
                vel_r += difference * kp;
                drive_wheels ( vel_r, vel_l );
                moving = 1;
                break;

        case -1:
                vel_l += difference * kp;
                vel_r -= difference * kp;
                drive_wheels ( -vel_r, -vel_l );
                moving = -1;
                break;

        default:
                stop();
                moving = 0;
                break;
        }
        my_msleep ( 150 );
}

void getData()
{
        if ( ReceiveComplete() ) {
                if ( CRCPass() ) {
                        memcpy ( my_points,Data,2 );
                        set_Display ( my_points );
                        if ( ACKRequested() ) {
                                SendACK();
                        }
                }
        }
}


void setID()
{
        uint8_t id_2;
        uint8_t id_3;

        do {
                if ( read_button ( BUTTON_MINUTE ) ) {
                        id_2 = 1;
                        id_3 = 0;
                        set_Display ( "PL-1" );
                } else if ( read_button ( BUTTON_HOUR ) ) {
                        id_3 = 1;
                        id_2 = 0;
                        set_Display ( "PL-2" );
                }
        } while ( !read_button ( BUTTON_CLEAN ) ||  ! ( id_3 | id_2 ) );

        if ( id_2 ) {
                Initialize ( 2,20 );
        } else {
                Initialize ( 3,20 );
        }
}
