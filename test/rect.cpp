
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

#include <meck.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE(
	"test_rect",
	"[meck::rect]"
) {
	using meck::point;
	using meck::rect;
	
	meck::scoped_sdl sdl(MECK_SDL_DEFAULT_INIT);
	
	SECTION("basic constructors and methods work") {
		rect r(1, 2, 3, 4);

		REQUIRE(r.x() == 1);
		REQUIRE(r.y() == 2);
		REQUIRE(r.w() == 3);
		REQUIRE(r.h() == 4);
		
		REQUIRE(r == rect(1, 2, 3, 4));
		REQUIRE(r != rect(2, 2, 3, 4));
		REQUIRE(r != rect(1, 3, 3, 4));
		REQUIRE(r != rect(1, 2, 4, 4));
		REQUIRE(r != rect(1, 2, 3, 5));
		
		r.x(5);
		r.y(6);
		r.w(7);
		r.h(8);
		
		REQUIRE(r.x() == 5);
		REQUIRE(r.y() == 6);
		REQUIRE(r.w() == 7);
		REQUIRE(r.h() == 8);
		
		REQUIRE(r == rect(5, 6, 7, 8));
		REQUIRE(r != rect(8, 7, 6, 5));
		
		r = rect(9, 10, 11, 12);

		REQUIRE(r.x() == 9);
		REQUIRE(r.y() == 10);
		REQUIRE(r.w() == 11);
		REQUIRE(r.h() == 12);
		
		REQUIRE(r == rect(9, 10, 11, 12));
		
		r = r;
		REQUIRE(r == rect(9, 10, 11, 12));
	}
	
	SECTION("second point get/set work as expected") {
		rect r(50, 100, 5, 10);
		
		REQUIRE(r.x2() == 55);
		REQUIRE(r.y2() == 110);
		
		r.x2(50 + 15);
		r.y2(100 + 30);
		
		REQUIRE(r.w() == 15);
		REQUIRE(r.h() == 30);
	}
	
	SECTION("from_center creates the correct rect") {
		REQUIRE(
			rect::from_center(100, 100, 5, 7)
				== rect(98, 97, 5, 7)
		);
		REQUIRE(
			rect::from_center(point(100, 100), point(5, 7))
				== rect(98, 97, 5, 7)
		);
	}
	
	SECTION("from_corners creates the correct rect") {
		REQUIRE(
			rect::from_corners(10, 20, 30, 40)
				== rect(10, 20, 20, 20)
		);
		REQUIRE(
			rect::from_corners(point(10, 20), point(30, 40))
				== rect(10, 20, 20, 20)
		);
	}
	
	SECTION("contains works correctly with points and rects") {
		rect r(10, 20, 5, 5);
		
		REQUIRE(r.contains(point(10, 20)));
		REQUIRE(r.contains(point(14, 24)));
		
		REQUIRE(!r.contains(point(9, 20)));
		REQUIRE(!r.contains(point(10, 19)));
		REQUIRE(!r.contains(point(16, 20)));
		REQUIRE(!r.contains(point(10, 26)));
		
		REQUIRE(r.contains(10, 20));
		REQUIRE(r.contains(14, 24));
		
		REQUIRE(!r.contains(9, 20));
		REQUIRE(!r.contains(10, 19));
		REQUIRE(!r.contains(16, 20));
		REQUIRE(!r.contains(10, 26));
		
		REQUIRE(r.contains(r));
		REQUIRE(r.contains(rect(11, 21, 3, 3)));
		
		REQUIRE(!r.contains(rect(9, 20, 5, 5)));
		REQUIRE(!r.contains(rect(10, 19, 5, 5)));
		REQUIRE(!r.contains(rect(10, 20, 6, 5)));
		REQUIRE(!r.contains(rect(10, 20, 5, 6)));
	}
	
	SECTION("intersections are correctly calculated") {
		rect r(10, 20, 30, 40);
		
		REQUIRE(r.intersects(r));
		REQUIRE(*r.get_intersection(r) == r);
		
		// Simple intersection
		REQUIRE(
			*r.get_intersection(rect(5, 15, 30, 40))
				== rect(10, 20, 25, 35)
		);
		REQUIRE(
			*r.get_intersection(rect(15, 25, 30, 40))
				== rect(15, 25, 25, 35)
		);
		
		// Larger at left
		REQUIRE(!r.get_intersection(rect(0, 0, 9, 80)));
		REQUIRE(!r.get_intersection(rect(0, 0, 10, 80)));
		REQUIRE(
			*r.get_intersection(rect(0, 0, 11, 80))
				== rect(10, 20, 1, 40)
		);
		
		// Larger at top
		REQUIRE(!r.get_intersection(rect(0, 0, 50, 19)));
		REQUIRE(!r.get_intersection(rect(0, 0, 50, 20)));
		REQUIRE(
			*r.get_intersection(rect(0, 0, 50, 21))
				== rect(10, 20, 30, 1)
		);
		
		// Larger at bottom
		REQUIRE(!r.get_intersection(rect(0, 60, 50, 19)));
		REQUIRE(!r.get_intersection(rect(0, 60, 50, 20)));
		REQUIRE(
			*r.get_intersection(rect(0, 59, 50, 21))
				== rect(10, 59, 30, 1)
		);
		
		// Larger at right
		REQUIRE(!r.get_intersection(rect(40, 0, 19, 80)));
		REQUIRE(!r.get_intersection(rect(40, 0, 20, 80)));
		REQUIRE(
			*r.get_intersection(rect(39, 0, 21, 80))
				== rect(39, 20, 1, 40)
		);
		
		// Smaller at top
		REQUIRE(!r.get_intersection(rect(20, 10, 10, 10)));
		REQUIRE(
			*r.get_intersection(rect(20, 10, 10, 20))
				== rect(20, 20, 10, 10)
		);
		
		// Smaller at bottom
		REQUIRE(!r.get_intersection(rect(20, 60, 10, 10)));
		REQUIRE(
			*r.get_intersection(rect(20, 50, 10, 20))
				== rect(20, 50, 10, 10)
		);
		
		// Smaller at right
		REQUIRE(!r.get_intersection(rect(40, 30, 10, 20)));
		REQUIRE(
			*r.get_intersection(rect(30, 30, 20, 20))
				== rect(30, 30, 10, 20)
		);
		
		// Smaller
		REQUIRE(
			*r.get_intersection(rect(20, 30, 10, 20))
				== rect(20, 30, 10, 20)
		);
		
		// Larger
		REQUIRE(
			*r.get_intersection(rect(0, 0, 100, 100))
				== r
		);
	}
	
	SECTION("unions are correctly calculated") {
		REQUIRE(
			rect(10, 20, 1, 1).get_u(rect(29, 39, 1, 1))
				== rect::from_corners(10, 20, 30, 40)
		);
		REQUIRE(
			rect(29, 20, 1, 1).get_u(rect(10, 39, 1, 1))
				== rect::from_corners(10, 20, 30, 40)
		);
		REQUIRE(
			rect(10, 39, 1, 1).get_u(rect(29, 20, 1, 1))
				== rect::from_corners(10, 20, 30, 40)
		);
		REQUIRE(
			rect(29, 39, 1, 1).get_u(rect(10, 20, 1, 1))
				== rect::from_corners(10, 20, 30, 40)
		);
	}
	
	SECTION("rect offset works") {
		rect r(1, 2, 3, 4);
		
		REQUIRE(r + point(10, 20) == rect(11, 22, 3, 4));
		REQUIRE(r - point(10, 20) == rect(-9, -18, 3, 4));
		
		r += point(10, 20);
		
		REQUIRE(r == rect(11, 22, 3, 4));
		
		r -= point(20, 40);
		
		REQUIRE(r == rect(-9, -18, 3, 4));
	}
	
}

