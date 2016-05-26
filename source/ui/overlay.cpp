
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
#include <meck/controller.hpp>
#include <meck/ui/container.hpp>
#include <meck/ui/overlay.hpp>
#include <meck/ui/theme.hpp>

namespace meck {
namespace ui {

overlay::overlay(
	controller& ctrlr
)
	: theme_(new theme(ctrlr))
	, controller_(ctrlr)
	, root_(nullptr)
	, focused_(nullptr)
{}

overlay::~overlay() noexcept {
}

point
overlay::get_size() const {
	return controller_
		.get_application()
		.get_renderer()
		.get_logical_size();
}

rect
overlay::get_outmost_rect() const {
	return rect(point(0, 0), get_size());
}

void
overlay::set_root() {
	root_ = nullptr;
}

void
overlay::set_root(
	container& blk
) {
	blk.set_size(get_size());
	blk.set_position(point(0, 0));
	root_ = &blk;
}

void
overlay::set_focused() {
	focused_ = nullptr;
}

void
overlay::set_focused(
	widget& wdg
) {
	focused_ = &wdg;
}


bool
overlay::is_focused(
	const widget& wdg
) {
	return focused_ == &wdg;
}

bool
overlay::react(
	::SDL_Event& event
) {
	return root_? root_->react(event): false;
}

void
overlay::think() {
	if (root_) root_->think();
}

void
overlay::render() {
	if (root_) root_->render();
}

void
overlay::setup() {
	RUNTIME_ASSERT(root_);
	root_->setup();
}

controller&
overlay::get_controller() {
	return controller_;
}

application&
overlay::get_application() {
	return controller_.get_application();
}

theme&
overlay::get_theme() {
	RUNTIME_ASSERT(theme_.get());
	return *theme_;
}

} // namespace:ui
} // namespace:meck

