/*
 * wordtree.h - wordtree source file
 * by Michael Schaeffer
 */

#ifndef WORDTREE_H
#define WORDTREE_H

class wordtree {
private:
  struct wt_node; 

public:
  void insert_word(char *new_word);
  void delete_word(char *new_word);
  void delete_words(wordtree &old_words);
  void dump();

  class iterator {
  public:
    iterator(wordtree &wt, char *prefix = NULL);

    char *operator()();

    bool letter_exists(char letter);
    bool is_word();

    iterator letter(char letter);

  private:
    iterator(wt_node *wtn, char *prefix = NULL);
    void initialize_prefix(char *prefix);

    char _prefix[PREFIX_BUF_SIZE];
    wt_node *_current_node;
  };

private:
  struct wt_node {
    wt_node(char ch = '\0', bool is_word = false);
    ~wt_node();
    void dump();

    char _ch;
    bool _is_word;
    wt_node *_child_node[26];
  };

  friend ostream &operator<<(ostream &o, wordtree &wt);

  void print(ostream &o, iterator i);

  void delete_words(iterator old_words);
  wt_node _node;
};  

ostream &operator<<(ostream &o, wordtree &wt);
istream &operator>>(istream &i, wordtree &wt);

#endif


