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

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include <boost/program_options.hpp>

#include "datatools.h"
#include "multiletter/acgt_multiletter.h"
#include "degenerate_search/degenerate_search.hpp"

void build_acgt_multiletters(std::vector<ml::acgt_multi_letter> &letters);

int main(int argc, char **argv) {
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

  namespace po = boost::program_options;

  std::string pattern_file;
  std::string text_file;

  po::options_description desc("Allowed options");
  desc.add_options()
      ("help,h", "produce help message")
      ("pattern-file,p", po::value<std::string>(&pattern_file), "path of the pattern file")
      ("input-file,i", po::value<std::string>(&text_file), "path of the text file");


  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  if (!vm.count("pattern-file") || !vm.count("input-file")) {
    std::cerr << desc << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream pf(pattern_file);
  if (!pf.is_open()) {
    throw std::runtime_error("unable to open pattern file");
  }

  std::ifstream tf(text_file);
  if (!tf.is_open()) {
    throw std::runtime_error("unable to open text file");
  }

  buffer::buffer<unsigned char> pbuf = read_pattern(pf);
  buffer::buffer<unsigned char> tbuf = read_text(tf);

  pf.close();
  tf.close();

  std::vector<ml::acgt_multi_letter> letters(16);
  build_acgt_multiletters(letters);

  std::chrono::high_resolution_clock::time_point ts = std::chrono::high_resolution_clock::now();
  preproc_backward_search2<unsigned char, ml::acgt_multi_letter> pp(tbuf, letters);
  ranges::range_tree r2 = degenerate_backward_search_in_bwt2(pbuf, letters, pp.bwt, pp.rs, letters);
  std::chrono::duration<double> time_search = std::chrono::high_resolution_clock::now() - ts;

  std::vector<std::size_t> v;
  for (auto r : r2) {
    for (int p = r.get_low(); p <= r.get_high(); ++p) {
      v.push_back(pp.SA[p]);
    }
  }
  
  std::chrono::duration<double> time_span = std::chrono::high_resolution_clock::now() - t1;

  std::cout << "Number of results: " << v.size() << std::endl;
  std::cout << "Search time: " << time_search.count() << " sec" << std::endl;
  std::cout << "Total elapse time: " << time_span.count() << " sec" << std::endl;

  return EXIT_SUCCESS;
}

void build_acgt_multiletters(std::vector<ml::acgt_multi_letter> &letters) {
  for (int i = 0; i < 16; ++i) {
    letters[i] = ml::acgt_multi_letter(i);
  }
}

