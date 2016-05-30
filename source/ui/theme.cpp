
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
#include <meck/ui/overlay.hpp>
#include <meck/ui/theme.hpp>

namespace meck {
namespace ui {

theme::theme(
	overlay& olay
)
	: owner(olay)
	, normal_font_pts(16)
	, normal_font(nullptr)
	, bold_font_pts(16)
	, bold_font(nullptr)
	, italic_font_pts(16)
	, italic_font(nullptr)
	, button_border(5, 5)
	, button_shadow_border(1, 1)
	, checkbox_border(0, 5)
	, label_border(0, 5)
	, textbox_border(5, 5)
	, textbox_shadow_border(1, 1)
	, button_before(0)
	, button_after(5)
	, textbox_before(0)
	, textbox_after(5)
	, text_fg {85, 85, 85, 255}
	, bg {240, 240, 240, 255}
	, container_bg {240, 240, 240, 255}
	, button_bg {200, 200, 200, 255}
	, button_focus_bg {222, 222, 222, 255}
	, button_shadow_bg {128, 128, 128, 255}
	, button_shadow_focus_bg {240, 130, 100, 255}
	, textbox_bg {234, 234, 234, 255}
	, textbox_focus_bg {250, 250, 250, 255}
	, textbox_shadow_bg {195, 195, 195, 255}
	, textbox_shadow_focus_bg {240, 130, 100, 255}
{}

void
theme::init() {
	const auto& opt_vm = owner.get_application().get_opt_vm();
	const auto& base_path = opt_vm["base-path"].as<std::string>();
	
	normal_font = font(
		base_path + "/resource/font/default/normal.ttf",
		normal_font_pts
	);
	
	bold_font = font(
		base_path + "/resource/font/default/bold.ttf",
		normal_font_pts
	);
	
	italic_font = font(
		base_path + "/resource/font/default/italic.ttf",
		normal_font_pts
	);
	
}

} // namespace:ui
} // namespace:meck

