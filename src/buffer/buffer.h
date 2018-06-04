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

#ifndef BUFFER_H_
#define BUFFER_H_

#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <iostream>

namespace buffer {

/**
 * Un buffer offrant une partie de l'API des string
 * (en particulier, on dispose de la méthode length()).
 * Permet de manipuler indifféremment les string ou les buffer.
 */
template<typename char_type = unsigned char>
class buffer {
public:
    typedef char_type value_type;

    buffer() {
        size = 0;
        buf = nullptr;
    }

    buffer(std::size_t length) {
        size = length;
        buf = new value_type[size];
    }

    buffer(std::size_t length, const char_type *data) {
        size = length;
        buf = new value_type[size];
        for (std::size_t i = 0; i < length; ++i) {
          buf[i] = data[i];
        }
    }

    buffer(std::initializer_list<char_type> data) {
      size = data.size();
      buf = new value_type[size];
      size_t i = 0;
      for (auto c : data) {
          buf[i] = c;
          ++i;
      }
    }

    buffer(buffer&& o) {
        size = o.size;
        o.size = 0;
        delete[] buf;
        buf = o.buf;
        o.buf =  nullptr;
    }

    virtual ~buffer() {
//      std::cerr << "~buffer()" << std::endl;
      delete[] buf;
    }

    buffer& operator=(buffer&& o) {
        size = o.size;
        o.size = 0;
        delete[] buf;
        buf = o.buf;
        o.buf =  nullptr;
        return *this;
    }

    size_t length() const {
        return size;
    }

    value_type& operator[](size_t i) const {
        assert(buf != nullptr);
        assert(i < size);
        return buf[i];
    }

    const value_type* data() const {
        return buf;
    }

    value_type* data() {
        return buf;
    }

private:
    buffer(const buffer& src);
    buffer& operator=(const buffer& src);

private:
    std::size_t size;
    value_type* buf;
};

template<typename char_type>
std::ostream& operator<<(std::ostream& stream, const buffer<char_type>& b) {
  stream << "[ ";
  for (std::size_t i = 0; i < b.length(); ++i) {
    stream << b[i] << " ";
  }
  stream << "]";
  return stream;
}

} /* namespace words */

#endif /* BUFFER_H_ */
