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

#ifndef MULTILETTER_H_
#define MULTILETTER_H_

#include <vector>
#include <iostream>
#include <initializer_list>

namespace ml {

template <class letter_type = char>
class multi_letter {
public:
  multi_letter();

  ~multi_letter();

  multi_letter(const multi_letter &l);
	multi_letter(multi_letter &&l);
	multi_letter(const std::vector<letter_type> &letters);
	multi_letter(std::initializer_list<letter_type> l);

	multi_letter& operator=(const multi_letter &l);
	multi_letter& operator=(multi_letter &&l);

	void add(const letter_type &letter);
	bool contains(const letter_type &letter) const;
	bool contains_some_letters(const multi_letter &multi_letter) const;

	bool is_equal(const multi_letter<letter_type>& ml) const;

	class const_iterator {
	  friend multi_letter;
  public:
    const_iterator &operator++() {
      ++it;
      return *this;
    }

    bool operator!=(const const_iterator& it) {
      return this->it != it.it;
    }

    const letter_type &operator*() {
      return *it;
    }

  private:
    const_iterator(typename std::vector<letter_type>::const_iterator it) {
      this->it = it;
    }

    typename std::vector<letter_type>::const_iterator it;
  };

	const_iterator begin() const {
	  return const_iterator(letters.begin());
	}

  const_iterator end() const {
    return const_iterator(letters.end());
  }


private:
	std::vector<letter_type> letters;
};

template <class letter_type = char>
bool operator==(const multi_letter<letter_type>& lhs, const multi_letter<letter_type>& rhs){
	return lhs.is_equal(rhs);
}

template <class letter_type = char>
bool operator!=(const multi_letter<letter_type>& lhs, const multi_letter<letter_type>& rhs){
	return !(lhs == rhs);
}

} /* namespace ml */

#include <utility>
#include <algorithm>

namespace ml {
	template <class letter_type>
	multi_letter<letter_type>::multi_letter() : letters() {
	  //std::cerr << "multi_letter(): " << this  << std::endl;
	}

	template <class letter_type>
	multi_letter<letter_type>::multi_letter(std::initializer_list<letter_type> l) : letters(l) {
	  //std::cerr << "multi_letter(std::initializer_list<letter_type> l): " << this  << std::endl;
	}

  template <class letter_type>
  multi_letter<letter_type>::~multi_letter() {
    //std::cerr << "~multi_letter(): " << this  << std::endl;
  }

  template <class letter_type>
  multi_letter<letter_type>::multi_letter(multi_letter &&l) {
    //std::cerr << "multi_letter(multi_letter &&l): " << this << std::endl;
    std::swap(letters, l.letters);
  }

  template <class letter_type>
  multi_letter<letter_type>::multi_letter(const multi_letter &l) {
    //std::cerr << "multi_letter(const multi_letter &l): " << this << std::endl;
    letters = l.letters;
  }

  template <class letter_type>
  multi_letter<letter_type>& multi_letter<letter_type>::operator=(const multi_letter &l) {
    //std::cerr << "multi_letter<letter_type>::operator=(const multi_letter &l): " << this  << std::endl;
    letters = l.letters;
    return *this;
  }

  template <class letter_type>
  multi_letter<letter_type>& multi_letter<letter_type>::operator=(multi_letter &&l) {
    //std::cerr << "multi_letter<letter_type>::operator=(multi_letter &&l): " << this  << std::endl;
    letters = std::move(l.letters);
    l.letters = std::vector<letter_type>();
    return *this;
  }

	template <class letter_type>
	multi_letter<letter_type>::multi_letter(const std::vector<letter_type> &letters) {
	  //std::cerr << "multi_letter(const std::vector<letter_type> &letters): " << this  << std::endl;
		for (auto l : letters) {
			add(l);
		}
	}

	template <class letter_type>
	void multi_letter<letter_type>::add(const letter_type &letter) {
		letters.push_back(letter);
		typename std::vector<letter_type>::size_type i = letters.size() - 1;
		while (i > 0 && letters[i] < letters[i - 1]) {
			std::swap(letters[i- 1], letters[i]);
			--i;
		}
	}

	template <class letter_type>
	bool multi_letter<letter_type>::contains(const letter_type &letter) const {
		return std::find(letters.begin(), letters.end(), letter) != letters.end();
	}

	template <class letter_type>
  bool multi_letter<letter_type>::contains_some_letters(const multi_letter &multi_letter) const {
	  // TODO: les lettres étant triées on peut faire plus efficace...
	  for (auto &l : letters) {
	    if (multi_letter.contains(l)) {
	      return true;
	    }
	  }
	  return false;
	}

	template <class letter_type>
	bool multi_letter<letter_type>::is_equal(const multi_letter<letter_type>& ml) const {
		typename std::vector<letter_type>::const_iterator it = ml.letters.begin();
		for (auto &l : letters) {
			if (it == ml.letters.end() || l != *it) {
				return false;
			}
			++it;
		}
		return it == ml.letters.end();
	}

  template <class letter_type>
  std::ostream& operator<<(std::ostream& stream, const multi_letter<letter_type>& l) {
    stream << "[ ";
    for (auto c : l) {
      stream << c << " ";
    }
    stream << "]";

    return stream;
  }

} /* namespace ml */


#endif /* MULTILETTER_H_ */
