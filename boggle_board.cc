/****************************************************************
 * wordtree.cc - wordtree source file
 * by Michael Schaeffer
 *
 * Revision History:
 * 8/1/1996 
 *   - File created
 * 12/28/1996 
 *   - Extensive modifications for correctness
 *   - Added delete_words function
 ****************************************************************/

#include "boggler.h"


// Boggle Cube definitions.  These strings contain the letters
// present on a set of boggle cubes.  Each string represents one
// cube.
char *boggle_cubes [] = {
  "hdtnho", "tnuwoo", "nssseu", "aemeee", "asarfi",
  "swctnc", "mteott", "qbzjkx", "namgne", "aafars",
  "noldhr", "tetiii", "londdr", "fsyria", "tilcei",
  "pieclt", "piecls", "myrrpi", "dnanne", "aeeaee",
  "mgaeeu", "toutoo", "spriyf", "rlhhod", "gworru" 
};

/****************************************************************
 * boggle_board::boggle_board()
 * 
 * Build a new boggle board
 ****************************************************************/
boggle_board::boggle_board() {
  _board = NULL;
  _marks = NULL;

  set_size(5);
}

/****************************************************************
 * boggle_board::~boggle_board
 * 
 * Delete any dynamic memory allocated by the boggle board
 * used to store the board
 ****************************************************************/
boggle_board::~boggle_board() {
  delete _board;
  delete _marks;
}

/****************************************************************
 * void boggle_board::set_size(int)
 * 
 * Set up the board arrays from the specified dimensions
 ****************************************************************/
void boggle_board::set_size(int size) {
  set_size(size, size);
}

/****************************************************************
 * void boggle_board::set_size(int, int)
 * 
 * Set up the board arrays from the specified dimensions
 ****************************************************************/
void boggle_board::set_size(int xsize, int ysize) {
  int i, j;

  delete _board;
  delete _marks;

  _xsize = xsize;
  _ysize = ysize;

  _board = new char[(_xsize + 2) * (_ysize + 2)];
  _marks = new int[(_xsize + 2) * (_ysize + 2)];

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

/****************************************************************
 * void boggle_board::shuffle()
 * 
 * This routine doesn't shuffle the board, per se, but rather
 * initializes its contents using the blocks describes in
 * boggle_cubes. If there are more than 25 cells that need to
 * be initialized, it switches to a standard random letter
 * algorithm.
 ****************************************************************/
void boggle_board::shuffle() {
  if (_xsize * _ysize <= 25) {
    int cube_used[25];

    for(int i = 0; i < 25; i++)
      cube_used[i] = FALSE;

    for(int i = 1; i <= _xsize; i++)
      for(int j = 1; j <= _ysize; j++) {

	while(1) {
	  int cube = limited_random(25);

	  if (!cube_used[cube]) {
	    set(i, j, boggle_cubes[cube][limited_random(6)]);
	  cube_used[cube] = TRUE;
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

/****************************************************************
 * char boggle_board::ref(int x, int y);
 * 
 * Determine the contents of a given block.
 ****************************************************************/
char boggle_board::ref(int x, int y) {
  assert((x >= 0) && (x <= _xsize + 1) && (y >= 0) && (y <= _ysize + 1));
  return _board[x + y * _xsize];
}

/****************************************************************
 * char boggle_board::set(int x, int y, char ch);
 * 
 * Set the contents of a given block.
 ****************************************************************/
void boggle_board::set(int x, int y, char ch) {
  assert((x >= 0) && (x <= _xsize + 1) && (y >= 0) && (y <= _ysize + 1));
  _board[x + y * _xsize] = ch;
}

/****************************************************************
 * int boggle_board::is_marked(int x, int y)
 * 
 * Determine is a board block is marked
 ****************************************************************/
int boggle_board::is_marked(int x, int y) {
  assert((x >= 0) && (x <= _xsize + 1) && (y >= 0) && (y <= _ysize + 1));
  return _marks[x + y * _xsize];
};

/****************************************************************
 * void boggle_board::mark(int x, int y)
 * 
 * Mark a board block as used
 ****************************************************************/
void boggle_board::mark(int x, int y) {
  assert((x >= 0) && (x <= _xsize + 1) && (y >= 0) && (y <= _ysize + 1));
  _marks[x + y * _xsize] = TRUE;
};

/****************************************************************
 * void boggle_board::unmark(int x, int y)
 * 
 * Mark a block as unused.
 ****************************************************************/
void boggle_board::unmark(int x, int y) {
  assert((x >= 0) && (x <= _xsize + 1) && (y >= 0) && (y <= _ysize + 1));
  _marks[x + y * _xsize] = FALSE;
};

/****************************************************************
 * int boggle_board::xsize()
 * 
 * Return the xsize of a boggle board
 ****************************************************************/
int boggle_board::xsize() {
  return _xsize;
};

/****************************************************************
 * int boggle_board::ysize()
 * 
 * Return the ysize of a boggle board
 ****************************************************************/
int boggle_board::ysize() {
  return _ysize;
};

/****************************************************************
 * boggle_board::find_words_at(int, int, wordtree::iterator, wordtree &);
 *
 * Find words in a boggle board, given a location in the board
 * and in the word tree (using an iterator for the word tree)
 ****************************************************************/
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

/****************************************************************
 * boggle_board::find_words(wordtree &, wordtree &)
 *
 * Find words in a boggle board by calling find_words_at for
 * each block in the board.
 ****************************************************************/
void boggle_board::find_words(wordtree &dict,
			      wordtree &found_words)
{
  for (int xloc = 1; xloc <= _xsize; xloc++) 
    for (int yloc = 1; yloc <= _ysize; yloc++) 
      find_words_at(xloc, yloc, wordtree::iterator(dict), found_words);
};

/****************************************************************
 * ostream &operator <<(ostream &o, boggle_board &board)
 * 
 * Print out the contents of a boggle board.
 ****************************************************************/
ostream &operator <<(ostream &o, boggle_board &board) {
  cout << "{ {" << board.xsize() << ' ' << board.ysize() << "} ";
  for(int i = 1; i <= board.xsize(); i++) {
    cout << "{ ";
    for(int j = 1; j <= board.ysize(); j++) {
      o << board.ref(i,j);
      if (j == board.ysize())
	cout << "}";
      else
	cout << ' ';
    }
    cout << ' ';
  }
  cout << '}';

  return o;
}

/****************************************************************
 * istream &operator >>(istream &i, boggle_board &board)
 * 
 * Read in a boggle board in standard form. If input stream is 
 * malformed, bail out and set ios::fail in i.
 ****************************************************************/
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

