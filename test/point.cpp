
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
	"test_point",
	"[meck::point]"
) {
	using meck::point;
	
	meck::scoped_sdl sdl(MECK_SDL_DEFAULT_INIT);
	
	SECTION("basic constructors and methods work") {
		point pt(1, 2);
		
		REQUIRE(pt.x() == 1);
		REQUIRE(pt.y() == 2);
		REQUIRE(pt == point(1, 2));
		REQUIRE(pt != point(1, 1));
		REQUIRE(pt != point(2, 1));
		REQUIRE(pt != point(2, 2));
		
		pt.x(3);
		pt.y(4);
		
		REQUIRE(pt.x() == 3);
		REQUIRE(pt.y() == 4);
		
		pt = point(5, 6);
		
		REQUIRE(pt.x() == 5);
		REQUIRE(pt.y() == 6);
		
		pt = pt;
		
		REQUIRE(pt.x() == 5);
		REQUIRE(pt.y() == 6);
	}
	
	SECTION("point arithmetic works") {
		REQUIRE(point(1, 2) + point(10, 20) == point(11, 22));
		REQUIRE(point(-1, -2) - point(10, 20) == point(-11, -22));
		REQUIRE(point(20, 60) / 5 == point(4, 12));
		REQUIRE(point(20, 60) / point(5, 10) == point(4, 6));
		REQUIRE(point(20, 60) % 11 == point(9, 5));
		REQUIRE(point(20, 60) % point(11, 13) == point(9, 8));
		REQUIRE(point(2, 3) * 5 == point(10, 15));
		REQUIRE(point(2, 3) * point(10, 20) == point(20, 60));
		
		point pt(1, 2);
		
		pt += point(10, 20);
		REQUIRE(pt == point(11, 22));
		
		pt -= point(1, 2);
		REQUIRE(pt == point(10, 20));
		
		pt /= 2;
		REQUIRE(pt == point(5, 10));
		
		pt %= 7;
		REQUIRE(pt == point(5, 3));
		
		pt *= 3;
		REQUIRE(pt == point(15, 9));
		
		pt /= point(5, 3);
		REQUIRE(pt == point(3, 3));
		
		pt *= point(10, 20);
		REQUIRE(pt == point(30, 60));
		
		pt %= point(7, 11);
		REQUIRE(pt == point(2, 5));
		
		REQUIRE(point(0, 0) < point(1, 0));
		REQUIRE(point(0, 1) < point(1, 0));
		REQUIRE(point(0, 1) < point(1, 1));
		REQUIRE(point(0, 0) < point(0, 1));
		
		REQUIRE(!(point(1, 0) < point(0, 0)));
		REQUIRE(!(point(1, 0) < point(0, 1)));
		REQUIRE(!(point(1, 1) < point(0, 1)));
		REQUIRE(!(point(0, 1) < point(0, 0)));
		REQUIRE(!(point(1, 1) < point(1, 1)));
	}
	
}

