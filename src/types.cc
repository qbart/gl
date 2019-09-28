#include "types.hh"

v4 rgb(byte r, byte g, byte b, byte a)
{
	return v4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}
