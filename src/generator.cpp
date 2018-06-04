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

#include <ctime>
#include <stdexcept>
#include <fstream>

#include <boost/program_options.hpp>

void gen_pattern(size_t num, size_t length, size_t num_degen);
void gen_text(size_t num, size_t length);

buffer::buffer<unsigned char> read_pattern(std::ifstream &f);
buffer::buffer<unsigned char> read_text(std::ifstream &f);

int main(int argc, char **argv) {
  namespace po = boost::program_options;

  std::string type;
  size_t length;
  size_t degen;
  size_t num;
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help,h", "produce help message")
      ("type,t", po::value<std::string>(&type)->default_value("text"), "set string to produce (text or pattern)")
      ("length,l", po::value<std::size_t>(&length), "string length")
      ("num,n", po::value<std::size_t>(&num)->default_value(1), "number of string to produce")
      ("degen,d", po::value<std::size_t>(&degen), "number of degenerate letters (only for pattern)");


  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return EXIT_SUCCESS;
  }

  if (type != "text" && type != "pattern") {
    throw std::runtime_error("invalid argument");
  }

  if (type == "text" && vm.count("degen")) {
    throw std::runtime_error("degen option is only valid for patterns");
  }

  if (type == "text") {
    gen_text(num, length);
  } else {
    gen_pattern(num, length, degen);
  }

  //gen_pattern(10, 10, 2);
  //gen_text(500);

  /*
  std::ifstream f("pattern.txt");
  if (!f.is_open()) {
    throw std::runtime_error("unable to open file");
  }

  while (!f.eof()) {
    buffer::buffer<unsigned char> buf = read_pattern(f);

    for (size_t i = 0; i < buf.length(); ++i) {
      std::cout << (int) buf[i] << std::endl;
    }
  }
  f.close();
  */

  /*
  std::ifstream f("text.txt");
  if (!f.is_open()) {
    throw std::runtime_error("unable to open file");
  }

  while (!f.eof()) {
    buffer::buffer<unsigned char> buf = read_text(f);

    for (size_t i = 0; i < buf.length(); ++i) {
      std::cout << (int) buf[i];
    }
  }
  f.close();
  */

  return EXIT_SUCCESS;
}
