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

#ifndef ACGT_MULTILETTER_H_
#define ACGT_MULTILETTER_H_

#include <vector>
#include <iostream>
#include <initializer_list>

namespace ml {

// Ici une multi-lettre est un octet où chacun des quatre premiers bits
// indique la présence d'une des quatre lettres.
// Les lettres sont 1, 2, 4 et 8
class acgt_multi_letter {
public:
  acgt_multi_letter();

  ~acgt_multi_letter();

  acgt_multi_letter(const acgt_multi_letter &l);
  acgt_multi_letter(const char letters);
  acgt_multi_letter(std::initializer_list<char> l);

  acgt_multi_letter& operator=(const acgt_multi_letter &l);
  acgt_multi_letter& operator=(acgt_multi_letter &&l);

	void add(const char &letter);
	bool contains(const char &letter) const;
	bool contains_some_letters(const acgt_multi_letter &multi_letter) const;

	bool is_equal(const acgt_multi_letter& ml) const;

	class const_iterator {
	  friend acgt_multi_letter;

	public:
    const_iterator &operator++() {
      ++pos;
      letters >>= 1;
      return *this;
    }

    bool operator!=(const const_iterator& it) {
      return it.letters == letters && it.pos == pos;
    }

    char operator*() {
      return letters & 1;
    }

  private:
    const_iterator(char letters, char pos) {
      this->letters = letters;
      this->pos = pos;
    }

    char letters;
    char pos;
  };

	const_iterator begin() const {
	  return const_iterator(letters, 0);
	}

  const_iterator end() const {
    return const_iterator(letters, 4);
  }

private:
	char letters;
};

bool operator==(const acgt_multi_letter& lhs, const acgt_multi_letter& rhs);

bool operator!=(const acgt_multi_letter& lhs, const acgt_multi_letter& rhs);

std::ostream& operator<<(std::ostream& stream, const acgt_multi_letter& l);

} /* namespace ml */


#endif /* ACGT_MULTILETTER_H_ */
