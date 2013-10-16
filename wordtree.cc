/****************************************************************
 * wordtree.cc - wordtree source file
 * by Michael Schaeffer
 *
 * Revision History:
 * 8/1/1996 - File created
 * 12/28/1996 - Extensive modifications for correctness
 ****************************************************************/

#include "boggler.h"

/****************************************************************
 * wordtree::insert_word(char *)
 *
 * Insert a word into a word tree, adding new nodes to the word
 * tree as necessary
 ****************************************************************/
void wordtree::insert_word(char *new_word) {
  wt_node *current_node = &_node;
  char *current_char = new_word;

  while (*current_char != '\0') {
    if (*current_char >= 'a' && *current_char <= 'z') { 

      if (current_node->_child_node[*current_char - 'a'] == NULL)
	current_node->_child_node[*current_char - 'a'] = 
	  new wt_node(*current_char, FALSE);
      
      current_node = current_node->_child_node[*current_char - 'a'];

      assert(current_node);
    }

    current_char++;
  }

  current_node->_is_word = TRUE;
};

/****************************************************************
 * wordtree::delete_word(char *)
 *
 * Mark a tree node as not being a word. No tree nodes are deleted
 ****************************************************************/
void wordtree::delete_word(char *new_word) {
  wt_node *current_node = &_node;
  char *current_char = new_word;

  while (*current_char != '\0') {
    current_node = current_node->_child_node[*current_char - 'a'];
    current_char++;

    assert(current_node);
  }

  current_node->_is_word = FALSE;
};


/****************************************************************
 * void wordtree::delete_words(wordtree &)
 *
 * Mark a every word in the paramater word as being deleted
 ****************************************************************/
void wordtree::delete_words(wordtree &wt) {
  delete_words(iterator(wt));
};

void wordtree::delete_words(iterator i)
{
  if (i.is_word())
    delete_word(i());

  for(char ch = 'a'; ch <= 'z'; ch++)
    if (i.letter_exists(ch))
      delete_words(i.letter(ch));
}

/****************************************************************
 * void wordtree::dump()
 *
 * Make a dump of the structure of the tree for debugging purposes
 ****************************************************************/
void wordtree::dump() {
  _node.dump();
};

/****************************************************************
 * wordtree::print(ostream &)
 *
 * Print each word in a wordtree on the passed output stream.
 ****************************************************************/
void wordtree::print(ostream &o, iterator i)
{
  if (i.is_word())
    o << i() << ' ';

  for(char ch = 'a'; ch <= 'z'; ch++)
    if (i.letter_exists(ch))
      print(o, i.letter(ch));
}

/****************************************************************
 * wordtree::iterator::iterator(wordtree &, char *);
 *
 * Create a new iterator rooted at the base of a wordtree, using
 * the specified string as a prefix.
 ****************************************************************/
wordtree::iterator::iterator(wordtree &wt, char *prefix)
{
  _current_node = &wt._node;

  initialize_prefix(prefix);
}

/****************************************************************
 * wordtree::iterator::iterator(wt_node *, char *);
 *
 * Create a new iterator rooted at the specified wordtree node
 ****************************************************************/
wordtree::iterator::iterator(wt_node *wtn, char *prefix)
{
  assert(wtn);

  _current_node = wtn;
  initialize_prefix(prefix);
};

/****************************************************************
 * wordtree::iterator::initialize_prefiz(char *)
 *
 * Initialize the prefix of an iterator.  The prefix is the
 * string represented by the iterator, and is used when a word node
 * is found to extract the word.
 ****************************************************************/
void wordtree::iterator::initialize_prefix(char *prefix)
{
  // Null out the string, and append the passed prefix
  _prefix[0] = '\0';

  if (prefix)
    strncpy(_prefix, prefix, PREFIX_BUF_SIZE - 1);

  // Append the current letter to the end of the prefix
  int length = strlen(_prefix);

  _prefix[length] = _current_node->_ch;
  _prefix[length + 1] = '\0';
};

