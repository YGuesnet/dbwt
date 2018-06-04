#include <cstdio>
#include <stack>
#include <cassert>
#include <iostream>

/**
 * Freely adapted from Emin Martinian
 * http://web.mit.edu/~emin/www.old/source_code/cpp_trees/
 */

namespace ranges {

//#include "trees/RedBlackTree.h"

// If the symbol CHECK_RB_TREE_ASSUMPTIONS is defined then the
// code does a lot of extra checking to make sure certain assumptions
// are satisfied.  This only needs to be done if you suspect bugs are
// present or if you make significant changes and want to make sure
// your changes didn't mess anything up.
// #define CHECK_RB_TREE_ASSUMPTIONS 1
#ifdef CHECK_RB_TREE_ASSUMPTIONS
#define check_assumptions() ca()
#else
#define check_assumptions() ((void) 0)
#endif

template<class T>
red_black_tree<T>::red_black_tree_node::red_black_tree_node()
    : red(false), left(this), right(this), parent(this) {
}

template<class T>
red_black_tree<T>::red_black_tree_node::red_black_tree_node(const T &entry)
  : entry(entry), red(false), left(nullptr), right(nullptr), parent(nullptr) {
}

template<class T>
red_black_tree<T>::red_black_tree_node::~red_black_tree_node() {
}

template<class T>
const T &red_black_tree<T>::red_black_tree_node::get_entry() const {
  return entry.e;
}

template<class T>
void red_black_tree<T>::red_black_tree_node::set_entry(const T &entry) {
  this->entry.e = entry;
}

template<class T>
void red_black_tree<T>::red_black_tree_node::set_left(red_black_tree_node *node) {
  left = node;
}

template<class T>
void red_black_tree<T>::red_black_tree_node::set_right(red_black_tree_node *node) {
  right = node;
}

template<class T>
void red_black_tree<T>::red_black_tree_node::set_parent(red_black_tree_node *node) {
  parent = node;
}

template<class T>
const typename red_black_tree<T>::red_black_tree_node *red_black_tree<T>::red_black_tree_node::get_left() const {
  return left;
}

template<class T>
const typename red_black_tree<T>::red_black_tree_node *red_black_tree<T>::red_black_tree_node::get_right() const {
  return right;
}

template<class T>
const typename red_black_tree<T>::red_black_tree_node *red_black_tree<T>::red_black_tree_node::get_parent() const {
  return parent;
}

template<class T>
typename red_black_tree<T>::red_black_tree_node *red_black_tree<T>::red_black_tree_node::get_left() {
  return left;
}

template<class T>
typename red_black_tree<T>::red_black_tree_node *red_black_tree<T>::red_black_tree_node::get_right() {
  return right;
}

template<class T>
typename red_black_tree<T>::red_black_tree_node *red_black_tree<T>::red_black_tree_node::get_parent() {
  return parent;
}

template<class T>
bool red_black_tree<T>::red_black_tree_node::is_red() const {
  return red;
}

template<class T>
void red_black_tree<T>::red_black_tree_node::set_red(bool red) {
  this->red = red;
}

template<class T>
void red_black_tree<T>::red_black_tree_node::print(const red_black_tree_node *nil) const {
  std::string s;
  s += "[key = ";
  s += std::to_string(this->get_entry());
  s += ", l.key = ";
  s += left == nil ? "NULL" : std::to_string(left->get_entry());
  s += ", r.key = ";
  s += right == nil ? "NULL" : std::to_string(right->get_entry());
  s += ", p.key = ";
  s+= parent->parent == nil ? "NULL" : std::to_string(parent->get_entry());
  s += ", ";
  s += red ? "red" : "black";
  s += "]";

  std::cout << s << std::endl;
}

template<class T>
typename red_black_tree<T>::const_iterator &red_black_tree<T>::const_iterator::operator++() {
  if (ptr->right != nil) {
    ptr = ptr->right;
    while (ptr->left != nil) {
      ptr = ptr->left;
    }
  } else {
    const red_black_tree_node *p = ptr->parent;
    while (p->right == ptr) {
      ptr = ptr->parent;
      p = ptr->parent;
    }
    ptr = ptr->parent;
  }
  return *this;
}

template<class T>
bool red_black_tree<T>::const_iterator::operator!=(const const_iterator& it) {
    return this->ptr != it.ptr;
}

template<class T>
const T &red_black_tree<T>::const_iterator::operator*() {
  return ptr->get_entry();
}

template<class T>
red_black_tree<T>::const_iterator::const_iterator(const red_black_tree_node *node,
        const red_black_tree_node *root, const red_black_tree_node *nil) {
  ptr = node;
  this->nil = nil;
  this->root = root;
}

template<class T>
typename red_black_tree<T>::const_iterator red_black_tree<T>::begin() const {
  const red_black_tree_node *p = root;
  while (p->left != nil) {
    p = p->left;
  }
  return const_iterator(p, root, nil);
}

template<class T>
typename red_black_tree<T>::const_iterator red_black_tree<T>::end() const {
  return const_iterator(root, root, nil);
}

template<class T>
red_black_tree<T>::red_black_tree() {
  //std::cerr << "red_black_tree<T>::red_black_tree()" << std::endl;
  nil = new red_black_tree_node;
  root = new red_black_tree_node;
  root->parent = root->left = root->right = nil;
  root->red = false;
  num_nodes = 0;
}

template<class T>
red_black_tree<T>::red_black_tree(red_black_tree &&tree) {
  //std::cerr << "red_black_tree<T>::red_black_tree(red_black_tree &&tree)" << std::endl;
  nil = tree.nil;
  root = tree.root;
  num_nodes = tree.num_nodes;
  tree.root = nullptr;
  tree.nil = nullptr;
  tree.num_nodes = 0;
}

template<class T>
red_black_tree<T>& red_black_tree<T>::operator=(red_black_tree<T> &&tree) {
//  std::cerr << "red_black_tree<T>::operator=(red_black_tree<T> &&tree)" << std::endl;
  clear();
  delete nil;
  delete root;
  nil = tree.nil;
  root = tree.root;
  num_nodes = tree.num_nodes;
  tree.root = nullptr;
  tree.nil = nullptr;
  tree.num_nodes = 0;
  return *this;
}


template<class T>
bool red_black_tree<T>::empty() const {
  return root->left == nil;
}

template<class T>
std::size_t red_black_tree<T>::get_num_nodes() const {
  return num_nodes;
}

template<class T>
void red_black_tree<T>::clear() {
  if (root == nullptr) {
    return;
  }

  red_black_tree_node *x = root->left;
  std::stack<red_black_tree_node *> stuffToFree;

  if (x != nil) {
    if (x->left != nil) {
      stuffToFree.push(x->left);
    }
    if (x->right != nil) {
      stuffToFree.push(x->right);
    }
    // delete x->storedEntry;
    delete x;
    while (!stuffToFree.empty()) {
      x = stuffToFree.top();
      stuffToFree.pop();
      if (x->left != nil) {
        stuffToFree.push(x->left);
      }
      if (x->right != nil) {
        stuffToFree.push(x->right);
      }
      // delete x->storedEntry;
      delete x;
    }
  }

  root->left = nil;
  num_nodes = 0;
}

template<class T>
void red_black_tree<T>::left_rotate(red_black_tree_node* x) {
  red_black_tree_node *y;
 
  y = x->right;
  x->right = y->left;

  /*  I originally wrote this function to use the sentinel for */
  /*  nil to avoid checking for nil.  However this introduces a */
  /*  very subtle bug because sometimes this function modifies */
  /*  the parent pointer of nil.  This can be a problem if a */
  /*  function which calls left_rotate also uses the nil sentinel */
  /*  and expects the nil sentinel's parent pointer to be unchanged */
  /*  after calling this function.  For example, when DeleteFixUP */
  /*  calls left_rotate it expects the parent pointer of nil to be */
  /*  unchanged. */
  if (y->left != nil) {
	  y->left->parent = x;
  }
  
  y->parent = x->parent;

  /* instead of checking if x->parent is the root as in the book, we */
  /* count on the root sentinel to implicitly take care of this case */

  if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;

  check_assumptions();
  assert(!nil->red);
}

template<class T>
void red_black_tree<T>::right_rotate(red_black_tree_node* y) {
  red_black_tree_node* x;

  x = y->left;
  y->left = x->right;

  /*  I originally wrote this function to use the sentinel for */
  /*  nil to avoid checking for nil.  However this introduces a */
  /*  very subtle bug because sometimes this function modifies */
  /*  the parent pointer of nil.  This can be a problem if a */
  /*  function which calls left_rotate also uses the nil sentinel */
  /*  and expects the nil sentinel's parent pointer to be unchanged */
  /*  after calling this function.  For example, when DeleteFixUP */
  /*  calls left_rotate it expects the parent pointer of nil to be */
  /*  unchanged. */
  if (x->right != nil)  {
	  x->right->parent = y;
  }

  /* instead of checking if x->parent is the root as in the book, we */
  /* count on the root sentinel to implicitly take care of this case */

  x->parent = y->parent;
  if (y == y->parent->left) {
    y->parent->left = x;
  } else {
    y->parent->right = x;
  }

  x->right = y;
  y->parent = x;

  check_assumptions();
  assert(!nil->red);
}

template<class T>
void red_black_tree<T>::insert(const T &entry) {
  red_black_tree_node *z = new red_black_tree_node(entry);
  tree_insert_node(z);
}

template<class T>
void red_black_tree<T>::tree_insert_node(red_black_tree<T>::red_black_tree_node *z) {
  tree_insert_help(z);
  tree_insert_fixup(z);
  ++num_nodes;
}

template<class T>
void red_black_tree<T>::tree_insert_help(red_black_tree<T>::red_black_tree_node *z) {
  red_black_tree_node *x;
  red_black_tree_node *y;
    
  y = root;
  x = root->left;
  while (x != nil) {
    y = x;
    if (z->get_entry() < x->get_entry()) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  z->parent = y;
  if ((y == root) || (z->get_entry() < y->get_entry())) {
    y->left = z;
  } else {
    y->right = z;
  }

  z->left = z->right = nil;
  z->red = 1;

  assert(!nil->red);
}

template<class T>
void red_black_tree<T>::tree_insert_fixup(red_black_tree_node *z) {
  red_black_tree_node * y;
  while (z->parent->red) { /* use sentinel instead of checking for root */
    if (z->parent == z->parent->parent->left) {
      y = z->parent->parent->right;
      if (y->red) {
        z->parent->red = 0;
        y->red = 0;
        z->parent->parent->red = 1;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(z);
        }
        z->parent->red = 0;
        z->parent->parent->red = 1;
        right_rotate(z->parent->parent);
      }
    } else { /* case for x->parent == x->parent->parent->right */
         /* this part is just like the section above with */
         /* left and right interchanged */
      y = z->parent->parent->left;
      if (y->red) {
        z->parent->red = 0;
        y->red = 0;
        z->parent->parent->red = 1;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(z);
        }
        z->parent->red = 0;
        z->parent->parent->red = 1;
        left_rotate(z->parent->parent);
      }
    }
  }
  root->left->red = 0;
  check_assumptions();
  assert(!nil->red);
  assert(!root->red);
}

