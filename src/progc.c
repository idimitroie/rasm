#include "progc.h"

static unsigned program_counter;

unsigned get_pc(void)
{
	return program_counter;
}

#define CELL (4)

unsigned int advance_half_word(void)
{
	return program_counter+=(CELL/2);
}

unsigned int advance_word(void)
{
	return program_counter+=CELL;
}

void reset_pc(void)
{
  program_counter = 0;
}
