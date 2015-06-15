#include <tools.h>

void my_msleep ( uint16_t ms )
{

    for ( uint16_t i = 0; i < ms; i++ ) {
        _delay_loop_2 ( 4000 );
    }
}

void initializeTimer ( uint16_t ms )
{
    uint16_t timerVal = ms/1000 * 15625;
    TCCR1B = 0x0D;
    OCR1AH = ( timerVal & 0xFF00 ) >> 8;
    OCR1AL = timerVal & 0x00FF;
    TIMSK1 = 0x03;
}

uint16_t concat_bytes ( uint8_t high, uint8_t low )
{
    uint16_t high16 = high << 8;
    return ( high16 | low );
}

int16_t concat_bytes_signed ( int8_t high, int8_t low )
{
    int16_t high16 = high << 8;
    return ( high16 | low );
}

void split_bytes ( int16_t value, int8_t* result )
{
    result[0] = value >> 8;
    result[1] = value;
}

void number2String ( uint16_t value, char* result )
{
    sprintf ( result, "%i", value );
}

void number2StringSigned ( int16_t value, char* result )
{
    sprintf ( result, "%i", value );
}

void number2Hex ( uint16_t value, char* result )
{
    sprintf ( result, "%x", value );
}

void string_concat ( char* name, char* value, char* result )
{
    sprintf ( result, "%s : %s", name, value );
}
int16_t rand_tools(int16_t start, int16_t end){
  return rand()%(end-start + 1) + start;
}
int16_t mod(int16_t value, int16_t div){

  return value > 0 ? value % div : -((-value) % div);
}