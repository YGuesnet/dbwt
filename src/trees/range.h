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

#ifndef RANGE_H_
#define RANGE_H_

#include <string>
#include <cassert>
#include <iostream>

namespace ranges {

class range {
public:
  range(int left, int right) : low(left), high(right) {
    assert(left <= right);
  }

  ~range() {
  }

  int get_low() const {
    return low;
  }

  int get_high() const {
    return high;
  }

  void set_low(int left) {
    low = left;
  }

  void set_high(int right) {
    high = right;
  }

  operator std::string() const {
    return "[" + std::to_string(low) + ", " + std::to_string(high) + "]";
  }

  /**
   * @return this->right == r.left
   */
  bool left_adjacent_to(const range& r) const {
    return this->high == r.low;
  }

  /**
   * @return this->left == r.right
   */
  bool right_adjacent_to(const range& r) const {
    return this->low == r.high;
  }

  bool overlaps(const range& r) const {
    return this->low <= r.high && r.low <= this->high;
  }

  bool is_to_the_left_of(const range& r) const {
    return this->high < r.low;
  }

  bool is_to_the_right_of(const range& r) const {
    return r.is_to_the_left_of(*this);
  }

  bool operator==(const range& r) const {
    return this->high == r.high && this->low == r.low;
  }

  bool operator!=(const range& r) const {
    return !(*this == r);
  }

  bool operator<(const range& r) const {
    return this->low < r.low || (this->low == r.low && this->high < r.high);
  }

  bool operator>(const range& r) const {
    return r < *this;
  }

  bool operator<=(const range& r) const {
    return *this < r || *this == r;
  }
  bool operator>=(const range& r) const {
    return *this > r || *this == r;
  }

private:
  int low;
  int high;
};

}

std::ostream& operator<<(std::ostream& stream, const ranges::range& r);

#endif /* RANGE_H_ */
