/****************************************************************
 * common.h - A set of commonly useful utility functions
 * by Michael Schaeffer
 *
 * Revision History:
 * 12/31/1996 - File created
 * 1/2/1997 - Added error handling routines, and included time.h
 ****************************************************************/

#ifndef __COMMON_H
#define __COMMON_H

#include <getopt.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int FALSE = 0;
const int TRUE = 1;

const int MAX_WORD_SIZE = 10;
const int PREFIX_BUF_SIZE = 32;

int limited_random(int);

void skip_whitespace(istream &);
int expect(istream &, char);

void message(char *prefix, char *msg);
void error(char *msg);
void warn(char *msg);

#endif
