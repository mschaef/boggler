/*
 * common.h - A set of commonly useful utility functions
 * by Michael Schaeffer
 */

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

const int MAX_WORD_SIZE = 32;
const int PREFIX_BUF_SIZE = 32;

int limited_random(int);

void skip_whitespace(istream &);
int expect(istream &, char);

void error(const char *msg);
void warn(const char *msg);

#endif
