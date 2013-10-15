/****************************************************************
 * boggle_board.h - boggle playfield source file
 * by Michael Schaeffer
 *
 * Revision History:
 * 8/1/1996 - File created
 * 12/28/1996 - Extensive modifications for correctness
 ****************************************************************/

#ifndef __BOGGLE_BOARD_H
#define __BOGGLE_BOARD_H

#include "boggler.h"

class boggle_board {
public:
  boggle_board();

  void set_size(int size);
  void set_size(int xsize, int ysize);

  ~boggle_board();

  void shuffle();
  char ref(int x, int y);
  void set(int x, int y, char ch);

  int is_marked(int x, int y);
  void mark(int x, int y);
  void unmark(int x, int y);

  int xsize();
  int ysize();

  void find_words(wordtree &, wordtree &);
  void find_words_at(int xloc, int yloc, wordtree::iterator, wordtree &);
private:
  int _xsize, _ysize;

  char *_board;
  int *_marks;
};

ostream &operator <<(ostream &o, boggle_board &board);
istream &operator >>(istream &o, boggle_board &board);

#endif

