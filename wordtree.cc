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
 * wordtree.cc - wordtree source file
 * by Michael Schaeffer
 */

#include <assert.h>
#include <string.h>

#include "common.h"
#include "wordtree.h"

void wordtree::insert_word(char *new_word)
{
  wt_node *current_node = &_node;
  char *current_char = new_word;

  while (*current_char != '\0') {
    if (*current_char >= 'a' && *current_char <= 'z') { 

      if (current_node->_child_node[*current_char - 'a'] == NULL)
	current_node->_child_node[*current_char - 'a'] = 
	  new wt_node(*current_char, false);
      
      current_node = current_node->_child_node[*current_char - 'a'];

      assert(current_node);
    }

    current_char++;
  }

  current_node->_is_word = true;
};

/* Ensure that the word, 'target_word' is not marked as a valid word. */
void wordtree::delete_word(char *target_word) {
  wt_node *current_node = &_node;
  char *current_char = target_word;

  while (*current_char != '\0') {
    current_node = current_node->_child_node[*current_char - 'a'];
    current_char++;

    assert(current_node);
  }

  current_node->_is_word = false;
};


/* Ensure that all words in `wt` are not marked as valid words. */
void wordtree::delete_words(wordtree &wt) {
  delete_words(iterator(wt));
};

/* Ensure that all words traversed by iterator i ` are not marked as
 * valid words. */
void wordtree::delete_words(iterator i)
{
  if (i.is_word())
    delete_word(i());

  for(char ch = 'a'; ch <= 'z'; ch++)
    if (i.letter_exists(ch))
      delete_words(i.letter(ch));
}

/* Dump the tree structure for debugging purposes. */
void wordtree::dump() {
  _node.dump();
};

/* Print each word in a wordtree to the output stream. */
void wordtree::print(ostream &o, iterator i)
{
  if (i.is_word())
    o << i() << ' ';

  for(char ch = 'a'; ch <= 'z'; ch++)
    if (i.letter_exists(ch))
      print(o, i.letter(ch));
}

/* Create a new traversal iterator rooted at the base of a wordtree,
 * using the specified string as a prefix.
 */
wordtree::iterator::iterator(wordtree &wt, char *prefix)
{
  _current_node = &wt._node;

  initialize_prefix(prefix);
}

/* Create a new traversal iterator rooted at the passed wt_node, and
 * using the specified string as a prefix.
 */
wordtree::iterator::iterator(wt_node *wtn, char *prefix)
{
  assert(wtn);

  _current_node = wtn;
  initialize_prefix(prefix);
};

/*
 * wordtree::iterator::initialize_prefix(char *)
 *
 * Initialize the prefix of an iterator.  The prefix is the
 * string represented by the nodes that have been traversed
 * by the iterator to that point.
 */
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

/* Determine if a letter is a valid continuation of
 * the prefix.
 */
bool wordtree::iterator::letter_exists(char letter)
{
  if (letter >= 'a' && letter <= 'z')
    return (_current_node->_child_node[letter - 'a']) != NULL;

  return false;
}

/* Determine if the iterator represents a traversal of a valid word. */
bool wordtree::iterator::is_word()
{
  return (_current_node->_is_word);
}

/* Return a pointer to the current prefix.  This string must be
 * used before the iterator is destructed.
 */
char *wordtree::iterator::operator()()
{
  return _prefix;
}

/* Return a new traversal iterator rooted at the specified letter. */
wordtree::iterator wordtree::iterator::letter(char letter)
{
  assert(letter_exists(letter));

  return iterator(_current_node->_child_node[letter - 'a'], _prefix);
}

wordtree::wt_node::wt_node(char ch, bool is_word /* = FALSE */)
{
  _ch = ch;
  _is_word = is_word;

  for(char ch = 'a'; ch <= 'z'; ch++)
    _child_node[ch - 'a'] = NULL;
};

wordtree::wt_node::~wt_node() {
  for(char ch = 'a'; ch <= 'z'; ch++)
    delete _child_node[ch - 'a'];
};

/* Make a dump of the structure of the tree for debugging purposes,
 * doing a preorder traversal of the wordtree.
 */
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

/* Print each word in a wordtree on the passed output stream.
 * wordtree::print does the actual work.
 */
ostream &operator<<(ostream &o, wordtree &wt)
{
     o << "{";

     wt.print(o, wordtree::iterator(wt));

     o << "}";

     return o;
}

/* Load a list of words, delimtied with braces, into a wordtree. */
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

