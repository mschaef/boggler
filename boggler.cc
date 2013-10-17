/*
 * boggler.cc - Boggler main source file
 * by Michael Schaeffer
 */

#include "common.h"
#include "wordtree.h"
#include "boggle_board.h"

char *solution_dict_file = NULL;     // The dictionary file to read
char *puzzle_file        = NULL;     // The puzzle file to read
char *ignore_file        = NULL;     // The file containing words to ignore

bool generate            = false;    // action flag to generate the puzzle
bool help                = false;    // display help information
bool write_puzzle        = false;    // write the final puzzle

int size                 = 5;        // The size of the puzzle to be generated

int seed                 = -1;       // The random number seed

// A set of definitions of long command line options
option long_options[] = {
  {"solution-dictionary-file", 1, 0, 'd'},
  {"puzzle-file", 1, 0, 'p'},
  {"ignore-file", 1, 0, 'i'},
  {"generate", 0, 0, 'g'},
  {"write-puzzle", 0, 0, 'w'},
  {"size", 1, 0, 'S'},
  {"random-seed", 1, 0, 'r'},
  {"help", 0, 0, 'h'},
  {0, 0, 0, 0}
};

// The text displayed for command line help
char help_text[] = "\n\
Boggler v0.1\n\
by Michael Schaeffer\n\
\n\
Usage: boggler [options]\n\
\n\
Options:\n\
\n\
--solution-dictionary-file=<filename> (-d)  - Load a dictionary file and use it to solve the board\n\
--puzzle-file=<filename> (-p) - Load a boggle from disk\n\
--ignore-file=<filename> (-i) - Load a list of words to ignore in scoring\n\
\n\
If <filename> is -, read from standard input.  If more than one such\n\
file is specified, they are read in the order dictionary, puzzle,\n\
ignore\n\
\n\
--generate (-g) - Generate a boggle puzzle randomly\n\
--write-puzzle (-w) - Write the puzzle to standard output\n\
\n\
--size=<size> (-S) - Set the size of the boggle puzzle board\n\
--random-seed=<number> (-r) - Set the seed value for rand()";

/****************************************************************
 * int expect(int, char*[])
 *
 * Scan and parse the command line options, adjusting the global
 * control variables appropriately
 ****************************************************************/
void parse_options(int argc, char *argv[])
{
  // Reset getopt's state.
  optind = 0;

  while(optind < argc) {
    int option_index = 0;
    char option = getopt_long(argc, argv, "d:p:i:gS:hr:w",
			      long_options, &option_index);

    switch(option) {
    case 'd':
      if (solution_dict_file)
           error("Two dictionary files cannot be specified");

      solution_dict_file = strdup(optarg);
      break;

    case 'p':
      if (puzzle_file)
	error("Two puzzle files cannot be specified");

      if (generate)
    	warn("The specified puzzle file will be ignored");

      puzzle_file = strdup(optarg);
      break;

    case 'i':
      if (ignore_file)
	error("Two ignore files cannot be specified");

      ignore_file = strdup(optarg);
      break;

    case 'g':
      if (puzzle_file)
	warn("The specified puzzle file will be ignored");

      generate = true;
      break;

    case 'S':
      size = atoi(optarg);

      if (size <= 0)
	error("Invalid argument passed for size");
      break;

    case 'r':
      if (seed != -1)
	warn("The random seed can be only specified once");
      else {
	seed = atoi(optarg);

	if (seed <= 0)
	  error("Invalid argument passed for random seed");
      }
      break;

    case 'w':
      write_puzzle = true;
      break;

      break;
    case 'h':
      help = true;

      break;
    }
  }
}

/****************************************************************
 * template<class T>
 * int read_input(const char *filename, T &)
 *
 * Read an input object from either a file or standard input.
 ****************************************************************/
template<class T>
void read_input(const char *fn,  T &object, const char *filedesc)
{
     bool ok;

     if (strcmp(fn, "-") == 0) {
          ok = (cin >> object);
     } else {
          ifstream in(fn);

          ok = (in >> object);
     }

     if (!ok)
          error("Error reading file.");
}


/****************************************************************
 * void do_command()
 *
 * Execute the operations requested by the user
 ****************************************************************/
void do_command()
{
     if (help) {
          cout << help_text << endl;
          return;
     }

     boggle_board board;
    
     // Initalize the random number generator
     srand((seed == -1) ? time(0) : seed);

     // Generate or load a puzzle board.
     if (generate) {
          board.set_size(size);
          board.shuffle();

     } else if (puzzle_file) {
          read_input(puzzle_file, board, "puzzle file");

     } else
          warn("No valid puzzle specified"); 

     // write the puzzle board in use.
     if (write_puzzle)
          cout << board << endl;

     // Solve the puzzle board, if requested.

     if (solution_dict_file) {
          wordtree dictionary;
          wordtree results;

          read_input(solution_dict_file, dictionary, "dictionary file");

          wordtree ignored_words;

          if (ignore_file) {
               read_input(ignore_file, ignored_words, "puzzle file");

               dictionary.delete_words(ignored_words);
          } 

          board.find_words(dictionary, results);

          cout << results << endl;
     }
}

/****************************************************************
 * int main(int, char *[])
 ****************************************************************/
int main(int argc, char *argv[])
{
  // Parse the command line options
  parse_options(argc, argv);

  // Execute the requested command
  do_command();

  // Delete the dynamic storage used by the command line options
  delete solution_dict_file;
  delete puzzle_file;
  delete ignore_file;

  return 0;
}


