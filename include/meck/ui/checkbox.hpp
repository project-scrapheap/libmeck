
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

#ifndef MECK_UI_CHECKBOX_HPP
#define MECK_UI_CHECKBOX_HPP

#include <meck/ui/text.hpp>

#include <SDL.h>

namespace meck {
namespace ui {

class checkbox
	: public text
{
public:
	explicit
	checkbox(
		overlay& olay
	)
		: text(olay)
		, checked_(false)
	{
		render_outer_rect_ = false;
		render_inner_rect_ = false;
		
		set_border(owner_.get_theme().checkbox_border);
	}
	
	virtual void
	finalize() {
		text::finalize();
		
		box_rect_.x(inner_rect_.x());
		box_rect_.y(inner_rect_.y());
		check_rect_.x(inner_rect_.x() + 2);
		check_rect_.y(inner_rect_.y() + 2);
		shadow_rect_.x(inner_rect_.x() - 1);
		shadow_rect_.y(inner_rect_.y() - 1);
		
		const int side = owner_.get_theme().normal_font.get_height();
		const int space = side + side / 2;
		
		set_position(outer_rect_.x() + space, outer_rect_.y());
		set_width(outer_rect_.w() - space);
		
		box_rect_.w(side);
		box_rect_.h(side);
		check_rect_.w(side - 4);
		check_rect_.h(side - 4);
		shadow_rect_.w(side + 2);
		shadow_rect_.h(side + 2);
	}
	
	virtual bool
	react(
		::SDL_Event& event
	);
	
	virtual void
	think();
	
	virtual void
	render();
	
	virtual std::string
	get_value() {
		return checked_str_;
	}
	
	virtual const std::string&
	get_value() const {
		return checked_str_;
	}
	
	virtual void
	set_value(
		const std::string& txt
	) {
		if (!(txt.size() == 1 && (txt[0] == '1' || txt[0] == '0')))
			RUNTIME_ERROR("invalid value %1%, should be \"1\" or \"0\"", txt);
		checked_ = txt[0] == '1';
		checked_str_ = txt;
		rerender_ = true;
	}
	
	virtual std::string
	get_label() {
		return value_;
	}
	
	virtual const std::string&
	get_label() const {
		return value_;
	}
	
	virtual void
	set_label(
		const std::string& label
	) {
		value_ = label;
		rerender_ = true;
	}
	
protected:
	rect box_rect_;
	rect check_rect_;
	rect shadow_rect_;
	std::string checked_str_;
	bool checked_;
};


} // namespace:ui
} // namespace:meck

#endif

