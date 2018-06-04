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

#include "sais/sais.hxx"
#include "trees/range_tree.h"

#include <chrono>
#include <list>
#include <sdsl/vectors.hpp>

#ifndef SRC_DEGENERATE_SEARCH_DEGENERATE_SEARCH_HPP_
#define SRC_DEGENERATE_SEARCH_DEGENERATE_SEARCH_HPP_

template<typename letter_index_type, class multi_letter_type>
ranges::range_tree degenerate_backward_search_in_bwt(
    const buffer::buffer<letter_index_type> &x, const std::vector<multi_letter_type > &alpha_x,
    const buffer::buffer<letter_index_type> &bwt, const std::vector<multi_letter_type > &alpha_bwt);

template<typename letter_type>
void get_bucket_start(const buffer::buffer<letter_type> &bwt, size_t *C, size_t alpha_size);

template<typename letter_type>
size_t rank(int c, const buffer::buffer<letter_type> &bwt, size_t i);

template<typename letter_type>
void get_freq(const buffer::buffer<letter_type> &bwt, size_t *C);

/**
 * La seule occurence de 0 doit être à la dernière position dans text
 */
template<typename letter_index_type, class multi_letter_type>
std::vector<std::size_t> degenerate_backward_search(
    const buffer::buffer<letter_index_type> &text, const std::vector<multi_letter_type > &alpha_text,
    const buffer::buffer<letter_index_type> &pattern, const std::vector<multi_letter_type > &alpha_pattern) {

  buffer::buffer<unsigned char> bwt(text.length());
  int *SA = new int[text.length()];

  saisxx(text.data(), SA + 1, (int) text.length() - 1);
  SA[0] = text.length() - 1;
  for (size_t i = 0; i < text.length(); ++i) {
    bwt[i] = SA[i] == 0 ? 0 : text[SA[i] - 1];
  }

  std::chrono::high_resolution_clock::time_point t1, t2;
  std::chrono::duration<double> time_span;
  t1 = std::chrono::high_resolution_clock::now();
  ranges::range_tree result = degenerate_backward_search_in_bwt(pattern, alpha_pattern, bwt, alpha_text);
  t2 = std::chrono::high_resolution_clock::now();
  time_span = t2 - t1;
  std::cout << "Backward search: " << time_span.count() << std::endl;

  std::vector<std::size_t> v = std::vector<std::size_t>();
  for (auto r : result) {
    for (int p = r.get_low(); p <= r.get_high(); ++p) {
      v.push_back(SA[p]);
    }
  }

  delete[] SA;
  return v;
}


template<typename letter_index_type, class multi_letter_type>
ranges::range_tree degenerate_backward_search_in_bwt(
    const buffer::buffer<letter_index_type> &x, const std::vector<multi_letter_type > &alpha_x,
    const buffer::buffer<letter_index_type> &bwt, const std::vector<multi_letter_type > &alpha_bwt) {
  size_t m = x.length();
  ranges::range_tree I;

  size_t *C = new size_t[alpha_bwt.size() + 1](); // +1 pour pouvoir écrire range(C[c], C[c + 1] - 1) même quand c est la dernière lettre

  get_bucket_start(bwt, C, alpha_bwt.size());
  /*
  std::cerr << "C : " << std::endl;
  for (size_t i = 0; i < alpha_bwt.size(); ++i) {
    std::cerr << "C[" << i << "] = " << C[i] << " ";
  }
  std::cout << std::endl;
   */

  assert(m > 0);

  //std::cerr << "Initialisation..." << std::endl;
  //std::cerr << "x[m - 1] = " << alpha_x[ x[m - 1] ] << std::endl;
  for (int i = 0; i < (int) alpha_bwt.size(); ++i) {
    //std::cerr << "c = " << alpha_bwt[i] << std::endl;
    if (alpha_bwt[i].contains_some_letters(alpha_x[ x[m - 1] ])) {
      size_t r1 = C[i];
      size_t r2 = C[i + 1];
      if (r1 < r2) {
        //std::cerr << "Insert " << ranges::range(r1, r2 - 1) << std::endl;
        I.insert(ranges::range(r1, r2 - 1));
      }
    }
  }

  size_t k = m - 1;
  // size_t n'est pas signé, on ne peut donc écrire while (k >= 0) ...
  // on met donc une boucle do while au lieu d'un while
  if (I.empty() || k == 0) {
    delete[] C;
    return std::move(I);
  }

  ranges::range_tree I2;
  do {
    --k;
    I2 = ranges::range_tree();

    //std::cerr << "Boucle k = " << k << "..." << std::endl;
    for (auto r : I) {
      //std::cerr << "r : " << r << std::endl;
      for (int i = 0; i < (int) alpha_bwt.size(); ++i) {
        auto c = alpha_bwt[i];
        if (c.contains_some_letters(alpha_x[ x[k] ])) {
          size_t r1 = rank(i, bwt, r.get_low());
          size_t r2 = rank(i, bwt, r.get_high() + 1);
//          std::cerr << "c : " << c << std::endl;
//          std::cerr << "C[" << c << "] = " << C[i] << std::endl;
//          std::cerr << "rank('" << c << "', bwt, " << r.get_low() << ") : " << r1 << std::endl;
//          std::cerr << "rank('" << c << "', bwt, " << r.get_high() + 1 << ") : " << r2 << std::endl;
          if (r1 < r2) {
            //std::cerr << "Insert [" << C[c] + r1 << ", " << C[c] + r2 - 1<< "]" << std::endl;
            I2.insert(ranges::range(C[i] + r1, C[i] + r2 - 1));
          }
        }
      }
    }

    I = std::move(I2);
    //std::cerr << "I : " << std::endl;
    //for (auto r : I) {
    //  std::cerr << "[" << r.get_low() << ", " << r.get_high() << "]" << std::endl;
    //}

  } while (!I.empty() && k > 0);

  delete[] C;
  return std::move(I);
}

