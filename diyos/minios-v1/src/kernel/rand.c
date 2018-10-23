#include <rand.h>

static int seed = 1;
void rand_seed(int value)
{
	seed = value;
}
/* This algorithm is mentioned in the ISO C standard, here extended
   for 32 bits.  */
static int rand_r (unsigned int *seed)
{
  unsigned int next = *seed;
  int result;

  next *= 1103515245;
  next += 12345;
  result = (unsigned int) (next / 65536) % 2048;

  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (unsigned int) (next / 65536) % 1024;

  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (unsigned int) (next / 65536) % 1024;

  *seed = next;

  return result;
}

int rand()
{
	return rand_r (&seed);
}
