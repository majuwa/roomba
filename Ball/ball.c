#include "ball.h"
#include "../lib/tools.h"
#include "../lib/roomba.h"
#include "../lib/usart.h"
#include <stdlib.h>
#include "../lib/RFM12B.h"
#include "../lib/music.h"

int16_t angle = 0;
char points_goal_0 = '0';
char points_goal_1= '0';
char max_points = 0;


void update_angle()
{
        int16_t angle_change = get_angle();
        angle = mod ( ( angle + angle_change ) , 360 );
}

char waitForAck ( uint8_t receiver )
{
        my_msleep ( 50 );
        if ( ACKReceived ( receiver ) )
                return 1;
        return 0;
}

void send_start_points()
{
        uint8_t sendSize = 2;
        char requestACK = 1;
        char payload[2] = {0};
        payload[0] = points_goal_0;
        Send ( 2, payload, sendSize + 1, requestACK );
        if ( waitForAck ( 2 ) );
        my_msleep ( 200 );
        Send ( 3, payload, sendSize + 1, requestACK );
        if ( waitForAck ( 3 ) );
}


void send_user_data ( )
{
        uint8_t goal_id;
        uint8_t sendSize = 2;
        char requestACK = 1;
        char payload[2] = {0};
        update_angle();

        if ( angle < 90 || angle > 270 ) {
                payload[0] = ++points_goal_0;
                goal_id = 2;
        } else {
                payload[0] = ++points_goal_1;
                goal_id = 3;
        }

        Send ( goal_id, payload, sendSize + 1, requestACK );
        if ( waitForAck ( goal_id ) );
}


int16_t get_angle()
{
        int8_t data_byte[2];
        int16_t angle_value;
        read_values_signed ( 20, data_byte, 2 );
        angle_value = concat_bytes_signed ( data_byte[0], data_byte[1] ) * 3;
        return ( angle_value < 0 ) ? ( 360 + angle_value ) : angle_value;
}

void initialize_ball()
{
        Initialize ( 1,20 );
        save_songs();
        send_start_points();
        get_angle();
        srand ( coder_values_r() );
        turn2 ( rand_tools ( -20, 20 ) , 300 );
}

int8_t check_cliff ( uint16_t cliffL, uint16_t cliffR )
{
        int16_t difference = ( int16_t ) ( cliffL - cliffR );

        // no line
        if ( cliffL < 1200 && cliffR < 1200 )
                return NO_CLIFF;

        // line on left side
        if ( difference > 300 )
                return CLIFF_L;
        // line on right side
        else if ( difference < -300 )
                return CLIFF_R;
        // bad timing for reading cliffs -> both cliffs over line
        else if ( cliffL > 1200 || cliffR > 1200 )
                return CLIFF_L; // could also use CLIFF_R here
        else
                return NO_CLIFF;
}


int16_t calc_new_angle ( int8_t cliff )
{
        update_angle();

        if ( cliff == CLIFF_L )
                return - ( 2 * ( angle % 90 + rand_tools ( -5,5 ) ) );
        else if ( cliff == CLIFF_R )
                return 2 * ( 90 - ( angle % 90  + rand_tools ( -5,5 ) ) );
        else
                return 0;
}


int16_t check_lightbump ( uint16_t* value )
{
        // center lightbumps
        if ( value[2] > 600 || value[3] > 600 )
                return rand_tools ( 150,210 );
        // front left
        else if ( value[1] > 400 )
                return -rand_tools ( 120, 160 );
        // front right
        else if ( value[4] >  400 )
                return rand_tools ( 120,160 );
        // left
        else if ( value[0] > 600 )
                return -rand_tools ( 90,120 );
        // right
        else if ( value[5] > 400 )
                return rand_tools ( 90,120 );
        // no lightbump
        else
                return 0;
}


void drive_ball ()
{
        // cliff front left and right; all 6 light bumpers; normal bumpers
        uint8_t packetIDs [] = {29,30,46,47,48,49,50,51,7};
        uint8_t datas [17];

        update_angle();
        multiple_sensors ( packetIDs, datas, 17, 9 );

        //check lightbumpers for hitting pong
        uint16_t light_bumpers[6];
        for ( int8_t i = 0; i < 6; i++ ) {
                light_bumpers[i] = concat_bytes ( datas[ ( 2 * i ) + 4], datas[ ( 2 * i ) + 5] );
        }

        handle_lightbumpers ( light_bumpers );

        // check cliffs for crossing side or goal line
        uint16_t cliffs[2];
        for ( int8_t i = 0; i < 2; i++ ) {
                cliffs[i] = concat_bytes ( datas[2 * i], datas[ ( 2 * i ) + 1] );
        }

        int8_t cliff = check_cliff ( cliffs[0], cliffs[1] );
        handle_cliff ( cliff );

        // emergency stop
        if ( datas[16] & BUMP_RIGHT || datas[16] & BUMP_LEFT )
                stop();
}

void handle_lightbumpers ( uint16_t* light_bumpers )
{
        // new angle for bouncing of pong roombas
        int16_t pong_angle = check_lightbump ( light_bumpers );
        if ( pong_angle ) {
                stop();
                turn2 ( pong_angle, 100 );
                drive ( BALL_SPEED );
        }
}

void handle_cliff ( uint8_t cliff )
{
        uint8_t virtual_wall = 0;

        if ( cliff ) {

                // check for virtual wall signal -> side line
                int8_t i = 0;
                while ( ( i < IR_TEST_VALUE ) && ( !virtual_wall ) ) {
                        read_values ( 13, &virtual_wall, 1 );
                        i++;
                }

                // no virtual wall -> goal
                if ( !virtual_wall ) {
                        start_after_goal();
                        return;
                }

                // new angle for bouncing off side
                int16_t new_angle = calc_new_angle ( cliff );

                if ( new_angle ) {
                        stop();
                        turn2 ( new_angle, 100 );
                        drive ( BALL_SPEED );
                }
        }
}

void start_after_goal()
{
        char* string1 = "GOAL";
        set_Display ( string1 );
        stop();
        send_user_data ();
        play_songs();

        my_msleep ( 1000 );
        check_win();

        user_prepare_serve();
        turn2 ( rand_tools ( -30,30 ),100 );
        drive ( BALL_SPEED );
        set_Display ( "    " );
}


void user_prepare_serve()
{
        int16_t tmp_angle = angle;

        // user must turn ball-roomba in a straight position for serve
        do {
                if ( read_button ( BUTTON_SPOT ) )
                        drive_circle ( -1, 100 );
                else if ( read_button ( BUTTON_DOCK ) )
                        drive_circle ( 1, 100 );
                else
                        stop();
        } while ( !read_button ( BUTTON_CLEAN ) );

        update_angle();

        if ( tmp_angle<90 || tmp_angle > 270 )
                angle = 180;
        else
                angle = 0;
}


void check_win()
{
        if ( points_goal_0 >= max_points ) {
                display_win ( "PL-1" );
        } else if ( points_goal_1 >= max_points ) {
                display_win ( "PL-2" );
        } else
                return;
}

void display_win ( char* winner )
{
        do {
                set_Display ( winner );
                my_msleep ( 1000 );
                set_Display ( "WINS" );
                my_msleep ( 1000 );
        } while ( 1 );
}

void set_max_points ()
{
        max_points = read_user_input_single();
        set_Display ( "OK  " );
        my_msleep ( 1000 );
        set_Display ( "   " );
}


