
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

#ifndef MECK_WINDOW_HPP
#define MECK_WINDOW_HPP

#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include <meck/error.hpp>
#include <meck/point.hpp>
#include <meck/rect.hpp>
#include <meck/surface.hpp>

#include <SDL.h>

namespace meck {

class window
	: private boost::noncopyable
{
public:
	explicit
	window(
		::SDL_Window* win
	) noexcept:
		window_(win)
	{}
	
	window(
		const std::string& title,
		const point& pos,
		const point& size,
		const ::Uint32 flags
	) :
		window_(::SDL_CreateWindow(
			title.c_str(),
			pos.x(),
			pos.y(),
			size.x(),
			size.y(),
			flags
		))
	{
		if (!window_)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	window(
		const std::string& title,
		const int x,
		const int y,
		const int w,
		const int h,
		const ::Uint32 flags
	) :
		window(
			title,
			point(x, y),
			point(w, h),
			flags
		)
	{}
	
	virtual
	~window() noexcept {
		::SDL_DestroyWindow(window_);
	}
	
	inline
	window(
		window&& rhs
	) noexcept:
		window_(rhs.window_)
	{
		rhs.window_ = nullptr;
	}

	window&
	operator=(
		window&& rhs
	) noexcept {
		if (this != &rhs) {
			if (window_)
				::SDL_DestroyWindow(window_);
			window_ = rhs.window_;
			rhs.window_ = nullptr;
		}
		return *this;
	}
	
	::SDL_Window*
	get() noexcept {
		return window_;
	}
	
	const ::SDL_Window*
	get() const noexcept {
		return window_;
	}
	
	point
	get_size() const {
		RUNTIME_ASSERT(window_);
		int w;
		int h;
		::SDL_GetWindowSize(window_, &w, &h);
		return point(w, h);
	}
	
	int
	get_width() const {
		return get_size().x();
	}

	int
	get_height() const {
		return get_size().y();
	}
	
	point
	get_drawable_size() const {
		RUNTIME_ASSERT(window_);
		int w;
		int h;
		::SDL_GL_GetDrawableSize(window_, &w, &h);
		return point(w, h);
	}
	
	int
	get_drawable_width() const {
		return get_drawable_size().x();
	}

	int
	get_drawable_height() const {
		return get_drawable_size().y();
	}
	
	window&
	set_title(
		const std::string& title
	) {
		RUNTIME_ASSERT(window_);
		::SDL_SetWindowTitle(window_, title.c_str());
		return *this;
	}
	
	std::string
	get_title() const {
		RUNTIME_ASSERT(window_);
		return ::SDL_GetWindowTitle(window_);
	}
	
	window&
	maximize() {
		RUNTIME_ASSERT(window_);
		::SDL_MaximizeWindow(window_);
		return *this;
	}
	
	window&
	minimize() {
		RUNTIME_ASSERT(window_);
		::SDL_MinimizeWindow(window_);
		return *this;
	}
	
	window&
	hide() {
		RUNTIME_ASSERT(window_);
		::SDL_HideWindow(window_);
		return *this;
	}
	
	window&
	restore() {
		RUNTIME_ASSERT(window_);
		::SDL_RestoreWindow(window_);
		return *this;
	}
	
	window&
	raise() {
		RUNTIME_ASSERT(window_);
		::SDL_RaiseWindow(window_);
		return *this;
	}
	
	window&
	show() {
		RUNTIME_ASSERT(window_);
		::SDL_ShowWindow(window_);
		return *this;
	}
	
	window&
	set_fullscreen(
		const ::Uint32 flags
	) {
		RUNTIME_ASSERT(window_);
		if (::SDL_SetWindowFullscreen(window_, flags))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	window&
	set_size(
		const int w,
		const int h
	) {
		return set_size(point(w, h));
	}
	
	window&
	set_size(
		const point& size
	) {
		RUNTIME_ASSERT(window_);
		::SDL_SetWindowSize(window_, size.x(), size.y());
		return *this;
	}
	
	float
	get_brightness() const {
		RUNTIME_ASSERT(window_);
		return ::SDL_GetWindowBrightness(window_);
	}
	
	window&
	set_brightness(
		const float brightness
	) {
		RUNTIME_ASSERT(window_);
		if (::SDL_SetWindowBrightness(window_, brightness))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	point
	get_position() const {
		RUNTIME_ASSERT(window_);
		int x;
		int y;
		::SDL_GetWindowPosition(window_, &x, &y);
		return point(x, y);
	}
	
	window&
	set_position(
		const int x,
		const int y
	) {
		return set_position(point(x, y));
	}
	
	window&
	set_position(
		const point& pos
	) {
		RUNTIME_ASSERT(window_);
		::SDL_SetWindowPosition(window_, pos.x(), pos.y());
		return *this;
	}
	
	point
	get_minimum_size() const {
		RUNTIME_ASSERT(window_);
		int w;
		int h;
		::SDL_GetWindowMinimumSize(window_, &w, &h);
		return point(w, h);
	}
	
	window&
	set_minimum_size(
		const int w,
		const int h
	) {
		return set_minimum_size(point(w, h));
	}
	
	window&
	set_minimum_size(
		const point& size
	) {
		RUNTIME_ASSERT(window_);
		::SDL_SetWindowMinimumSize(window_, size.x(), size.y());
		return *this;
	}
	
	point
	get_maximum_size() const {
		RUNTIME_ASSERT(window_);
		int w;
		int h;
		::SDL_GetWindowMaximumSize(window_, &w, &h);
		return point(w, h);
	}
	
	window&
	set_maximum_size(
		int w,
		int h
	) {
		return set_maximum_size(point(w, h));
	}
	
	window&
	set_maximum_size(
		const point& size
	) {
		RUNTIME_ASSERT(window_);
		::SDL_SetWindowMaximumSize(window_, size.x(), size.y());
		return *this;
	}
	
	bool
	get_grab() const {
		RUNTIME_ASSERT(window_);
		return ::SDL_GetWindowGrab(window_) == SDL_TRUE;
	}
	
	window&
	set_grab(
		const bool grabbed
	) {
		RUNTIME_ASSERT(window_);
		::SDL_SetWindowGrab(window_, static_cast<::SDL_bool>(grabbed));
		return *this;
	}
	
	int
	get_display_index() const {
		RUNTIME_ASSERT(window_);
		int index = ::SDL_GetWindowDisplayIndex(window_);
		if (index < 0)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return index;
	}
	
	void
	get_display_mode(
		::SDL_DisplayMode& mode
	) const {
		RUNTIME_ASSERT(window_);
		if (::SDL_GetWindowDisplayMode(window_, &mode))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	::Uint32
	get_flags() const {
		RUNTIME_ASSERT(window_);
		return ::SDL_GetWindowFlags(window_);
	}
	
	window&
	set_icon(
		surface& icon
	) {
		RUNTIME_ASSERT(window_);
		::SDL_SetWindowIcon(window_, icon.get());
		return *this;
	}
	
	window&
	set_bordered(
		const bool bordered = true
	) {
		RUNTIME_ASSERT(window_);
		::SDL_SetWindowBordered(window_, static_cast<::SDL_bool>(bordered));
		return *this;
	}
	
	surface
	get_window_surface() {
		RUNTIME_ASSERT(window_);
		return surface(::SDL_GetWindowSurface(window_));
	}
	
protected:
	::SDL_Window* window_;
};

typedef std::shared_ptr<window> window_ptr;

} // namespace:meck

#endif

