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

#ifndef RANGE_TREE_H_
#define RANGE_TREE_H_

#include "range.h"
#include "red_black_tree.h"

namespace ranges {

class range_tree : public red_black_tree<range> {
public:
  range_tree();
  range_tree(range_tree &&tree);
  range_tree& operator=(range_tree &&tree);

  virtual ~range_tree() {
    //std::cerr << "~range_tree()" << std::endl;
  };

protected:
  virtual void tree_insert_node(red_black_tree_node *node);
};

}

#endif /* RANGE_TREE_H_ */
