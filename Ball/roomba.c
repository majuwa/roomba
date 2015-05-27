#include <roomba.h>
#include <usart.h>
#include <tools.h>
#include <math.h>
#include <stdlib.h>

void roomba_init()
{
    send_byte_roomba ( 128 );
    send_byte_roomba ( 131 );
}

void set_Display ( char *data )
{
    send_byte_roomba ( 164 );
    send_byte_roomba ( *data );
    send_byte_roomba ( * ( data+1 ) );
    send_byte_roomba ( * ( data+2 ) );
    send_byte_roomba ( * ( data+3 ) );
}

uint8_t read_button ( uint8_t button )
{
    send_byte_roomba ( 142 );
    send_byte_roomba ( 18 );
    uint8_t result = receive_byte_roomba();
    return ( button & result ) == button;
}

void read_values ( uint8_t packetid, uint8_t * data, uint8_t length )
{
    send_byte_roomba ( 142 );
    send_byte_roomba ( packetid );
    for ( uint8_t i = 0; i <  length; i++ ) {
        data[i] = receive_byte_roomba();
    }
}

void read_values_signed ( uint8_t packetid, int8_t* data, uint8_t length )
{
    send_byte_roomba ( 142 );
    send_byte_roomba ( packetid );
    for ( uint8_t i = 0; i <  length; i++ ) {
        data[i] = receive_byte_roomba();
    }
}

void drive ( int16_t velocity )
{
    drive_wheels ( velocity, velocity );
}

void drive_wheels ( int16_t velocityRight, int16_t velocityLeft )
{
    int8_t velR [2];
    int8_t velL [2];
    split_bytes ( velocityRight, velR );
    split_bytes ( velocityLeft, velL );
    send_byte_roomba ( 145 );
    send_byte_roomba ( velR[0] );
    send_byte_roomba ( velR[1] );
    send_byte_roomba ( velL[0] );
    send_byte_roomba ( velL[1] );
}

void stop()
{
    send_byte_roomba ( 145 );
    for ( uint8_t i = 0; i < 2; i++ ) {
        send_byte_roomba ( ( uint8_t ) 0 );
        send_byte_roomba ( ( uint8_t ) 0 );
    }
}

void drive_roomba ( uint16_t distance, int16_t velocity )
{
    drive ( velocity );
    if ( velocity < 0 ) {
        velocity = -velocity;
    }
    uint16_t time = distance/velocity;
    my_msleep ( time*900 );
    stop();
}

void drive_roomba_clicks ( uint16_t distance, int16_t velocity )
{

    uint16_t clicks_finish = distance * CLICKS_PER_MM;

    uint8_t ticks_r [2];
    read_values ( 43, ticks_r, 2 );

    uint16_t counter = 0;
    uint16_t start = concat_bytes ( ticks_r[0], ticks_r[1] );

    drive ( velocity );

    my_msleep ( 200 );

    while ( counter < clicks_finish ) {
        read_values ( 43, ticks_r, 2 );
        if ( velocity < 0 ) {
            counter = start - concat_bytes ( ticks_r[0], ticks_r[1] );
        } else {
            counter = concat_bytes ( ticks_r[0], ticks_r[1] ) - start;
        }
    }
    stop();
}

void drive_roomba_code ( uint8_t ir_code, int16_t velocity )
{
    int8_t velocityB [2];
    int8_t radiusB [2];

    switch ( ir_code ) {
    case IR_UP:
        split_bytes ( velocity, velocityB );
        int16_t rad1 = 32767;
        split_bytes ( rad1, radiusB );
        break;
    case IR_DOWN:
        split_bytes ( ( -velocity ), velocityB );
        int16_t rad2 = 32767;
        split_bytes ( rad2, radiusB );
        break;
    case IR_RIGHT:
        split_bytes ( velocity, velocityB );
        int16_t rad3 = -1;
        split_bytes ( rad3, radiusB );
        break;
    case IR_LEFT:
        split_bytes ( velocity, velocityB );
        int16_t rad4 = 1;
        split_bytes ( rad4, radiusB );
        break;
    case IR_ON_OFF:
        stop();
        return;
    }
    send_byte_roomba ( 137 );
    send_byte_roomba ( velocityB[0] );
    send_byte_roomba ( velocityB[1] );
    send_byte_roomba ( radiusB[0] );
    send_byte_roomba ( radiusB[1] );
}

uint16_t read_user_input ( void )
{
    char string[] = "----";
    set_Display ( string );

    uint16_t return_value = 0;
    uint8_t ir_value;
    uint8_t count = 0;
    do {
        read_values ( 17, &ir_value, 1 );
        if ( IR_0 <= ir_value && ir_value <= ( IR_0 + 10 ) && count < 4 ) {
            string[count] = ir_value - IR_0 + '0';
            set_Display ( string );
            return_value *= 10;
            return_value += ir_value - IR_0;
            my_msleep ( 200 );
            count++;
        }
    } while ( ir_value != IR_RIGHT );

    set_Display ( "COOL" );

    return return_value;
}

