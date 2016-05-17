
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

#include "test_app.hpp"

#include <meck/screenshot.hpp>

namespace test {

void
renderer_controller::think() {
}

void
renderer_controller::render() {
	using meck::point;
	using meck::rect;
	
	meck::renderer_ptr rndr(app_.get_renderer());
	
	rndr->set_draw_color(255, 255, 255);
	
	rndr->draw_rect(10, 10, 20, 20);
	rndr->draw_rect(30, 10, 50, 30);
	
	rndr->fill_rect(10, 40, 20, 50);
	rndr->fill_rect(30, 40, 50, 60);
	
	for (int x_i = 60; x_i < 120; x_i += 2)
		rndr->draw_point(x_i, 10);
	
	for (int y_i = 10; y_i < 70; y_i += 2)
		rndr->draw_point(60, y_i);
	
	std::vector<rect> rs {
		rect(point(100, 100), point(40, 40)),
		rect(point(140, 140), point(80, 80)),
	};
	rndr->draw_rects(rs);
	
	rndr->copy(fox_text_, boost::none, point(70, 20));
	rndr->copy(qmark_image_, boost::none, point(514, 314));
	
	rndr->set_draw_color(255, 0, 0);
	rndr->fill_rect(100, 460, 200, 560);
	
	rndr->set_draw_color(0, 255, 0);
	rndr->fill_rect(210, 460, 310, 560);
	
	rndr->set_draw_color(0, 0, 255);
	rndr->fill_rect(320, 460, 420, 560);
	
	rndr->set_draw_color(255, 255, 255);
	
	rndr->draw_line(100, 570, 770, 570);
	rndr->draw_line(770, 60, 770, 570);
	
	rndr->draw_line(110, 580, 780, 580);
	rndr->draw_line(780, 70, 780, 580);
	
	rndr->set_draw_color(0, 0, 0);
	
	meck::screenshot_BMP("test_app-data/frame-0-screenshot.bmp", app_);
	
	meck::surface expected_srfc("test_app-data/frame-0.bmp");
	meck::surface screenshot_srfc("test_app-data/frame-0-screenshot.bmp");
	meck::surface actual_srfc(app_.get_window()->get_window_surface());
	
	const std::size_t n = actual_srfc.get_width() * actual_srfc.get_height()
			* actual_srfc.get_format().BytesPerPixel;
	
	RUNTIME_ASSERT(std::memcmp(
		expected_srfc.get()->pixels,
		actual_srfc.get()->pixels,
		n
	));
	
	RUNTIME_ASSERT(std::memcmp(
		screenshot_srfc.get()->pixels,
		actual_srfc.get()->pixels,
		n
	));
	
	next_controller();
}

} // namespace:test