template<class T>
typename red_black_tree<T>::red_black_tree_node * red_black_tree<T>::get_successor_of(red_black_tree_node * x) const {
  red_black_tree_node *y = x->right;

  if (y != nil) {
    while (y->left != nil) { /* returns the minium of the right subtree of x */
      y = y->left;
    }
    return y;
  } else {
    y = x->parent;
    while (x == y->right) { /* sentinel used instead of checking for nil */
      x = y;
      y = y->parent;
    }
    if (y == root) {
    	return nullptr;
    }
    return y;
  }
}

template<class T>
typename red_black_tree<T>::red_black_tree_node *red_black_tree<T>::get_predecessor_of(red_black_tree_node *x) const {
  red_black_tree_node *y = x->left;

  if (y != nil) {
    while (y->right != nil) { /* returns the maximum of the left subtree of x */
      y = y->right;
    }
    return y;
  } else {
    y = x->parent;
    while (x == y->left) {
      if (y == root) {
    	  return nullptr;
      }
      x = y;
      y = y->parent;
    }
    return y;
  }
}

template<class T>
typename red_black_tree<T>::red_black_tree_node *red_black_tree<T>::get_minimum_of(red_black_tree_node *node) {
  if (node == nil) {
    return nil;
  }

  red_black_tree_node *x = node;
  while (x->left != nil) {
    x = x->left;
  }
  return x;
}

