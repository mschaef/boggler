/****************************************************************
 * common.cc - A set of commonly useful utility functions
 * by Michael Schaeffer
 *
 * Revision History:
 * 12/31/1996 - File created
 * 1/2/1997 - Added error handling routines
 ****************************************************************/

#include "common.h"

/****************************************************************
 * int limited_random(int)
 *
 * Produce a random integer in the range [0, limit)
 ****************************************************************/
int limited_random(int limit)
{
  return (rand() % limit);
}

/****************************************************************
 * void skip_whitespace(istream &)
 *
 * Produce a random integer in the range [0, limit)
 ****************************************************************/
void skip_whitespace(istream &i)
{
  char ch = ' ';

  while (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r')
    i.get(ch);

  i.putback(ch);
}


/****************************************************************
 * int expect(istream &, char)
 *
 * Return TRUE if the character was found on the stream. Destroys
 * the character
 ****************************************************************/
int expect(istream &i, char ch_expected)
{
  char ch;
  
  i.get(ch);

  return ch == ch_expected;
}



/****************************************************************
 * void message(char *, char*);
 *
 * Write a prefixed message out to cerr
 ****************************************************************/
void message(char *prefix, char *msg){
  cerr << prefix << msg << '\n';
}

/****************************************************************
 * void error(char *);
 *
 * Issue a fatal error message and abort
 ****************************************************************/
void error(char *msg)
{
  message("Error: ", msg);
  exit(1);
}

/****************************************************************
 * void warn(char *);
 *
 * Issue a warning message
 ****************************************************************/
void warn(char *msg)
{
  message("Warning: ", msg);
}
