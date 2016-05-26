
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
#include <meck/keycode.hpp>

namespace meck {

keycode_dispatcher::keycode_dispatcher(
	application& app
)
	: app_(app)
{
}

keycode_dispatcher::~keycode_dispatcher() noexcept {
}

void
keycode_dispatcher::add_bindable(
	const keycode_function& func,
	const std::string& name,
	const std::string& section
) {
	std::string i = format("%1%.%2%", section, name);
	std::string key = app_.get_ptree().get<std::string>(i);
	auto pair = global::keycodes.find(key);
	if (pair == global::keycodes.end()) {
		RUNTIME_ERROR("binding %1%.%2% specifies invalid key %3%",
			section, name, key);
	}
	dispatch_map_.insert(dispatch_map::value_type(pair->second, func));
}

bool
keycode_dispatcher::handle(
	::SDL_Event& event
) {
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		auto pair = dispatch_map_.find(event.key.keysym.sym);
		if (pair != dispatch_map_.end())
			return pair->second(event);
	}
	return false;
}

namespace global {

keycode_map keycodes {
	{ "a", ::SDLK_a },
	{ "b", ::SDLK_b },
	{ "c", ::SDLK_c },
	{ "d", ::SDLK_d },
	{ "e", ::SDLK_e },
	{ "f", ::SDLK_f },
	{ "g", ::SDLK_g },
	{ "h", ::SDLK_h },
	{ "i", ::SDLK_i },
	{ "j", ::SDLK_j },
	{ "k", ::SDLK_k },
	{ "l", ::SDLK_l },
	{ "m", ::SDLK_m },
	{ "n", ::SDLK_n },
	{ "o", ::SDLK_o },
	{ "p", ::SDLK_p },
	{ "q", ::SDLK_q },
	{ "r", ::SDLK_r },
	{ "s", ::SDLK_s },
	{ "t", ::SDLK_t },
	{ "u", ::SDLK_u },
	{ "v", ::SDLK_v },
	{ "w", ::SDLK_w },
	{ "x", ::SDLK_x },
	{ "y", ::SDLK_y },
	{ "z", ::SDLK_z },

	{ "0", ::SDLK_0 },
	{ "1", ::SDLK_1 },
	{ "2", ::SDLK_2 },
	{ "3", ::SDLK_3 },
	{ "4", ::SDLK_4 },
	{ "5", ::SDLK_5 },
	{ "6", ::SDLK_6 },
	{ "7", ::SDLK_7 },
	{ "8", ::SDLK_8 },
	{ "9", ::SDLK_9 },

	{ "F1", ::SDLK_F1 },
	{ "F2", ::SDLK_F2 },
	{ "F3", ::SDLK_F3 },
	{ "F4", ::SDLK_F4 },
	{ "F5", ::SDLK_F5 },
	{ "F6", ::SDLK_F6 },
	{ "F7", ::SDLK_F7 },
	{ "F8", ::SDLK_F8 },
	{ "F9", ::SDLK_F9 },
	{ "F10", ::SDLK_F10 },
	{ "F11", ::SDLK_F11 },
	{ "F12", ::SDLK_F12 },

	{ "space", ::SDLK_SPACE },
	{ "return", ::SDLK_RETURN },
	{ "escape", ::SDLK_ESCAPE },
	{ "exclaim", ::SDLK_EXCLAIM },
	{ "backspace", ::SDLK_BACKSPACE },
	{ "tab", ::SDLK_TAB },
	{ "quote", ::SDLK_QUOTE },
	{ "quotedbl", ::SDLK_QUOTEDBL },
	{ "hash", ::SDLK_HASH },
	{ "percent", ::SDLK_PERCENT },
	{ "dollar", ::SDLK_DOLLAR },
	{ "ampersand", ::SDLK_AMPERSAND },
	{ "leftparen", ::SDLK_LEFTPAREN },
	{ "rightparen", ::SDLK_RIGHTPAREN },
	{ "asterisk", ::SDLK_ASTERISK },
	{ "plus", ::SDLK_PLUS },
	{ "minus", ::SDLK_MINUS },
	{ "period", ::SDLK_PERIOD },
	{ "comma", ::SDLK_COMMA },
	{ "slash", ::SDLK_SLASH },
	{ "colon", ::SDLK_COLON },
	{ "semicolon", ::SDLK_SEMICOLON },
	{ "less", ::SDLK_LESS },
	{ "equals", ::SDLK_EQUALS },
	{ "greater", ::SDLK_GREATER },
	{ "question", ::SDLK_QUESTION },
	{ "at", ::SDLK_AT },
	{ "capslock", ::SDLK_CAPSLOCK },

	{ "printscreen", ::SDLK_PRINTSCREEN },
	{ "scrolllock", ::SDLK_SCROLLLOCK },
	{ "pause", ::SDLK_PAUSE },
	{ "insert", ::SDLK_INSERT },
	{ "home", ::SDLK_HOME },
	{ "pageup", ::SDLK_PAGEUP },
	{ "pagedown", ::SDLK_PAGEDOWN },
	{ "delete", ::SDLK_DELETE },
	{ "end", ::SDLK_END },

	{ "right", ::SDLK_RIGHT },
	{ "left", ::SDLK_LEFT },
	{ "down", ::SDLK_DOWN },
	{ "up", ::SDLK_UP },

	{ "numlck_clear", ::SDLK_NUMLOCKCLEAR },
	{ "kp_divide", ::SDLK_KP_DIVIDE },
	{ "kp_multiply", ::SDLK_KP_MULTIPLY },
	{ "kp_minus", ::SDLK_KP_MINUS },
	{ "kp_plus", ::SDLK_KP_PLUS },
	{ "kp_enter", ::SDLK_KP_ENTER },
	{ "kp_0", ::SDLK_KP_0 },
	{ "kp_1", ::SDLK_KP_1 },
	{ "kp_2", ::SDLK_KP_2 },
	{ "kp_3", ::SDLK_KP_3 },
	{ "kp_4", ::SDLK_KP_4 },
	{ "kp_5", ::SDLK_KP_5 },
	{ "kp_6", ::SDLK_KP_6 },
	{ "kp_7", ::SDLK_KP_7 },
	{ "kp_8", ::SDLK_KP_7 },
	{ "kp_9", ::SDLK_KP_9 },
	{ "kp_period", ::SDLK_KP_PERIOD },
	{ "kp_equals", ::SDLK_KP_EQUALS },

	{ "application", ::SDLK_APPLICATION },
	{ "power", ::SDLK_POWER },

	{ "lctrl", ::SDLK_LCTRL },
	{ "lshift", ::SDLK_LSHIFT },
	{ "lalt", ::SDLK_LALT },
	{ "lgui", ::SDLK_LGUI },
	{ "rctrl", ::SDLK_RCTRL },
	{ "rshift", ::SDLK_RSHIFT },
	{ "ralt", ::SDLK_RALT },
	{ "rgui", ::SDLK_RGUI }

};

} // namespace:global

} // namespace:meck

