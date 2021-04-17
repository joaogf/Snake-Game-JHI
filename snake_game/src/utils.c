#include "utils.h"

int num_of_digits(int num)
{
	int digits = 1;

	while(num > 0)
	{
		num /= 10;
		digits++;
	}

	return digits;
}

JHI_Keys convert_joy_to_key(JHI_JOY_DIR_ dir)
{
	switch(dir)
	{
		case JOY_RIGHT:
			return KEY_RIGHT;
		case JOY_LEFT:
			return KEY_LEFT;
		case JOY_UP:
			return KEY_UP;
		case JOY_DOWN:
			return KEY_DOWN;
	}

	return NO_KEY;
}