void drive_circle ( uint16_t radius, uint16_t velocity )
{
    int16_t rad = radius*10;
    //rad = (rad+40)/2;
    int8_t velocityB [2];
    int8_t radiusB [2];
    split_bytes ( velocity, velocityB );
    split_bytes ( rad, radiusB );

    send_byte_roomba ( 137 );
    send_byte_roomba ( velocityB[0] );
    send_byte_roomba ( velocityB[1] );
    send_byte_roomba ( radiusB[0] );
    send_byte_roomba ( radiusB[1] );

}

void drive_circle_time ( uint16_t radius, uint16_t velocity, uint16_t time )
{
    drive_circle ( radius, velocity );
    my_msleep ( time*1000 );
    stop();
}

void drive_rectangle ( uint16_t length, uint16_t width )
{
    my_msleep ( 200 );
    drive_roomba_clicks ( length, 200 );
    my_msleep ( 200 );
    turn ( 90 );
    my_msleep ( 200 );
    drive_roomba_clicks ( width, 200 );
    my_msleep ( 200 );
    turn ( 90 );
    my_msleep ( 200 );
    drive_roomba_clicks ( length, 200 );
    my_msleep ( 200 );
    turn ( 90 );
    my_msleep ( 200 );
    drive_roomba_clicks ( width, 200 );
    my_msleep ( 200 );
    turn ( 90 );
    my_msleep ( 200 );
}

void turn ( int16_t degree )
{
    int8_t velocity [2];
    int8_t radiusB [2];

    split_bytes ( 100, velocity );

    int16_t posDeg;

    if ( degree < 0 ) {
        posDeg = -degree;
        split_bytes ( -1, radiusB );
    } else {
        posDeg = degree;
        split_bytes ( 1, radiusB );
    }

    const float radValue = M_PI / 180;
    uint16_t wheel_distance = CLICKS_PER_MM * WHEEL_2_CENTER * posDeg * radValue;

    uint8_t ticks_r [2];
    read_values ( 43, ticks_r, 2 );

    uint16_t counter = 0;
    uint16_t start = concat_bytes ( ticks_r[0], ticks_r[1] );

    send_byte_roomba ( 137 );
    send_byte_roomba ( velocity[0] );
    send_byte_roomba ( velocity[1] );
    send_byte_roomba ( radiusB[0] );
    send_byte_roomba ( radiusB[1] );

    my_msleep ( 200 );

    while ( counter < wheel_distance ) {
        read_values ( 43, ticks_r, 2 );
        if ( degree > 0 ) {
            counter = start - concat_bytes ( ticks_r[0], ticks_r[1] );
        } else {
            counter = concat_bytes ( ticks_r[0], ticks_r[1] ) - start;
        }
    }

    stop();
}

uint8_t backup_collision ( uint16_t degree, uint8_t bump )
{

    if ( ( bump & BUMP_RIGHT ) == BUMP_RIGHT ) {
        stop();
        my_msleep ( 200 );
        drive_roomba_clicks ( 100, -150 );
        my_msleep ( 200 );
        turn ( degree );
        my_msleep ( 200 );
        return 1;
    } else if ( ( bump & BUMP_LEFT ) == BUMP_LEFT ) {
        stop();
        my_msleep ( 200 );
        drive_roomba_clicks ( 100, -150 );
        my_msleep ( 200 );
        turn ( -degree );
        my_msleep ( 200 );
        return 1;
    } else {
        return 0;
    }
}

void multiple_sensors ( uint8_t* packetIDs, uint8_t* datas, uint8_t dataSize, uint8_t packetCount )
{
    send_byte_roomba ( 149 );
    send_byte_roomba ( packetCount );
    for ( uint8_t i = 0; i < packetCount; i++ ) {
        send_byte_roomba ( packetIDs[i] );
    }
    for ( uint8_t j = 0; j < dataSize; j++ ) {
        datas[j] = receive_byte_roomba();
    }
}

uint8_t p_follow_line ( uint16_t cliffLeft, uint16_t cliffRight, int16_t velocity )
{
    float kp = 0.2;

    char test[6];
    int16_t difference = ( int16_t ) ( cliffLeft - cliffRight );
    int16_t velLeft = velocity;
    int16_t velRight = velocity;

    if ( abs ( difference ) > 200 ) {
        velLeft -= difference * kp;
        velRight += difference * kp;
        drive_wheels ( velRight, velLeft );
        return 1;
    }

    number2Hex ( difference, test );
    set_Display ( test );
    return 0;
}

uint8_t pid_follow_line ( uint16_t cliffLeft, uint16_t cliffRight, int16_t velocity )
{
    float kp = 0.2;
    float ki = 0.3;
    float kd = 0.0;

    static int16_t esum = 0;
    static int16_t ealt = 0;

    char test[6];
    int16_t difference = ( int16_t ) ( cliffLeft - cliffRight );
    int16_t velLeft = velocity;
    int16_t velRight = velocity;
    int16_t y;

    if ( abs ( difference ) > 200 ) {
        esum += difference;
        y = kp * difference + ki * 0.03 * esum + kd * ( difference - ealt ) /0.03;
        velLeft -= y;
        velRight += y;
        drive_wheels ( velRight, velLeft );
        ealt = difference;
        return 1;
    }

    number2Hex ( difference, test );
    set_Display ( test );
    return 0;
}

