// Copyright (c) Mike Schaeffer. All rights reserved.
//
// The use and distribution terms for this software are covered by the
// Eclipse Public License 2.0 (https://opensource.org/licenses/EPL-2.0)
// which can be found in the file LICENSE at the root of this distribution.
// By using this software in any fashion, you are agreeing to be bound by
// the terms of this license.
//
// You must not remove this notice, or any other, from this software.

/*
 * common.h - A set of commonly useful utility functions
 * by Michael Schaeffer
 */

#ifndef __COMMON_H
#define __COMMON_H

#include <iostream>

using namespace std;

const int MAX_WORD_SIZE = 32;
const int PREFIX_BUF_SIZE = 32;

int limited_random(int);

void skip_whitespace(istream &);
int expect(istream &, char);

void error(const char *msg);
void warn(const char *msg);

#endif
