#ifndef DIRECTION__HEADER
#define DIRECTION__HEADER

#include <stdint.h>

enum Axis
{
    AXIS_Z,   // Vertical
    AXIS_Y,   // North/South
    AXIS_X,   // East/West

    NUM_AXIS,
    AXIS_START = 0
};

inline Axis &operator++ (Axis &OldAxis)      { return OldAxis = Axis(OldAxis + 1); }
inline Axis &operator-- (Axis &OldAxis)      { return OldAxis = Axis(OldAxis - 1); }

// BitPacking  XX 0 YY 0 ZZ
//             76 5 43 2 10

#define ONEMASK 18
#define ZEROMASK 219
#define XORMASK 73

#define DIRECTION_NONE 0					// Index -1
#define DIRECTION_UP 1						// Index 0
#define DIRECTION_DOWN 2					// Index 1

#define DIRECTION_NORTH 8					// Index 2
#define DIRECTION_UP_NORTH 9				// Index 3
#define DIRECTION_DOWN_NORTH 10				// Index 4

#define DIRECTION_SOUTH 16					// Index 5
#define DIRECTION_UP_SOUTH 17				// Index 6
#define DIRECTION_DOWN_SOUTH 18				// Index 7

#define DIRECTION_EAST 64					// Index 8
#define DIRECTION_UP_EAST 65				// Index 9
#define DIRECTION_DOWN_EAST 66				// Index 10

#define DIRECTION_NORTHEAST 72				// Index 11
#define DIRECTION_UP_NORTHEAST 73			// Index 12
#define DIRECTION_DOWN_NORTHEAST 74			// Index 13

#define DIRECTION_SOUTHEAST 80				// Index 14
#define DIRECTION_UP_SOUTHEAST 81			// Index 15
#define DIRECTION_DOWN_SOUTHEAST 82			// Index 16

#define DIRECTION_WEST 128					// Index 17
#define DIRECTION_UP_WEST 129				// Index 18
#define DIRECTION_DOWN_WEST 130				// Index 19

#define DIRECTION_NORTHWEST 136				// Index 20
#define DIRECTION_UP_NORTHWEST 137			// Index 21
#define DIRECTION_DOWN_NORTHWEST 138		// Index 22

#define DIRECTION_SOUTHWEST 144				// Index 23
#define DIRECTION_UP_SOUTHWEST 145			// Index 24
#define DIRECTION_DOWN_SOUTHWEST 146		// Index 25

#define DIRECTION_UNKNOWN 36

#define NUM_CARDINAL_DIRECTIONS 4
#define NUM_COMPASS_DIRECTIONS 8
#define NUM_AXIAL_DIRECTIONS 6
#define NUM_ANGULAR_DIRECTIONS 26

struct Direction
{

	Direction()
	{
		Data = DIRECTION_UNKNOWN;
	};

	Direction(uint8_t Rawvalue)
	{
		Data = Rawvalue;
	};

	inline void rectify() // Rectifies 11 values back into 00
	{
		Data &= ZEROMASK;

		uint8_t Temp1 = Data & XORMASK;
		uint8_t Temp2 = (~(Data ^ (Data >> 1))) & XORMASK;

		Data = (Data + (Temp1 & Temp2)) & ZEROMASK;
	};

	inline Direction& Invert()
	{
		Direction newDir = Direction(~Data); //Flip all bits, this will turn 00 into 11
		newDir.rectify();
		return newDir;
	};

	inline const Direction operator+(const Direction& OtherDirection)
	{
		Direction newDir = *this;
		newDir.Data &= OtherDirection.Data;
		newDir.rectify();
		return newDir;
	};

	inline Direction& operator++(int)
	{
		return *this;
	};

	inline bool operator==(const Direction& OtherDirection)
	{
		return OtherDirection.Data == Data;
	};

	inline bool operator!=(const Direction& OtherDirection)
	{
		return OtherDirection.Data != Data;
	};

	inline int ValueonAxis(Axis QuerryAxis)
	{
		int temp = (Data >> ((int)QuerryAxis) * 3) & 3;
		if (temp == 1)
			return 1;
		if (temp == 2)
			return -1;
		return 0;
	};

	inline bool Positive()
	{
		int temp = Data;
		int read = temp & 3;

		if (read == 1)
			return true;
		if (read == 2)
			return false;

		temp >>= 3;
		read = temp & 3;

		if (read == 1)
			return true;
		if (read == 2)
			return false;

		temp >>= 3;
		read = temp & 3;

		if (read == 1)
			return true;
		if (read == 2)
			return false;
	};

	inline uint8_t Index()
	{
		uint8_t Z = (Data & 3);
		uint8_t Y = (((Data >> 3) & 3) * 3);
		uint8_t X = (((Data >> 6) & 3) * 9);

		return X + Y + Z - 1;
	};

    uint8_t Data;

	static uint8_t const CARDINAL_DIRECTIONS[];
	static uint8_t const COMPASS_DIRECTIONS[];
	static uint8_t const AXIAL_DIRECTIONS[];
	static uint8_t const ANGULAR_DIRECTIONS[];
};

#endif // DIRECTION__HEADER
