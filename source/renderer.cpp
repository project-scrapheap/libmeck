
/* meck - Mediocre (Entertainment Creation Kit)
 * Copyright(c) 2016, Erik Edlund <erik.edlund@32767.se>
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 *  * Neither the name of Erik Edlund, nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ----
 * 
 * Parts of meck are based on libSDL2pp - "C++11 bindings/wrapper for SDL2"
 * by Dmitry Marakasov <amdmi3@amdmi3.ru>. libSDL2pp can be found at:
 * 
 *     https://github.com/libSDL2pp/libSDL2pp
 * 
 * Code taken from libSDL2pp is most often altered and is not necessarily
 * representative of the original software.
 * 
 * libSDLpp is made available under the following conditions:
 * 
 * Copyright (C) 2013-2015 Dmitry Marakasov <amdmi3@amdmi3.ru>
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 * 
 *  2. Altered source versions must be plainly marked as such, and must not
 *     be misrepresented as being the original software.
 * 
 *  3. This notice may not be removed or altered from any source distribution.
 */

#include <meck/renderer.hpp>

namespace meck {

renderer&
renderer::fill_copy(
	texture& txtre,
	const boost::optional<rect>& srcrect,
	const boost::optional<rect>& dstrect,
	const point& offset,
	const int flip
) {
	// Resolve rectangles.
	rect src = srcrect? *srcrect:
		rect(0, 0, txtre.get_width(), txtre.get_height());
	rect dst = dstrect? *dstrect:
		rect(0, 0, get_output_width(), get_output_height());
	
	// Rectangle for a single tile.
	rect start_tile(
		offset.x(),
		offset.y(),
		src.w(),
		src.h()
	);
	
	// Ensure tile is leftmost and topmost.
	if (start_tile.x() + start_tile.w() <= 0) {
		start_tile.x(start_tile.x() + ((-start_tile.x())
			/ start_tile.w() * start_tile.w()));
	}
	if (start_tile.x() > 0) {
		start_tile.x(start_tile.x() - ((start_tile.x() + start_tile.w() - 1)
			/ start_tile.w() * start_tile.w()));
	}
	if (start_tile.y() + start_tile.h() <= 0) {
		start_tile.y(start_tile.y() + ((-start_tile.y())
			/ start_tile.h() * start_tile.h()));
	}
	if (start_tile.y() > 0) {
		start_tile.y(start_tile.y() - ((start_tile.y() + start_tile.h() - 1)
			/ start_tile.h() * start_tile.h()));
	}
	
	// Render tile array.
	
	for (int y = start_tile.y(); y < dst.h(); y += start_tile.h()) {
		for (int x = start_tile.x(); x < dst.w(); x += start_tile.w()) {
			rect tile_src = src;
			rect tile_dst(x, y, start_tile.w(), start_tile.h());
			
			// Clamp with dstrect
			int xunderflow = -x;
			if (xunderflow > 0) {
				tile_src.w(tile_src.w() - xunderflow);
				tile_src.x(tile_src.x() + xunderflow);
				tile_dst.w(tile_dst.w() - xunderflow);
				tile_dst.x(tile_dst.x() + xunderflow);
			}
			int yunderflow = -y;
			if (yunderflow > 0) {
				tile_src.h(tile_src.h() - yunderflow);
				tile_src.y(tile_src.y() + yunderflow);
				tile_dst.h(tile_dst.h() - yunderflow);
				tile_dst.y(tile_dst.y() + yunderflow);
			}
			int xoverflow = tile_dst.x() + tile_dst.w() - dst.w();
			if (xoverflow > 0) {
				tile_src.w(tile_src.w() - xoverflow);
				tile_dst.w(tile_dst.w() - xoverflow);
			}
			int yoverflow = tile_dst.y() + tile_dst.h() - dst.h();
			if (yoverflow > 0) {
				tile_src.h(tile_src.h() - yoverflow);
				tile_dst.h(tile_dst.h() - yoverflow);
			}

			// Make tile_dst absolute.
			tile_dst.x(tile_dst.x() + dst.x());
			tile_dst.y(tile_dst.y() + dst.y());
			
			if (flip) {
				// Mirror tile_src inside src to take flipping into account.
				if (flip & SDL_FLIP_HORIZONTAL)
					tile_src.x(src.w() - tile_src.x() - tile_src.w());
				if (flip & SDL_FLIP_VERTICAL)
					tile_src.y(src.h() - tile_src.y() - tile_src.h());

				copy(txtre, tile_src, tile_dst, 0.0, boost::none, flip);
			} else {
				copy(txtre, tile_src, tile_dst);
			}
		}
	}
	return *this;
}

} // namespace:meck