/**
 * Retourne C tel que C[i] est l'indice de début du bucket de la lettre i, 0 <= i < alpha_size)
 * C[alpha_size] vaut bwt.length()
 */
template<typename letter_type>
void get_bucket_start(const buffer::buffer<letter_type> &bwt, size_t *C, size_t alpha_size) {
  get_freq(bwt, C);

  size_t freq = 0; // sum_{k = 0 .. i - 1}C[k]
  for (size_t i = 0; i <= alpha_size; ++i) {
    size_t t = C[i];
    C[i] = freq;
    freq += t;
  }
}

/**
 * Retourne le nombre de fois où la lettre c est apparue dans bwt[0 .. i - 1]
 * Implémentation naïve, comment faire en O(1) sans prendre plus de O(n) espace supplémentaire ?
 * Diffère légèrement de la définition de Thierry où il prend bwt[0 .. i]
 */
template<typename letter_type>
size_t rank(int c, const buffer::buffer<letter_type> &bwt, size_t i) {
  size_t count = 0;
  size_t j = 0;
  while (j < i) {
    count += bwt[j] == c;
    ++j;
  }
  return count;
}

/**
 * Retourne C tel que C[i] = |bwt|_i (nombre de i dans bwt, , 0 <= i < alpha_size)
 * C[alpha_size] est indéfini
 * En entrée C doit être initialisé à 0
 */
template<typename letter_type>
void get_freq(const buffer::buffer<letter_type> &bwt, size_t *C) {
  for (size_t i = 0; i < bwt.length(); ++i) {
    ++C[ (size_t) bwt[i] ];
  }
}

