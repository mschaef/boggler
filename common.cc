/*
 * common.cc - A set of commonly useful utility functions
 * by Michael Schaeffer
 */

#include "common.h"

/* Produce a random integer in the range [0, limit) */
int limited_random(int limit)
{
  return (rand() % limit);
}

/* Skip all whitespace on the input stream, leaving it positioned
 * at the first non-whitespace character. */
void skip_whitespace(istream &i)
{
  for(;;) {
       int ch = i.get();

       if (!isspace(ch)) {
            i.putback(ch);
            return;
       }
  }
}

/* Return TRUE if the character `ch_expected` was found on the stream. The
 * character will be consumed. */
int expect(istream &i, char ch_expected)
{
  char ch;
  
  i.get(ch);

  return ch == ch_expected;
}

void message(const char *prefix, const char *msg)
{
  cerr << prefix << msg << endl;
}

void error(const char *msg)
{
  cerr << "Error: " << msg << endl;

  exit(1);
}

void warn(const char *msg)
{
  cerr << "Warning: " << msg << endl;
}
