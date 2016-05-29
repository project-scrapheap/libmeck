
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

#include <meck/application.hpp>
#include <meck/ui/ascii_textbox.hpp>

namespace meck {
namespace ui {

#define KEY_HACK_SPEC(Key, Char) \
	case SDLK_##Key: \
		value_.append(Char); \
		rerender_ = true; \
		return true; \
		break
#define KEY_HACK(Key) \
	KEY_HACK_SPEC(Key, #Key)

bool
ascii_textbox::react(
	::SDL_Event& event
) {
	if (test_clicked(outer_rect_, event, SDL_BUTTON_LEFT)) {
		owner_.set_focused(*this);
		return true;
	} else if (event.type == SDL_KEYDOWN && owner_.is_focused(*this)) {
		
		switch (event.key.keysym.sym) {
			KEY_HACK(a);
			KEY_HACK(b);
			KEY_HACK(c);
			KEY_HACK(d);
			KEY_HACK(e);
			KEY_HACK(f);
			KEY_HACK(g);
			KEY_HACK(h);
			KEY_HACK(i);
			KEY_HACK(j);
			KEY_HACK(k);
			KEY_HACK(l);
			KEY_HACK(m);
			KEY_HACK(n);
			KEY_HACK(o);
			KEY_HACK(p);
			KEY_HACK(q);
			KEY_HACK(r);
			KEY_HACK(s);
			KEY_HACK(t);
			KEY_HACK(u);
			KEY_HACK(v);
			KEY_HACK(w);
			KEY_HACK(x);
			KEY_HACK(y);
			KEY_HACK(z);

			KEY_HACK(0);
			KEY_HACK(1);
			KEY_HACK(2);
			KEY_HACK(3);
			KEY_HACK(4);
			KEY_HACK(5);
			KEY_HACK(6);
			KEY_HACK(7);
			KEY_HACK(8);
			KEY_HACK(9);

			KEY_HACK_SPEC(SPACE, " ");
			KEY_HACK_SPEC(PERIOD, ".");
			KEY_HACK_SPEC(EXCLAIM, "!");
			KEY_HACK_SPEC(QUOTE, "'");
			KEY_HACK_SPEC(QUOTEDBL, "\"");
			KEY_HACK_SPEC(PERCENT, "%");
			KEY_HACK_SPEC(DOLLAR, "$");
			KEY_HACK_SPEC(AMPERSAND, "&");
			KEY_HACK_SPEC(LEFTPAREN, "(");
			KEY_HACK_SPEC(RIGHTPAREN, ")");
			KEY_HACK_SPEC(ASTERISK, "*");
			KEY_HACK_SPEC(PLUS, "+");
			KEY_HACK_SPEC(MINUS, "-");
			KEY_HACK_SPEC(COMMA, ",");
			KEY_HACK_SPEC(SLASH, "/");
			KEY_HACK_SPEC(COLON, ":");
			KEY_HACK_SPEC(SEMICOLON, ";");
			KEY_HACK_SPEC(LESS, "<");
			KEY_HACK_SPEC(EQUALS, "=");
			KEY_HACK_SPEC(GREATER, ">");
			KEY_HACK_SPEC(QUESTION, "?");
			KEY_HACK_SPEC(AT, "@");
			
			KEY_HACK_SPEC(KP_DIVIDE, "/");
			KEY_HACK_SPEC(KP_MULTIPLY, "*");
			KEY_HACK_SPEC(KP_MINUS, "-");
			KEY_HACK_SPEC(KP_PLUS, "+");
			KEY_HACK_SPEC(KP_0, "0");
			KEY_HACK_SPEC(KP_1, "1");
			KEY_HACK_SPEC(KP_2, "2");
			KEY_HACK_SPEC(KP_3, "3");
			KEY_HACK_SPEC(KP_4, "4");
			KEY_HACK_SPEC(KP_5, "5");
			KEY_HACK_SPEC(KP_6, "6");
			KEY_HACK_SPEC(KP_7, "7");
			KEY_HACK_SPEC(KP_8, "8");
			KEY_HACK_SPEC(KP_9, "9");
			KEY_HACK_SPEC(KP_PERIOD, ".");
			KEY_HACK_SPEC(KP_EQUALS, "=");
			
			case SDLK_BACKSPACE:
				if (value_.size()) {
					value_.pop_back();
					rerender_ = true;
				}
				return true;
			break;
			
			default:
				value_.append("?");
				rerender_ = true;
				return true;
			break;
		}
	}
	return false;
}

#undef KEY_HACK
#undef KEY_HACK_SPEC

void
ascii_textbox::think() {
	text::think();
	
	const bool focused = owner_.is_focused(*this);
	
	outer_color_ = focused?
		owner_.get_theme().textbox_focus_bg:
		owner_.get_theme().textbox_bg;
	
	append_ = focused && static_cast<int>(
		owner_.get_application().get_timer().total_sec()
	) % 2 == 0? '_': ' ';

}

void
ascii_textbox::render() {
	text::render();
}


} // namespace:ui
} // namespace:meck

