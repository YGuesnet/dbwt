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

#include "datatools.h"
#include "multiletter/acgt_multiletter.h"

#include <random>
#include <fstream>

buffer::buffer<unsigned char> generate_degenerate_text(std::size_t text_length) {
  buffer::buffer<unsigned char> text(text_length + 1);
  for (size_t i = 0; i < text_length; ++i) {
    text[i] = ((unsigned char) (((long) std::rand() * 15) / ((long) RAND_MAX + 1))) + 1;
  }
  text[text_length] = 0;

  return text;
}

buffer::buffer<unsigned char> generate_solid_text(std::size_t text_length) {
  std::random_device r;
  std::default_random_engine generator(r());
  std::uniform_int_distribution<std::size_t> distribution(0, 3);

  buffer::buffer<unsigned char> text(text_length + 1);
  for (size_t i = 0; i < text_length; ++i) {
    int pos = distribution(generator);
    text[i] = 1 << pos;
  }

  text[text_length] = 0;

  return text;
}

buffer::buffer<unsigned char> generate_conservative_text(std::size_t text_length, std::size_t num_degenerate_letters) {
  buffer::buffer<unsigned char> text = generate_solid_text(text_length);

  std::random_device r;
  std::default_random_engine generator(r());
  std::uniform_int_distribution<std::size_t> distribution(0, text_length - 1);

  for (std::size_t i = 0; i < num_degenerate_letters; ++i) {
    std::size_t pos;
    do {
      pos = distribution(generator);
    } while (text[pos] != 1 && text[pos] != 2 && text[pos] != 4 && text[pos] != 8);
    do {
      text[pos] = ((unsigned char) (((long) std::rand() * 15) / ((long) RAND_MAX + 1))) + 1;
    } while (text[pos] == 1 || text[pos] == 2 || text[pos] == 4 || text[pos] == 8);
  }

  return text;
}

void gen_pattern(size_t num, size_t length, size_t num_degen) {
  for (size_t i = 0; i < num; ++i) {
    buffer::buffer<unsigned char> buf = generate_conservative_text(length, num_degen);
    std::cout << "> pattern " << i + 1 << " " << num_degen << std::endl;
    for (size_t i = 0; i < length; ++i) {
      std::cout << ml::acgt_multi_letter(buf[i]);
    }
  }
}

void gen_text(size_t num, size_t length) {
  for (size_t i = 0; i < num; ++i) {
    buffer::buffer<unsigned char> buf = generate_solid_text(length);

    std::cout << "> text" << i + 1 << std::endl;
    size_t c = 0;
    size_t max_c = 70;
    for (size_t i = 0; i < length; ++i) {
      if (c == max_c) {
        c = 0;
        std::cout << std::endl;
      }
      switch (buf[i]) {
      case 1:
        std::cout << "A";
        break;
      case 2:
        std::cout << "C";
        break;
      case 4:
        std::cout << "G";
        break;
      case 8:
        std::cout << "T";
        break;
      default:
        throw std::runtime_error("invalid value");
        break;
      }
      ++c;
    }
    std::cout << std::endl;
  }
}

buffer::buffer<unsigned char> read_pattern(std::ifstream &f) {
  char c;
  if (!f.get(c) || c != '>') {
    throw std::runtime_error("invalid format");
  }

  std::string line;
  if (!getline(f, line)) {
    throw std::runtime_error("invalid format");
  }

  std::vector<unsigned char> v;
  while (f.get(c) && c != '>') {
    f.unget();
    std::string line;
    if (!getline(f, line)) {
      throw std::runtime_error("invalid format");
    }
    if ((line.length() != 7 && line.length() != 8) || (line.length() == 8 && line[7] != '\r')) {
      throw std::runtime_error("invalid format");
    }

    unsigned char l = 0;
    unsigned char bit;
    int dec = 0;

    bit = line[0] - '0';
    if (bit > 1) {
      throw std::runtime_error("invalid format");
    }
    l += bit << dec;
    ++dec;
    if (line[1] != ' ') {
      throw std::runtime_error("invalid format");
    }
    bit = line[2] - '0';
    if (bit > 1) {
      throw std::runtime_error("invalid format");
    }
    l += bit << dec;
    ++dec;
    if (line[3] != ' ') {
      throw std::runtime_error("invalid format");
    }
    bit = line[4] - '0';
    if (bit > 1) {
      throw std::runtime_error("invalid format");
    }
    l += bit << dec;
    ++dec;
    if (line[5] != ' ') {
      throw std::runtime_error("invalid format");
    }
    bit = line[6] - '0';
    if (bit > 1) {
      throw std::runtime_error("invalid format");
    }
    l += bit << dec;
    ++dec;

    v.push_back(l);
  }

  if (!f.eof()) {
    f.unget();
  }

  buffer::buffer<unsigned char> buf(v.size(), v.data());

  return buf;
}

buffer::buffer<unsigned char> read_text(std::ifstream &f) {
  char c;
  if (!f.get(c) || c != '>') {
    throw std::runtime_error("invalid format");
  }

  std::string line;
  if (!getline(f, line)) {
    throw std::runtime_error("invalid format");
  }

  std::vector<unsigned char> v;
  while (f.get(c) && c != '>') {
    switch (c) {
    case 'A':
      v.push_back(1);
      break;
    case 'C':
      v.push_back(2);
      break;
    case 'G':
      v.push_back(4);
      break;
    case 'T':
      v.push_back(8);
      break;
    case '\n':
    case '\r':
      break;
    default:
      throw std::runtime_error("invalid format");
    }
  }
  v.push_back(0);

  if (!f.eof()) {
    f.unget();
  }

  buffer::buffer<unsigned char> buf(v.size(), v.data());
  return buf;
}