/****************************************************************
 * wordtree::iterator::letter_exists(char)
 *
 * Determine if the passed letter is a valid continuation of
 * the prefix.
 ****************************************************************/
int wordtree::iterator::letter_exists(char letter)
{
  if (letter >= 'a' && letter <= 'z')
    return (_current_node->_child_node[letter - 'a']) != NULL;
  else
    return FALSE;
}

/****************************************************************
 * wordtree::iterator::is_word()
 *
 * Determine if we've reached a valid word yet
 ****************************************************************/
int wordtree::iterator::is_word()
{
  return (_current_node->_is_word);
}

/****************************************************************
 * wordtree::iterator::operator()()
 *
 * Return a pointer to the current prefix.  This string must be
 * used before the iterator is destructed.
 ****************************************************************/
char *wordtree::iterator::operator()()
{
  return _prefix;
}

/****************************************************************
 * wordtree::iterator::letter(char)
 *
 * Return a new iterator rooted at the letter passed as a
 * paramater
 ****************************************************************/
wordtree::iterator wordtree::iterator::letter(char letter)
{
  assert(letter_exists(letter));

  return iterator(_current_node->_child_node[letter - 'a'], _prefix);
}

/**********************************************************************
 * wordtree::wt_node::wt_node(char, int)
 *
 * Create a new wordtree node.
 **********************************************************************/
wordtree::wt_node::wt_node(char ch, int is_word /* = FALSE */)
{
  _ch = ch;
  _is_word = is_word;

  // Initialize each child node to null
  for(char ch = 'a'; ch <= 'z'; ch++)
    _child_node[ch - 'a'] = NULL;
};

/**********************************************************************
 * wordtree::wt_node::wt_node(char, int)
 *
 * Destroy the wordtree node by destroying all subtrees
 **********************************************************************/
wordtree::wt_node::~wt_node() {
  for(char ch = 'a'; ch <= 'z'; ch++)
    delete _child_node[ch - 'a'];
};

/**********************************************************************
 * wordtree::wt_node::dump()
 *
 * Make a dump of the structure of the tree for debugging purposes,
 * doing a preorder traversal of the wordtree.
 **********************************************************************/
void wordtree::wt_node::dump() {
  if (_is_word)
    cout << this << " *(";
  else
    cout << this << "  (";

  for(char ch = 'a'; ch <= 'z'; ch++)
    if (_child_node[ch - 'a'] != NULL)
      cout << ch << ", " << _child_node[ch - 'a'] << "; ";

  cout << ")" << endl;

  for(char ch = 'a'; ch <= 'z'; ch++)
    if (_child_node[ch - 'a'] != NULL) 
      _child_node[ch - 'a']->dump();
}

/****************************************************************
 * ostream &operator<<(ostream &, wordtree &);
 *
 * Print each word in a wordtree on the passed output stream.
 * wordtree::print1 does the actual work.
 ****************************************************************/
ostream &operator<<(ostream &o, wordtree &wt)
{
     o << "{";

     wt.print(o, wordtree::iterator(wt));

     o << "}";

     return o;
}

/****************************************************************
 * ostream &operator<<(ostream &, wordtree &);
 *
 * load a dictionary file into a wordtree
 ****************************************************************/
istream &operator>>(istream &i, wordtree &wt)
{
  char word_buf[MAX_WORD_SIZE];
  char ch = '\0';
  int index;

  skip_whitespace(i);
  if (!expect(i, '{')) goto failed_read;

  while(ch != '}') {
    skip_whitespace(i);
    i.get(ch);

    index = 0;
    while((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
      word_buf[index] = ch;
      index++;

      i.get(ch);
    }

    i.putback(ch);
    word_buf[index] = '\0';

    if (index > 0)
         wt.insert_word(word_buf);
  }

  return i;

failed_read:
  i.clear(ios::failbit);
  return i;
};

