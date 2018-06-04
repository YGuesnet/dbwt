/**
 * dsbwt: Efficient pattern matching in degenerate strings with the
 * Burrows–Wheeler transform
 * Copyright (C) 2018 Jacqueline W. Daykin, Richard Groult, Yannick Guesnet,
 * Thierry Lecroq, Arnaud Lefebvre, Martine Leonard, Laurent Mouchard,
 * Elise Prieur-Gaston, Bruce Watson
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <trees/range_tree.h>

namespace ranges {

range_tree::range_tree() : red_black_tree() {
  //std::cerr << "range_tree::range_tree()" << std::endl;
}

range_tree::range_tree(range_tree &&tree) : red_black_tree(std::move(tree)) {
//  std::cerr << "range_tree::range_tree(range_tree &&tree)" << std::endl;
}

range_tree& range_tree::operator=(range_tree &&tree) {
  //std::cerr << "range_tree::operator=(range_tree &&tree)" << std::endl;
  red_black_tree::operator=(std::move(tree));
  return *this;

}

void range_tree::tree_insert_node(red_black_tree_node *node) {
  red_black_tree_node *x;
  red_black_tree_node *y;
  range r = node->get_entry();

  y = root;
  x = root->get_left();
  while (x != nil && x->get_entry().get_low() - 1 != r.get_high() && x->get_entry().get_high() != r.get_low() - 1) {
    y = x;
    if (r < x->get_entry()) {
      x = x->get_left();
    } else {
      x = x->get_right();
    }
  }

  if (x == nil) {
    // Ajout d'un noeud
    node->set_parent(y);
    if ((y == root) || (node->get_entry() < y->get_entry())) {
      y->set_left(node);
    } else {
      y->set_right(node);
    }

    node->set_left(nil);
    node->set_right(nil);
    node->set_red();

    assert(!nil->is_red());

    tree_insert_fixup(node);
    ++num_nodes;
  } else {
    // Fusion de noeud : le nombre de noeud ne change pas sauf
    // si on fusionne trois noeuds
    if (x->get_entry().get_low() - 1 == r.get_high()) {
      r.set_high(x->get_entry().get_high());
      x->set_entry(r);
      y = get_maximum_of(x->get_left());
      if (y->get_entry().get_high() == r.get_low() - 1) {
        // l'intervalle touche deux intervalles (un de chaque coté)
        r.set_low(y->get_entry().get_low());
        delete_node(y);
        x->set_entry(r);
      }
    } else {
      r.set_low(x->get_entry().get_low());
      x->set_entry(r);
      y = get_minimum_of(x->get_right());
      if (y->get_entry().get_low() - 1 == r.get_high()) {
        // l'intervalle touche deux intervalles (un de chaque coté)
        r.set_high(y->get_entry().get_high());
        delete_node(y);
        x->set_entry(r);
      }
    }
    delete node;
  }
}

}
