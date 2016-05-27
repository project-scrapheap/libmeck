
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

#include <meck/detail/test.hpp>

namespace test {

ui_block_controller::ui_block_controller(
	meck::application& app,
	const meck::controller_ptr& ctrlr
)
	: section_controller(app, ctrlr)
	, body_block_(ui_overlay_)
	, header_block_(ui_overlay_)
	, menu_block_(ui_overlay_)
	, header_0_block_(ui_overlay_)
	, header_1_block_(ui_overlay_)
	, header_2_block_(ui_overlay_)
	, menu_0_block_(ui_overlay_)
	, menu_1_block_(ui_overlay_)
{
	ui_overlay_.set_root(body_block_);
	
	body_block_.add(header_block_);
	header_block_.add(header_0_block_);
	header_block_.add(header_1_block_);
	header_block_.add(header_2_block_);
	
	body_block_.add(menu_block_);
	menu_block_.add(menu_0_block_);
	menu_block_.add(menu_1_block_);
	
	body_block_.set_size(680, 500);
	body_block_.set_inner_color({
		255,
		255,
		255,
		255
	});
	body_block_.center();
	
	header_block_.expand_width();
	header_block_.set_height(100);
	header_block_.set_inner_color({
		127,
		127,
		127,
		255
	});
	
	for (auto header_item : header_block_) {
		header_item->set_width(1.0 / 3.0);
		header_item->set_border(10, 10);
		header_item->expand_height();
	}
	
	header_0_block_.set_inner_color({255, 0, 0, 255});
	header_1_block_.set_inner_color({0, 255, 0, 255});
	header_2_block_.set_inner_color({0, 0, 255, 255});
	
	menu_block_.expand_width();
	menu_block_.set_height(400);
	menu_block_.set_border(10, 10);
	menu_block_.set_inner_color({
		255,
		255,
		255,
		255
	});
	
	for (auto menu_item : menu_block_) {
		menu_item->expand_width();
		menu_item->set_border(10, 10);
		menu_item->set_height(190);
	}
	
	menu_0_block_.set_inner_color({255, 255, 0, 255});
	menu_1_block_.set_inner_color({0, 255, 255, 255});
	
	ui_overlay_.finalize();
}

void
ui_block_controller::think() {
}

void
ui_block_controller::render() {
	ui_overlay_.render();
	meck::detail::test::compare_renderer_to_file(
		app_,
		"test_app-data/ui_block-0.bmp",
		"test_app-data/ui_block-0-screenshot.bmp"
	);
	next_controller();
}

} // namespace:test

