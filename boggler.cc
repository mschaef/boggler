/****************************************************************
 * boggler.cc - Boggler main source file
 * by Michael Schaeffer
 *
 * Revision History:
 * 8/1/1996
 *   - File created
 * 12/28/1996 
 *   - Extensive modifications for correctness
 * 12/31/1996 
 *   - Moved all I/O to class defn files
 * 1/2/1997 
 *   - Incorporated getopt_long to parse command line options
 *   - Built driver program for CLI
 *   - Changed to multiline format for revision history :)
 ****************************************************************/

#include "boggler.h"

char *dict_file    = NULL;     // The dictionary file to read
int dict_stdin     = FALSE;    // dictionary stdin read flag

char *puzzle_file  = NULL;     // The puzzle file to read
int puzzle_stdin   = FALSE;    // puzzle stdin read flag

char *ignore_file  = NULL;     // The file containing words to ignore
int ignore_stdin   = FALSE;    // ignore list stdin read flag

int generate       = FALSE;    // action flag to generate the puzzle
int solve          = FALSE;    // action flag to solve the puzzle
int score          = FALSE;    // solve flag to score the final solution
int location       = FALSE;    // solve flag to add location data to 
                               // the final solution
int help           = FALSE;    // display help information
int write_puzzle   = FALSE;    // write the final puzzle

int size           = 5;        // The size of the puzzle to be generated

int seed           = -1;       // The random number seed

// A set of definitions of long command line options
option long_options[] = {
  {"dictionary-file", 1, 0, 'd'},
  {"puzzle-file", 1, 0, 'p'},
  {"ignore-file", 1, 0, 'i'},
  {"generate", 0, 0, 'g'},
  {"solve", 0, 0, 's'},
  {"write-puzzle", 0, 0, 'w'},
  {"score", 0, 0, 'c'},
  {"location", 0, 0, 'l'},
  {"size", 1, 0, 'S'},
  {"random-seed", 1, 0, 'r'},
  {"help", 0, 0, 'h'},
  {0, 0, 0, 0}
};

// The text displayed for command line help
char help_text[] = "\
Boggler v0.1
by Michael Schaeffer

Usage: boggler [options]

Options:

--dictionary-file=<filename> (-d)  - Load a dictionary file
--puzzle-file=<filename> (-p) - Load a boggle from disk
--ignore-file=<filename> (-i) - Load a list of words to ignore in scoring

If <filename> is -, read from standard input.  If more than one such
file is specified, they are read in the order dictionary, puzzle,
ignore

--generate (-g) - Generate a boggle puzzle randomly
--solve (-s) - Solve the boggle puzzle
--score (-c) - Generate scoring information
--location (-l) - Generate word location information
--write-puzzle (-w) - Write the puzzle to standard output

--size=<size> (-S) - Set the size of the boggle puzzle board\n
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
    char option = getopt_long(argc, argv, "d:p:i:gsS:clhr:w",
			      long_options, &option_index);

    switch(option) {
    case 'd':
      if (dict_file)
	error("Two dictionary files cannot be specified");

      if (!strcmp(optarg, "-"))
	dict_stdin = TRUE;

      dict_file = strdup(optarg);
      break;

    case 'p':
      if (puzzle_file)
	error("Two puzzle files cannot be specified");

      if (generate)
    	warn("The specified puzzle file will be ignored");

      if (!strcmp(optarg, "-"))
	puzzle_stdin = TRUE;

      puzzle_file = strdup(optarg);
      break;

    case 'i':
      if (ignore_file)
	error("Two ignore files cannot be specified");

      if (!strcmp(optarg, "-"))
	ignore_stdin = TRUE;

      ignore_file = strdup(optarg);
      break;

    case 'g':
      if (puzzle_file)
	warn("The specified puzzle file will be ignored");

      generate = TRUE;
      break;

    case 's':
      solve = TRUE;
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
      write_puzzle = TRUE;
      break;

    case 'c':
      warn("Scoring not implemented yet");
      score = TRUE;
      break;

    case 'l':
      warn("Location not implemented yet");
      location = TRUE;
      break;

      break;
    case 'h':
      help = TRUE;

      break;
    }
  }
}

/****************************************************************
 * template<class T>
 * int read_from_stream(istream &, T &)
 *
 * Read an object from a stream, checking for failure
 ****************************************************************/
template<class T>
int read_from_stream(istream &i, T &object)
{
  if (i.eof() || i.fail() || i.bad())
    return TRUE;

  i >> object;

  if (i.eof() || i.fail() || i.bad())
    return TRUE;

  return FALSE;
}

/****************************************************************
 * void do_command()
 *
 * Execute the operations requested by the user
 ****************************************************************/
void do_command()
{
  // If the user asked for help, that's all we do
  if(help)
      cout << help_text << '\n';
  else {
    int status;
    wordtree dictionary;
    wordtree results;
    boggle_board b;
    
    // Initalize the random number generator
    if (seed == -1)
      srand(time(0));
    else
      srand(seed);

    // Initialize the dictionary file
    if (dict_file) {
      if (dict_stdin)
	status = read_from_stream(cin, dictionary);
      else {
	ifstream i(dict_file);
	status = read_from_stream(i, dictionary);
      }

      if (status)
	error("Error reading dictionary file");
    }


    // Initialize the puzzle board
    if (generate) {
      b.set_size(size);
      b.shuffle();

    } else if (puzzle_file) {
      if (puzzle_stdin)
	status = read_from_stream(cin, b);
      else {
	ifstream i(puzzle_file);
	status = read_from_stream(i, b);
      }
      
      if (status)
	error("Error reading puzzle file");

    } else
      warn("No valid puzzle specified");

    
    // Output the puzzle if the user so desires
    if (write_puzzle)
      cout << b << '\n';
    
    // Solve the puzzle
    if (solve) {
      wordtree ignored_words;

      if (ignore_file) {
	if (ignore_stdin)
	  status = read_from_stream(cin, ignored_words);
	else {
	  ifstream i(puzzle_file);
	  status = read_from_stream(i, ignored_words);
	}
	
	if (status)
	  error("Error reading puzzle file");
	else {
	  dictionary.delete_words(ignored_words);
	}
      } 

      b.find_words(dictionary, results);
      cout << results << '\n';
    }
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
  delete dict_file;
  delete puzzle_file;
  delete ignore_file;

  return 0;
}


