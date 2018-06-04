#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <cmath>
#include <climits>
#include <iostream>
#include <memory>

/**
 * Freely adapted from Emin Martinian
 * http://web.mit.edu/~emin/www.old/source_code/cpp_trees/
 */

namespace ranges {


template<class T>
class red_black_tree {
protected:
  class red_black_tree_node;

public:
  red_black_tree();

  red_black_tree(red_black_tree &&tree);
  red_black_tree& operator=(red_black_tree &&tree);

  virtual ~red_black_tree();

  void print() const;

  bool delete_entry(const T& entry);
  void insert(const T &entry);

  bool empty() const;
  void clear();

  class const_iterator {
    friend red_black_tree;

  public:
    const_iterator &operator++();

    bool operator!=(const const_iterator& it) ;

    const T &operator*();

  private:
    const_iterator(const red_black_tree_node *node,
        const red_black_tree_node *root, const red_black_tree_node *nil);

    const red_black_tree_node *root;
    const red_black_tree_node *nil;
    const red_black_tree_node *ptr;
  };

  const_iterator begin() const;

  const_iterator end() const;

  std::size_t get_num_nodes() const;

protected:
  /**
   * T a besoin de disposer de l'opérateur de comparaison <,
   * de l'égalité == ainsi que de l'affectation =
   */
  class red_black_tree_node {
    friend class red_black_tree;

  public:
    red_black_tree_node();
    red_black_tree_node(const T &entry);

    ~red_black_tree_node();

    const T &get_entry() const;
    void set_entry(const T &entry);

    void set_left(red_black_tree_node *node);
    void set_right(red_black_tree_node *node);
    void set_parent(red_black_tree_node *node);

    const red_black_tree_node *get_left() const;
    const red_black_tree_node *get_right() const;
    const red_black_tree_node *get_parent() const;

    red_black_tree_node *get_left();
    red_black_tree_node *get_right();
    red_black_tree_node *get_parent();

    bool is_red() const;
    void set_red(bool red = true);

  protected:
    void print(const red_black_tree_node *nil) const;

    union entry {
      T e;
      int fake;

      entry() { fake = 0; };
      entry(const T &entry) { e = entry; };
      ~entry() {};
    } entry;

    bool red; /* if !red then the node is black */
    red_black_tree_node *left;
    red_black_tree_node *right;
    red_black_tree_node *parent;

  };

protected:
  /**
   *   A sentinel is used for root and for nil.
   *   nil is a class member and root is created when RedBlackTreeCreate is called.
   *   root->left should always point to the node which is the root of the tree and
   *   root->parent is nil.
   *   nil points to a node which should always be black but has arbitrary children and
   *   parent and no key or info.
   *   The point of using these sentinels is so that the root and nil nodes do not require
   *   special cases in the code
   */

  red_black_tree_node * nil;
  red_black_tree_node * root;
  std::size_t num_nodes;

  void delete_node(red_black_tree_node *node);

  red_black_tree_node *get_predecessor_of(red_black_tree_node *) const;
  red_black_tree_node *get_successor_of(red_black_tree_node *) const;
  red_black_tree_node *search(const T &entry);

  red_black_tree_node *get_minimum_of(red_black_tree_node *node);
  red_black_tree_node *get_maximum_of(red_black_tree_node *node);

  void left_rotate(red_black_tree_node *);
  void right_rotate(red_black_tree_node *);

  virtual void tree_insert_node(red_black_tree_node *);

  void tree_insert_help(red_black_tree_node *);
  void tree_insert_fixup(red_black_tree_node *);

  void delete_fix_up(red_black_tree_node *);

  void tree_print_helper(red_black_tree_node *) const;

  void ca() const;
};

}

#include <trees/red_black_tree.hpp>

#endif // REDBLACKTREE_H