template<class T>
typename red_black_tree<T>::red_black_tree_node *red_black_tree<T>::get_maximum_of(red_black_tree_node *node) {
  if (node == nil) {
    return nil;
  }

  red_black_tree_node *x = node;
  while (x->right != nil) {
    x = x->right;
  }
  return x;
}


template<class T>
typename red_black_tree<T>::red_black_tree_node *red_black_tree<T>::search(const T &entry) {
  red_black_tree_node *x = root->left;

  x = root->left;
  while (x != nil) {
    if (entry == x->get_entry()) {
      return x;
    } else if (entry < x->get_entry()) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  return x;
}

template<class T>
void red_black_tree<T>::delete_fix_up(red_black_tree_node *x) {
  red_black_tree_node *w;
  red_black_tree_node *rootLeft = root->left;

  while( (!x->red) && (rootLeft != x)) {
    if (x == x->parent->left) {
      w = x->parent->right;
      if (w->red) {
        w->red = 0;
        x->parent->red = 1;
        left_rotate(x->parent);
        w = x->parent->right;
      }
      if ((!w->right->red) && (!w->left->red)) {
        w->red = 1;
        x = x->parent;
      } else {
        if (!w->right->red) {
          w->left->red = 0;
          w->red = 1;
          right_rotate(w);
          w = x->parent->right;
        }
        w->red = x->parent->red;
        x->parent->red = 0;
        w->right->red = 0;
        left_rotate(x->parent);
        x = rootLeft; /* this is to exit while loop */
      }
    } else { /* the code below is has left and right switched from above */
      w = x->parent->left;
      if (w->red) {
        w->red = 0;
        x->parent->red = 1;
        right_rotate(x->parent);
        w = x->parent->left;
      }
      if ((!w->right->red) && (!w->left->red)) {
        w->red = 1;
        x = x->parent;
      } else {
        if (!w->left->red) {
          w->right->red = 0;
          w->red = 1;
          left_rotate(w);
          w = x->parent->left;
        }
        w->red = x->parent->red;
        x->parent->red = 0;
        w->left->red = 0;
        right_rotate(x->parent);
        x = rootLeft; /* this is to exit while loop */
      }
    }
  }
  x->red = 0;

  check_assumptions();
  assert(!nil->red);
}

template<class T>
bool red_black_tree<T>::delete_entry(const T& entry) {
  red_black_tree_node *z = search(entry);
  if (z == nil) {
    return false;
  }

  delete_node(z);
  return true;
}

template<class T>
void red_black_tree<T>::delete_node(red_black_tree_node *z) {
  red_black_tree_node *y;
  red_black_tree_node *x;

  // y : noeud a retirer de l'arbre, il s'agit de z si z à un fils ou moins
  // et sinon il s'agit du successeur de z (qu'on mettra ensuite à la place de z).
  y = ((z->left == nil) || (z->right == nil)) ? z : get_successor_of(z);

  // y a au plus un fils : x (nil si y n'a pas de fils)
  x = (y->left == nil) ? y->right : y->left;

  // On retire y
  // On s'autorise à modifier le père de nill
  x->parent = y->parent;
  if (root == y->parent) {
    root->left = x;
  } else {
    if (y == y->parent->left) {
      y->parent->left = x;
    } else {
      y->parent->right = x;
    }
  }

  if (y != z) {
    // On met y à la place de z
    assert(y != nil);
  
    y->left = z->left;
    y->right = z->right;
    y->parent = z->parent;
    z->left->parent = z->right->parent = y;
    if (z == z->parent->left) {
      z->parent->left = y;
    } else {
      z->parent->right = y;
    }
    if (!(y->red)) {
      y->red = z->red;
      delete_fix_up(x);
    } else {
      y->red = z->red; 
    }
  } else {
    if (!(y->red)) {
      delete_fix_up(x);
    }
  }

  delete z;
  --num_nodes;

  check_assumptions();
  assert(!nil->red);
}

template<class T>
void red_black_tree<T>::tree_print_helper(red_black_tree_node* x) const {
  if (x != nil) {
    tree_print_helper(x->left);
    x->print(nil);
    tree_print_helper(x->right);
  }
}

template<class T>
red_black_tree<T>::~red_black_tree() {
//  std::cerr << "~red_black_tree()" << std::endl;
  clear();
  delete nil;
  delete root;
}

template<class T>
void red_black_tree<T>::print() const {
  tree_print_helper(root->left);
}

template<class T>
void red_black_tree<T>::ca() const {
 	assert(nil->red == 0);
 	assert(root->red == 0);
}

}