template<typename letter_index_type, class multi_letter_type>
ranges::range_tree degenerate_backward_search_in_bwt2(
    const buffer::buffer<letter_index_type> &x, const std::vector<multi_letter_type > &alpha_x,
    const buffer::buffer<letter_index_type> &bwt, const sdsl::rank_support_v<> *rs,
    const std::vector<multi_letter_type > &alpha_bwt) {
  size_t m = x.length();
  ranges::range_tree I;

  size_t *C = new size_t[alpha_bwt.size() + 1](); // +1 pour pouvoir écrire range(C[c], C[c + 1] - 1) même quand c est la dernière lettre

  get_bucket_start(bwt, C, alpha_bwt.size());
  /*
  std::cerr << "C : " << std::endl;
  for (size_t i = 0; i < alpha_bwt.size(); ++i) {
    std::cerr << "C[" << i << "] = " << C[i] << " ";
  }
  std::cout << std::endl;
   */

  assert(m > 0);

  //std::cerr << "Initialisation..." << std::endl;
  //std::cerr << "x[m - 1] = " << alpha_x[ x[m - 1] ] << std::endl;
  for (int i = 0; i < (int) alpha_bwt.size(); ++i) {
    //std::cerr << "c = " << alpha_bwt[i] << std::endl;
    if (alpha_bwt[i].contains_some_letters(alpha_x[ x[m - 1] ])) {
      size_t r1 = C[i];
      size_t r2 = C[i + 1];
      if (r1 < r2) {
        //std::cerr << "Insert " << ranges::range(r1, r2 - 1) << std::endl;
        I.insert(ranges::range(r1, r2 - 1));
      }
    }
  }

  size_t k = m - 1;
  // size_t n'est pas signé, on ne peut donc écrire while (k >= 0) ...
  // on met donc une boucle do while au lieu d'un while
  if (I.empty() || k == 0) {
    delete[] C;
    return std::move(I);
  }

  ranges::range_tree I2;
  do {
    --k;
    I2 = ranges::range_tree();

//    std::cerr << "Boucle k = " << k << "..." << std::endl;
//    std::cerr << "Taille de I: " << I.get_num_nodes() << std::endl;
    for (auto r : I) {
      //std::cerr << "r : " << r << std::endl;
      for (int i = 0; i < (int) alpha_bwt.size(); ++i) {
        auto c = alpha_bwt[i];
        if (c.contains_some_letters(alpha_x[ x[k] ])) {
          size_t r1 = rs[i](r.get_low());
          size_t r2 = rs[i](r.get_high() + 1);

//          std::cerr << "c : " << c << std::endl;
//          std::cerr << "C[" << c << "] = " << C[i] << std::endl;
//          std::cerr << "rank('" << c << "', bwt, " << r.get_low() << ") : " << r1 << std::endl;
//          std::cerr << "rank('" << c << "', bwt, " << r.get_high() + 1 << ") : " << r2 << std::endl;
          if (r1 < r2) {
            //std::cerr << "Insert [" << C[c] + r1 << ", " << C[c] + r2 - 1<< "]" << std::endl;
            I2.insert(ranges::range(C[i] + r1, C[i] + r2 - 1));
          }
        }
      }
    }

    I = std::move(I2);
//    std::cerr << "I : " << std::endl;
//    for (auto r : I) {
//      std::cerr << "[" << r.get_low() << ", " << r.get_high() << "]" << std::endl;
//    }

  } while (!I.empty() && k > 0);

  delete[] C;
  return std::move(I);
}

template<typename letter_index_type, class multi_letter_type>
std::list<ranges::range> degenerate_backward_search_in_bwt3(
    const buffer::buffer<letter_index_type> &x, const std::vector<multi_letter_type > &alpha_x,
    const buffer::buffer<letter_index_type> &bwt, const sdsl::rank_support_v<> *rs,
    const std::vector<multi_letter_type > &alpha_bwt) {
  size_t m = x.length();
  std::list<ranges::range> I;

  size_t *C = new size_t[alpha_bwt.size() + 1](); // +1 pour pouvoir écrire range(C[c], C[c + 1] - 1) même quand c est la dernière lettre

  get_bucket_start(bwt, C, alpha_bwt.size());
  /*
  std::cerr << "C : " << std::endl;
  for (size_t i = 0; i < alpha_bwt.size(); ++i) {
    std::cerr << "C[" << i << "] = " << C[i] << " ";
  }
  std::cout << std::endl;
   */

  assert(m > 0);

  //std::cerr << "Initialisation..." << std::endl;
  //std::cerr << "x[m - 1] = " << alpha_x[ x[m - 1] ] << std::endl;
  for (int i = 0; i < (int) alpha_bwt.size(); ++i) {
    //std::cerr << "c = " << alpha_bwt[i] << std::endl;
    if (alpha_bwt[i].contains_some_letters(alpha_x[ x[m - 1] ])) {
      size_t r1 = C[i];
      size_t r2 = C[i + 1];
      if (r1 < r2) {
        //std::cerr << "Insert " << ranges::range(r1, r2 - 1) << std::endl;
        I.push_back(ranges::range(r1, r2 - 1));
      }
    }
  }

  size_t k = m - 1;
  // size_t n'est pas signé, on ne peut donc écrire while (k >= 0) ...
  // on met donc une boucle do while au lieu d'un while
  if (I.empty() || k == 0) {
    delete[] C;
    return std::move(I);
  }

  std::list<ranges::range> I2;
  do {
    --k;
    I2 = std::list<ranges::range>();

    //std::cerr << "Boucle k = " << k << "..." << std::endl;
    for (auto r : I) {
      //std::cerr << "r : " << r << std::endl;
      for (int i = 0; i < (int) alpha_bwt.size(); ++i) {
        auto c = alpha_bwt[i];
        if (c.contains_some_letters(alpha_x[ x[k] ])) {
          size_t r1 = rs[i](r.get_low());
          size_t r2 = rs[i](r.get_high() + 1);

//          std::cerr << "c : " << c << std::endl;
//          std::cerr << "C[" << c << "] = " << C[i] << std::endl;
//          std::cerr << "rank('" << c << "', bwt, " << r.get_low() << ") : " << r1 << std::endl;
//          std::cerr << "rank('" << c << "', bwt, " << r.get_high() + 1 << ") : " << r2 << std::endl;
          if (r1 < r2) {
            //std::cerr << "Insert [" << C[c] + r1 << ", " << C[c] + r2 - 1<< "]" << std::endl;
            I2.push_back(ranges::range(C[i] + r1, C[i] + r2 - 1));
          }
        }
      }
    }

    I = std::move(I2);
//    std::cerr << "I : " << std::endl;
//    for (auto r : I) {
//      std::cerr << "[" << r.get_low() << ", " << r.get_high() << "]" << std::endl;
//    }

  } while (!I.empty() && k > 0);

  delete[] C;
  return std::move(I);
}

