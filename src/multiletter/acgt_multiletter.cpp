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

#include "acgt_multiletter.h"

#include <utility>
#include <algorithm>

namespace ml {

  bool operator==(const acgt_multi_letter& lhs, const acgt_multi_letter& rhs) {
    return lhs.is_equal(rhs);
  }

  bool operator!=(const acgt_multi_letter& lhs, const acgt_multi_letter& rhs) {
    return !(lhs == rhs);
  }

  acgt_multi_letter::acgt_multi_letter() : letters(0) {
  }

  acgt_multi_letter::acgt_multi_letter(const char letters) : letters(letters) {

  }

  acgt_multi_letter::acgt_multi_letter(std::initializer_list<char> l) : letters(0) {
    for (char c : l) {
      letters |= c;
    }
  }

  acgt_multi_letter::~acgt_multi_letter() {
  }

  acgt_multi_letter::acgt_multi_letter(const acgt_multi_letter &l) {
    letters = l.letters;
  }

  acgt_multi_letter& acgt_multi_letter::operator=(const acgt_multi_letter &l) {
    letters = l.letters;
    return *this;
  }

  acgt_multi_letter& acgt_multi_letter::operator=(acgt_multi_letter &&l) {
    letters = std::move(l.letters);
    l.letters = 0;
    return *this;
  }

  void acgt_multi_letter::add(const char &letter) {
    letters |= letter;
  }

  bool acgt_multi_letter::contains(const char &letter) const {
    return (letters & letter) != 0;
  }

  bool acgt_multi_letter::contains_some_letters(const acgt_multi_letter &multi_letter) const {
    return (letters & multi_letter.letters) != 0;
  }

  bool acgt_multi_letter::is_equal(const acgt_multi_letter& ml) const {
    return letters == ml.letters;
  }

  std::ostream& operator<<(std::ostream& stream, const acgt_multi_letter& l) {
    /*
    stream << "[";
    if (l.contains(1)) {
      stream << "a";
    }
    if (l.contains(2)) {
      stream << "c";
    }
    if (l.contains(4)) {
      stream << "g";
    }
    if (l.contains(8)) {
      stream << "t";
    }
    stream << "]";
    */

    stream << (l.contains(1) ? "1" : "0");
    stream << " ";
    stream << (l.contains(2) ? "1" : "0");
    stream << " ";
    stream << (l.contains(4) ? "1" : "0");
    stream << " ";
    stream << (l.contains(8) ? "1" : "0");
    stream << std::endl;

    return stream;
  }

} /* namespace ml */
