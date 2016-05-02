#pragma once

#include <cctype>
#include "types.hpp"

static inline u32 bytes2word(u8 *bytes)
{
	return
		(bytes[0] << 24) |
		(bytes[1] << 16) |
		(bytes[2] <<  8) |
		(bytes[3] <<  0);
}

static inline void word2bytes(u32 word, u8 *bytes)
{
	bytes[0] = word >> 24;
	bytes[1] = (word >> 16) & 0xff;
	bytes[2] = (word >>  8) & 0xff;
	bytes[3] = word & 0xff;
}

static inline void bytes_add(u8 *bytes, u32 word)
{
	word2bytes(bytes2word(bytes) + word, bytes);
}

static inline void cstring_to_upper_case(char *str)
{
	for (int i = 0; str[i]; i++)
		str[i] = toupper(str[i]);
}