template<typename letter_index_type, class multi_letter_type>
class preproc_backward_search2 {
public:
  preproc_backward_search2(const buffer::buffer<letter_index_type> &text, const std::vector<multi_letter_type > &alpha_text) {
    bwt = buffer::buffer<unsigned char>(text.length());
    SA = new int[text.length()];

    bbwt = new sdsl::bit_vector[alpha_text.size()];
    for (std::size_t c = 0; c < alpha_text.size(); ++c) {
      bbwt[c] = sdsl::bit_vector(text.length()); // + 1 ???
    }

    saisxx(text.data(), SA + 1, (int) text.length() - 1);
    SA[0] = text.length() - 1;
    for (size_t i = 0; i < text.length(); ++i) {
      letter_index_type c = SA[i] == 0 ? 0 : text[SA[i] - 1];
      bwt[i] = c;
      bbwt[c][i] = 1;
    }
    rs = new sdsl::rank_support_v<>[alpha_text.size()];
    for (std::size_t c = 0; c < alpha_text.size(); ++c) {
  //    rs[c].init(&(bbwt[c]));
      sdsl::util::assign(rs[c], sdsl::rank_support_v<>(&(bbwt[c])));
    }
  }

  ~preproc_backward_search2() {
//    std::cerr << "~preproc_backward_search2()" << std::endl;
    delete[] SA;
    delete[] bbwt;
    delete[] rs;
  }

  buffer::buffer<letter_index_type> bwt;
  int *SA;
  sdsl::bit_vector *bbwt;
  sdsl::rank_support_v<> *rs;
};

/**
 * Version optimisée avec une fonction rank en O(1)
 * Utilise des "succint structures" (des vecteurs de bits)
 * La seule occurence de 0 doit être à la dernière position dans text
 */
template<typename letter_index_type, class multi_letter_type>
std::vector<std::size_t> degenerate_backward_search2(
    const buffer::buffer<letter_index_type> &text, const std::vector<multi_letter_type > &alpha_text,
    const buffer::buffer<letter_index_type> &pattern, const std::vector<multi_letter_type > &alpha_pattern) {

  preproc_backward_search2<letter_index_type, multi_letter_type> pp(text, alpha_text);

  std::chrono::high_resolution_clock::time_point t1, t2;
  std::chrono::duration<double> time_span;
  t1 = std::chrono::high_resolution_clock::now();
  ranges::range_tree result = degenerate_backward_search_in_bwt2(pattern, alpha_pattern, pp.bwt, pp.rs, alpha_text);

  std::vector<std::size_t> v = std::vector<std::size_t>();
  for (auto r : result) {
    for (int p = r.get_low(); p <= r.get_high(); ++p) {
      v.push_back(pp.SA[p]);
    }
  }

  t2 = std::chrono::high_resolution_clock::now();
  time_span = t2 - t1;
  std::cout << "Backward search: " << time_span.count() << std::endl;

  return v;
}


#endif /* SRC_DEGENERATE_SEARCH_DEGENERATE_SEARCH_HPP_ */
