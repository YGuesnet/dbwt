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

#ifndef SRC_DATAGEN_H_
#define SRC_DATAGEN_H_

#include "buffer/buffer.h"

#include <iostream>

buffer::buffer<unsigned char> generate_degenerate_text(std::size_t text_length);

buffer::buffer<unsigned char> generate_solid_text(std::size_t text_length);

buffer::buffer<unsigned char> generate_conservative_text(std::size_t text_length, std::size_t num_degenerate_letters);

void gen_pattern(size_t num, size_t length, size_t num_degen);

void gen_text(size_t num, size_t length);

buffer::buffer<unsigned char> read_pattern(std::ifstream &f);

buffer::buffer<unsigned char> read_text(std::ifstream &f);

#endif /* SRC_DATAGEN_H_ */
