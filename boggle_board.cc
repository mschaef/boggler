/*
 * boggle_board.cc - A mutable representation of a game board.
 * by Michael Schaeffer
 */


#include <assert.h>

#include "common.h"
#include "wordtree.h"
#include "boggle_board.h"

/* Boggle Cube definitions.  These strings contain the letters
 * present on a set of boggle cubes.  Each string represents one
 * cube.
 */
const char *boggle_cubes [] = {
  "hdtnho", "tnuwoo", "nssseu", "aemeee", "asarfi",
  "swctnc", "mteott", "qbzjkx", "namgne", "aafars",
  "noldhr", "tetiii", "londdr", "fsyria", "tilcei",
  "pieclt", "piecls", "myrrpi", "dnanne", "aeeaee",
  "mgaeeu", "toutoo", "spriyf", "rlhhod", "gworru" 
};


boggle_board::boggle_board() {
  _board = NULL;
  _marks = NULL;

  set_size(5);
}

boggle_board::~boggle_board() {
  delete _board;
  delete _marks;
}

void boggle_board::set_size(int size) {
  set_size(size, size);
}

/*
 * Reset the board to the specified size.
 */
void boggle_board::set_size(int xsize, int ysize) {
  int i, j;

  delete _board;
  delete _marks;

  _xsize = xsize;
  _ysize = ysize;

  _board = new char[(_xsize + 2) * (_ysize + 2)];
  _marks = new bool[(_xsize + 2) * (_ysize + 2)];

  for(i = 0; i < _xsize + 2; i++)
    for(j = 0; j < _ysize + 2; j++) {
      set(i, j, '*');
      unmark(i, j);
    }

  for(i = 0; i < _xsize; i++) {
      mark(i, 0);
      mark(i, _ysize);
  }

  for(i = 0; i < _ysize; i++) {
      mark(0, i);
      mark(_xsize, i);
  }
}

/*
 * void boggle_board::shuffle()
 * 
 * This routine doesn't shuffle the board, per se, but rather
 * initializes its contents using the blocks describes in
 * boggle_cubes. If there are more than 25 cells that need to
 * be initialized, it switches to a standard random letter
 * algorithm.
 */
void boggle_board::shuffle() {
  if (_xsize * _ysize <= 25) {
       bool cube_used[25];

    for(int i = 0; i < 25; i++)
         cube_used[i] = false;

    for(int i = 1; i <= _xsize; i++)
      for(int j = 1; j <= _ysize; j++) {

	while(1) {
	  int cube = limited_random(25);

	  if (!cube_used[cube]) {
	    set(i, j, boggle_cubes[cube][limited_random(6)]);
        cube_used[cube] = true;
	  break;
	  }
	}
      };
  } else {
    for(int i = 1; i <= _xsize; i++)
      for(int j = 1; j <= _ysize; j++)
	set(i, j, 'a' + limited_random(26));
  }
}


char boggle_board::ref(int x, int y) {
  assert((x >= 0) && (x <= _xsize + 1) && (y >= 0) && (y <= _ysize + 1));
  return _board[x + y * _xsize];
}

void boggle_board::set(int x, int y, char ch) {
  assert((x >= 0) && (x <= _xsize + 1) && (y >= 0) && (y <= _ysize + 1));
  _board[x + y * _xsize] = ch;
}

bool boggle_board::is_marked(int x, int y) {
  assert((x >= 0) && (x <= _xsize + 1) && (y >= 0) && (y <= _ysize + 1));
  return _marks[x + y * _xsize];
};

void boggle_board::mark(int x, int y) {
  assert((x >= 0) && (x <= _xsize + 1) && (y >= 0) && (y <= _ysize + 1));
  _marks[x + y * _xsize] = true;
};

void boggle_board::unmark(int x, int y) {
  assert((x >= 0) && (x <= _xsize + 1) && (y >= 0) && (y <= _ysize + 1));
  _marks[x + y * _xsize] = false;
};

int boggle_board::xsize() {
  return _xsize;
};

int boggle_board::ysize() {
  return _ysize;
};

/* Recursive step for breadth-first word search. */
void boggle_board::find_words_at(int xloc,
                                 int yloc,
                                 wordtree::iterator wl_location,
                                 wordtree &words)
{
  if (wl_location.is_word())
    words.insert_word(wl_location());

  if (!is_marked(xloc, yloc)) {
    char ch = ref(xloc, yloc);

    if(wl_location.letter_exists(ch)) {
      mark(xloc, yloc);

      wordtree::iterator new_loc = wl_location.letter(ref(xloc, yloc));

      find_words_at(xloc    , yloc - 1, new_loc, words);
      find_words_at(xloc + 1, yloc - 1, new_loc, words);
      find_words_at(xloc + 1, yloc    , new_loc, words);
      find_words_at(xloc + 1, yloc + 1, new_loc, words);
      find_words_at(xloc    , yloc + 1, new_loc, words);
      find_words_at(xloc - 1, yloc + 1, new_loc, words);
      find_words_at(xloc - 1, yloc    , new_loc, words);
      find_words_at(xloc - 1, yloc - 1, new_loc, words);

      unmark(xloc, yloc);
    }
  }
};

/* Search the board for the words contained in 'dict', loading each
 * found word into 'found_words'.
 */
void boggle_board::find_words(wordtree &dict, wordtree &found_words)
{
  for (int xloc = 1; xloc <= _xsize; xloc++) 
    for (int yloc = 1; yloc <= _ysize; yloc++) 
      find_words_at(xloc, yloc, wordtree::iterator(dict), found_words);
};

/**
 * Write a board to the given ostream.
 */
ostream &operator <<(ostream &o, boggle_board &board) {
     cout << "{{" << board.xsize() << ' ' << board.ysize() << "}";
     for(int i = 1; i <= board.xsize(); i++) {
          cout << "{";
          for(int j = 1; j <= board.ysize(); j++) {
               o << board.ref(i,j);
               if (j == board.ysize())
                    cout << "}";
               else
                    cout << ' ';
          }
     }
     cout << '}';

     return o;
}

/*
 * Attempt to read a board from the given ostream. If there
 * is a parse error, the read is immediately aborted, and the
 * stream left in a bad state.
 */
istream &operator >>(istream &i, boggle_board &board) {
  int xsize, ysize;
  int xloc, yloc;
  char ch;

  skip_whitespace(i);
  if (!expect(i, '{')) goto failed_read;

  skip_whitespace(i);
  if (!expect(i, '{')) goto failed_read;

  i >> xsize;
  i >> ysize;

  skip_whitespace(i);
  if (!expect(i, '}')) goto failed_read;

  board.set_size(xsize, ysize);

  for(xloc = 1; xloc <= xsize; xloc++) {
    skip_whitespace(i);
    if (!expect(i, '{')) goto failed_read;

    for(yloc = 1; yloc <= ysize; yloc++) {
      skip_whitespace(i);
      i >> ch;
      board.set(xloc, yloc, ch);
    }

    skip_whitespace(i);
    if (!expect(i, '}')) goto failed_read;
  }
      
  return i;

failed_read:
  i.clear(ios::failbit);
  return i;
}

