
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

ui_theme_controller::ui_theme_controller(
	meck::application& app,
	const meck::controller_ptr& ctrlr
)
	: section_controller(app, ctrlr)
	, body_block_(ui_overlay_)
	, widget_block_(ui_overlay_)
	, buttons_block_(ui_overlay_)
	, foo_label_(ui_overlay_)
	, foo_textbox_(ui_overlay_)
	, bar_label_(ui_overlay_)
	, bar_textbox_(ui_overlay_)
	, empty_label_(ui_overlay_)
	, empty_textbox_(ui_overlay_)
	, accept_checkbox_(ui_overlay_)
	, extra_checkbox_(ui_overlay_)
	, button_space_(ui_overlay_)
	, ok_button_(
		ui_overlay_,
		boost::bind(
			&ui_theme_controller::ok_action,
			this,
			_1
		)
	)
	, cancel_button_(
		ui_overlay_,
		boost::bind(
			&ui_theme_controller::cancel_action,
			this,
			_1
		)
	)
	, qmark_image_(
		ui_overlay_,
		"resource/img/question-mark.png"
	)
{
	ui_overlay_.set_root(body_block_);
	
	body_block_.add(widget_block_);
	body_block_.add(qmark_image_);
	
	widget_block_.add(foo_label_);
	widget_block_.add(foo_textbox_);
	widget_block_.add(bar_label_);
	widget_block_.add(bar_textbox_);
	widget_block_.add(empty_label_);
	widget_block_.add(empty_textbox_);
	widget_block_.add(accept_checkbox_);
	widget_block_.add(extra_checkbox_);
	widget_block_.add(button_space_);
	widget_block_.add(buttons_block_);
	
	buttons_block_.add(ok_button_);
	buttons_block_.add(cancel_button_);
	
	body_block_.set_size(680, 500);
	body_block_.center();
	
	widget_block_.set_border(20, 20);
	widget_block_.set_width(404);
	widget_block_.expand_height();
	
	foo_label_.set_value("Foo");
	foo_label_.set_for(foo_textbox_);
	foo_textbox_.set_value("bar");
	
	bar_label_.set_value("Bar");
	bar_label_.set_for(bar_textbox_);
	bar_textbox_.set_value("foo");
	
	empty_label_.set_value("Initially empty");
	empty_label_.set_for(empty_textbox_);
	empty_textbox_.set_value("");
	
	button_space_.expand_width();
	button_space_.set_height(20);
	button_space_.set_render_inner_rect(false);
	
	accept_checkbox_.set_label("I accept something");
	
	extra_checkbox_.set_label("Initially checked");
	extra_checkbox_.set_value("1");
	
	// This is a horz container, so the height is automatically
	// set to that of the biggest child if it is not otherwise
	// specified.
	buttons_block_.expand_width();
	
	ok_button_.set_width(172);
	ok_button_.set_after(10);
	ok_button_.set_value("Ok");
	
	cancel_button_.set_width(172);
	cancel_button_.set_before(10);
	cancel_button_.set_value("Cancel");
	
	// This could be more easily solved using a horz_container,
	// but solving it this way allows manual positioning to be
	// tested, which is nice.
	qmark_image_.set_positioning(meck::ui::block::positioning::MANUAL);
	qmark_image_.set_position(
		body_block_.get_inner_rect().w()
			- widget_block_.get_border().x()
			- qmark_image_.get_image_texture().get_size().x(),
		widget_block_.get_border().y()
	);
}

void
ui_theme_controller::think() {
}

void
ui_theme_controller::render() {
	ui_overlay_.render();
	
	meck::detail::test::compare_renderer_to_file(
		app_,
		"test_app-data/ui_theme-0.bmp",
		"test_app-data/ui_theme-0-screenshot.bmp"
	);
	next_controller();
}

} // namespace:test

