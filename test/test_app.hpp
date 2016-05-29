
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

#ifndef TEST_HPP
#define TEST_HPP

#include <queue>

#include <meck.hpp>

namespace test {

class section_controller;
class queue_controller;

class section_controller
	: public meck::controller
{
public:
	explicit
	section_controller(
		meck::application& app,
		const meck::controller_ptr& ctrlr
	)
		: meck::controller(app)
		, queue_controller_(ctrlr)
	{}
	
	virtual void
	next_controller() {
		app_.queue_controller(queue_controller_);
	}
	
	virtual void
	render() {
	}
	
protected:
	meck::controller_ptr queue_controller_;
};

/*
class configure_controller
	: public section_controller
{
public:
	explicit
	configure_controller(
		meck::application& app,
		const meck::controller_ptr& ctrlr
	)
		: section_controller(app, ctrlr)
	{}
	
	virtual void
	think();
	
};
*/

class font_controller
	: public section_controller
{
public:
	explicit
	font_controller(
		meck::application& app,
		const meck::controller_ptr& ctrlr
	)
		: section_controller(app, ctrlr)
		, font_("../resource/font/ubuntu-family/UbuntuMono-R.ttf", 32)
	{}
	
	virtual void
	think();
	
protected:
	meck::font font_;
};

class keycode_controller
	: public section_controller
{
public:
	explicit
	keycode_controller(
		meck::application& app,
		const meck::controller_ptr& ctrlr
	)
		: section_controller(app, ctrlr)
		, frame_(0)
		, frame_limit_(8)
		, menu_keydown_(false)
		, menu_keyup_(false)
	{
		KEYCODE_DISPATCH(keycode_controller, menu, "game_control");
	}
	
	bool
	key_menu(
		::SDL_Event& event
	);
	
	virtual void
	think();
	
protected:
	::Uint32 frame_;
	::Uint32 frame_limit_;
	bool menu_keydown_;
	bool menu_keyup_;
};

class renderer_controller
	: public section_controller
{
public:
	explicit
	renderer_controller(
		meck::application& app,
		const meck::controller_ptr& ctrlr
	)
		: section_controller(app, ctrlr)
		, frame_(0)
		, font_("resource/font/ubuntu-family/UbuntuMono-R.ttf", 32)
		, fox_text_(nullptr)
		, qmark_image_(nullptr)
	{
		fox_text_ = font_.render_text_solid(
			app_.get_renderer(),
			"The quick brown fox jumps over the lazy dog",
			{255, 255, 255, 255}
		);
		
		qmark_image_ = meck::texture(
			app_.get_renderer(),
			"resource/img/question-mark.png"
		);
	}
	
	virtual void
	think();
	
	virtual void
	render();
	
protected:
	::Uint32 frame_;
	meck::font font_;
	meck::texture fox_text_;
	meck::texture qmark_image_;
};

class ui_block_controller
	: public section_controller
{
public:
	explicit
	ui_block_controller(
		meck::application& app,
		const meck::controller_ptr& ctrlr
	);
	
	virtual void
	think();
	
	virtual void
	render();
	
protected:
	meck::ui::vert_container body_block_;
	meck::ui::horz_container header_block_;
	meck::ui::vert_container menu_block_;
	meck::ui::block header_0_block_;
	meck::ui::block header_1_block_;
	meck::ui::block header_2_block_;
	meck::ui::block menu_0_block_;
	meck::ui::block menu_1_block_;
};

class ui_theme_controller
	: public section_controller
{
public:
	explicit
	ui_theme_controller(
		meck::application& app,
		const meck::controller_ptr& ctrlr
	);
	
	virtual void
	think();
	
	virtual void
	render();
	
protected:
	meck::ui::vert_container body_block_;
	meck::ui::vert_container widget_block_;
	meck::ui::label foo_label_;
	meck::ui::ascii_textbox foo_textbox_;
	meck::ui::label bar_label_;
	meck::ui::ascii_textbox bar_textbox_;
	meck::ui::label empty_label_;
	meck::ui::ascii_textbox empty_textbox_;
	
	meck::ui::image qmark_image_;
};

class queue_controller
	: public meck::controller
{
public:
	explicit
	queue_controller(
		meck::application& app
	) :
		meck::controller(app)
	{}
	
	virtual void
	init() {
		meck::controller::init();
		
		if (queue_.size()) {
			queue_.pop();
		} else {
			meck::controller_ptr self(app_.get_current_controller());
			
#define ADD_CONTROLLER(Prefix) \
	queue_.push(meck::controller_ptr( \
		new Prefix##_controller(app_, self) \
	)) \
	/**/
			//ADD_CONTROLLER(configure);
			ADD_CONTROLLER(font);
			ADD_CONTROLLER(keycode);
			ADD_CONTROLLER(renderer);
			ADD_CONTROLLER(ui_block);
			ADD_CONTROLLER(ui_theme);
			
#undef ADD_CONTROLLER
		}
	}
	
	virtual void
	think() {
		if (queue_.size()) {
			app_.queue_controller(queue_.front());
		} else {
			app_.stop();
		}
	}
	
	virtual void
	render() {
	}
	
protected:
	meck::controller_queue queue_;
};

} // namespace:test

#endif

