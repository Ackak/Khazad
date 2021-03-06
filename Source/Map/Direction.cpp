#include <Direction.h>

uint8_t const Direction::CARDINAL_DIRECTIONS[NUM_CARDINAL_DIRECTIONS] = {DIRECTION_NORTH, DIRECTION_SOUTH, DIRECTION_EAST, DIRECTION_WEST};
uint8_t const Direction::COMPASS_DIRECTIONS[NUM_COMPASS_DIRECTIONS] = {DIRECTION_NORTH, DIRECTION_NORTHEAST, DIRECTION_EAST, DIRECTION_SOUTHEAST, DIRECTION_SOUTH, DIRECTION_SOUTHWEST, DIRECTION_WEST, DIRECTION_NORTHWEST};
uint8_t const Direction::AXIAL_DIRECTIONS[NUM_AXIAL_DIRECTIONS] = {DIRECTION_UP, DIRECTION_DOWN, DIRECTION_NORTH, DIRECTION_SOUTH, DIRECTION_EAST, DIRECTION_WEST};
uint8_t const Direction::ANGULAR_DIRECTIONS[NUM_ANGULAR_DIRECTIONS] = {DIRECTION_UP, DIRECTION_DOWN, DIRECTION_NORTH, DIRECTION_UP_NORTH, DIRECTION_DOWN_NORTH, DIRECTION_SOUTH, DIRECTION_UP_SOUTH, DIRECTION_DOWN_SOUTH, DIRECTION_EAST, DIRECTION_UP_EAST, DIRECTION_DOWN_EAST, DIRECTION_NORTHEAST, DIRECTION_UP_NORTHEAST, DIRECTION_DOWN_NORTHEAST, DIRECTION_SOUTHEAST, DIRECTION_UP_SOUTHEAST, DIRECTION_DOWN_SOUTHEAST, DIRECTION_WEST, DIRECTION_UP_WEST, DIRECTION_DOWN_WEST, DIRECTION_NORTHWEST, DIRECTION_UP_NORTHWEST, DIRECTION_DOWN_NORTHWEST, DIRECTION_SOUTHWEST, DIRECTION_UP_SOUTHWEST, DIRECTION_DOWN_SOUTHWEST};

