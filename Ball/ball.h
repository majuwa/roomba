#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED
#include <avr/io.h>

#define WALL_VALUE 200
#define CLIFF_L -1
#define CLIFF_R 1
#define NO_CLIFF 0

#define BALL_SPEED 400

#define LIGHT_BUMPER_LEFT 1
#define LIGHT_BUMPER_FRONT_LEFT 2
#define LIGHT_BUMPER_CENTER_LEFT 4
#define LIGHT_BUMPER_CENTER_RIGHT 8
#define LIGHT_BUMPER_FRONT_RIGHT 16
#define LIGHT_BUMPER_RIGHT 32
#define IR_TEST_VALUE 10
/**
 * send the initial points to all pongs
 */
void send_start_points ( void );
/**
 * Get the current angle in the game
 * @return int16 angle in degree
 */
int16_t get_angle ( void );
/**
 * Initialation of ball
 * -> initialize radio
 * -> reset angle
 * -> set the sounds
 * -> turn a random angle
 */
void initialize_ball ( void );
/**
 * Checks if there is a line on the floor
 * @param cliffL the value of the left cliff sensor
 * @param cliffR the value of the right cliff sensor
 * @return NO_CLIFF for line, CLIFF_L line on the left, CLIFF_R line on the right
 */
int8_t check_cliff ( uint16_t cliffL, uint16_t cliffR );

/**
 * Calculates the angle the roomba should bounce of
 * @param cliff NO_CLIFF, or CLIFF_L, or CLIFF_R
 * @return bounce of angle in degree
 */
int16_t calc_new_angle ( int8_t cliff );
/**
 * Moves the ball and does all necessary checking
 */
void drive_ball ( void );

/**
 * Collision detection with pong.
 * @param value values of the light-bumper 6 uint16 value in order from left to right for more details look at the roomba manual
 * @return 0 if no pong else a value != 0 with a angle in degree. the angle is calcuted according to the sensor values and little random offset
 */
int16_t check_lightbump ( uint16_t* value );
/**
 * Set Display to "Goal" increase the points for one users, check if game was won and if yes stops the game
 */
void start_after_goal ( void );
/**
 * Checks after a goal if the maximum score was reached by one player
 */
void check_win ( void );
/**
 * Displays win of one player on Roomba display
 * @param winner char-string of the player who won, e.g. "PL-1" or "PL-2"
 */
void display_win ( char* winner );
/**
 * Prepare ball for serve after a goal. User must turn Roomba into a straight position with SPOT and DOCK buttons
 */
void user_prepare_serve ( void );
/**
 * Reads angle turned since last call and updates intern angle counter
 */
void update_angle ( void );
/**
 * Sends new score of user to pong-Roombas after a goal
 */
void send_user_data ( void );
/**
 * Turns if side line was detected or counts a goal if goal line was detected
 * @param cliff the cliff-value calculated in check_cliff(...) ("CLIFF_L", "CLIFF_R", "NO_CLIFF")
 */
void handle_cliff ( uint8_t cliff );
/**
 * Turns if pong was detected
 * @param light_bumpers the light bumper values
 */
void handle_lightbumpers ( uint16_t* light_bumpers );
/**
 * Set the max points to win the game max '9' via IR remote
 */
void set_max_points ( void );

#endif
