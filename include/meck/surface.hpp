
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

#ifndef MECK_surface_HPP
#define MECK_surface_HPP

#include <mutex>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include <meck/error.hpp>
#include <meck/hack.hpp>
#include <meck/point.hpp>
#include <meck/rect.hpp>

#include <SDL.h>
#include <SDL_image.h>

namespace meck {

class surface
	: private boost::noncopyable
{
public:
	class lock_guard :
		private boost::noncopyable
	{
	public:
		typedef surface mutex_type;
		
		inline explicit
		lock_guard(
			mutex_type& m
		)
			: m_(m)
		{
			m_.lock();
		}
		
		inline
    	~lock_guard() {
    		m_.unlock();
    	}
    	
    	inline void*
    	get_pixels() {
    		return m_.get()->pixels;
    	}
    	
    	inline int
    	get_pitch() {
    		return m_.get()->pitch;
    	}
    	
    	inline ::SDL_PixelFormat&
    	get_format() {
    		return *m_.get()->format;
    	}
    	
	private:
		mutex_type& m_;
	};
	
	typedef lock_guard lock_guard_type;
	
	explicit
	surface(
		::SDL_Surface* srfce
	) noexcept
		: surface_(srfce)
	{}
	
	explicit
	surface(
		const boost::filesystem::path& filename
	)
		: surface_(::IMG_Load(filename.string().c_str()))
	{
		if (!surface_)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	inline
	surface(
		::Uint32 flags,
		int width,
		int height,
		int depth,
		::Uint32 Rmask,
		::Uint32 Gmask,
		::Uint32 Bmask,
		::Uint32 Amask
	)
		: surface_(::SDL_CreateRGBSurface(
				flags,
				width,
				height,
				depth,
				Rmask,
				Gmask,
				Bmask,
				Amask
			))
	{
		if (!surface_)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	inline
	surface(
		void* pixels,
		int width,
		int height,
		int depth,
		int pitch,
		::Uint32 Rmask,
		::Uint32 Gmask,
		::Uint32 Bmask,
		::Uint32 Amask
	)
		: surface_(::SDL_CreateRGBSurfaceFrom(
				pixels,
				width,
				height,
				depth,
				pitch,
				Rmask,
				Gmask,
				Bmask,
				Amask
			))
	{
		if (!surface_)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	virtual
	~surface() noexcept {
		::SDL_FreeSurface(surface_);
	}
	
	inline
	surface(
		surface&& rhs
	) noexcept
		: surface_(rhs.surface_)
	{
		rhs.surface_ = nullptr;
	}

	surface&
	operator=(
		surface&& rhs
	) noexcept {
		if (this != &rhs) {
			if (surface_)
				::SDL_FreeSurface(surface_);
			surface_ = rhs.surface_;
			rhs.surface_ = nullptr;
		}
		return *this;
	}
	
	void
	lock() {
		if (surface_ && SDL_MUSTLOCK(surface_)) {
			if (::SDL_LockSurface(surface_))
				RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		}
	}
	
	void
	unlock() {
		if (surface_ && SDL_MUSTLOCK(surface_)) {
			::SDL_UnlockSurface(surface_);
		}
	}
	
	::SDL_Surface*
	get() noexcept {
		return surface_;
	}
	
	const ::SDL_Surface*
	get() const noexcept {
		return surface_;
	}
	
	surface
	convert(
		const ::SDL_PixelFormat& format
	) {
		RUNTIME_ASSERT(surface_);
		::SDL_Surface* sfce = ::SDL_ConvertSurface(
			surface_,
			&format,
			0
		);
		if (!sfce)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return surface(sfce);
	}
	
	surface
	convert(
		::Uint32 pxlformat
	) {
		RUNTIME_ASSERT(surface_);
		::SDL_Surface* sfce = ::SDL_ConvertSurfaceFormat(
			surface_,
			pxlformat,
			0
		);
		if (!sfce)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return surface(sfce);
	}
	
	void
	blit(
		const boost::optional<rect>& srcrect,
		surface& dst,
		const rect& dstrect
	) {
		RUNTIME_ASSERT(surface_);
		rect tmp_dstrect(dstrect);
		if (::SDL_BlitSurface(
			surface_,
			srcrect? (*srcrect).get(): nullptr,
			dst.get(),
			tmp_dstrect.get()
		))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	void
	blit_scaled(
		const boost::optional<rect>& srcrect,
		surface& dst,
		const boost::optional<rect>& dstrect
	) {
		RUNTIME_ASSERT(surface_);
		rect tmp_dstrect(dstrect? *dstrect: rect());
		if (::SDL_BlitScaled(
			surface_,
			srcrect? (*srcrect).get(): nullptr,
			dst.get(),
			dstrect? tmp_dstrect.get(): nullptr
		))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	rect
	get_clip_rect() const {
		RUNTIME_ASSERT(surface_);
		rect r;
		::SDL_GetClipRect(surface_, r.get());
		return r;
	}
	
	::Uint32
	get_color_key() const {
		RUNTIME_ASSERT(surface_);
		::Uint32 key;
		if (::SDL_GetColorKey(surface_, &key))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return key;
	}
	
	::Uint8
	get_alpha_mod() const {
		RUNTIME_ASSERT(surface_);
		::Uint8 alpha;
		if (::SDL_GetSurfaceAlphaMod(surface_, &alpha))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return alpha;
	}
	
	::SDL_BlendMode
	get_blend_mode() const {
		RUNTIME_ASSERT(surface_);
		::SDL_BlendMode blend_mode;
		if (::SDL_GetSurfaceBlendMode(surface_, &blend_mode))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return blend_mode;
	}
	
	void
	get_color_mod(
		::Uint8& r,
		::Uint8& g,
		::Uint8& b
	) const {
		RUNTIME_ASSERT(surface_);
		if (::SDL_GetSurfaceColorMod(surface_, &r, &g, &b))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	surface&
	set_clip_rect(
		const boost::optional<rect>& r = boost::none
	) {
		RUNTIME_ASSERT(surface_);
		if (::SDL_SetClipRect(surface_, r? (*r).get(): nullptr) != SDL_TRUE)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	surface&
	set_color_key(
		const bool flag,
		const ::Uint32 key
	) {
		RUNTIME_ASSERT(surface_);
		if (::SDL_SetColorKey(surface_, flag, key))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	surface&
	set_alpha_mod(
		const ::Uint8 alpha = 255
	) {
		RUNTIME_ASSERT(surface_);
		if (::SDL_SetSurfaceAlphaMod(surface_, alpha))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	surface&
	set_blend_mode(
		const ::SDL_BlendMode blend_mode
	) {
		RUNTIME_ASSERT(surface_);
		if (::SDL_SetSurfaceBlendMode(surface_, blend_mode))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	surface&
	set_color_mod(
		const ::Uint8 r = 255,
		const ::Uint8 g = 255,
		const ::Uint8 b = 255
	) {
		RUNTIME_ASSERT(surface_);
		if (SDL_SetSurfaceColorMod(surface_, r, g, b))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	surface&
	set_RLE(
		const bool flag
	) {
		RUNTIME_ASSERT(surface_);
		if (::SDL_SetSurfaceRLE(surface_, static_cast<int>(flag)))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	surface&
	fill_rect(
		const boost::optional<rect>& r,
		const ::Uint32 color
	) {
		RUNTIME_ASSERT(surface_);
		if (::SDL_FillRect(surface_, r? (*r).get(): nullptr, color))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	template
	<
		class Container
	>
	surface&
	fill_rects(
		const Container& rs,
		::Uint32 color
	) {
		RUNTIME_ASSERT(surface_);
		DOWNGRADE_CONTAINER(::SDL_Rect, ds, rs)
		if (::SDL_FillRects(surface_, ds.data(), rs.size(), color))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	int
	get_width() const {
		RUNTIME_ASSERT(surface_);
		return surface_->w;
	}
	
	int
	get_height() const {
		RUNTIME_ASSERT(surface_);
		return surface_->h;
	}
	
	point
	get_size() const {
		return point(get_width(), get_height());
	}
	
	const ::SDL_PixelFormat&
	get_format() const {
		RUNTIME_ASSERT(surface_);
		return *surface_->format;
	}
	
	::Uint32
	get_format_format() const {
		RUNTIME_ASSERT(surface_);
		return surface_->format->format;
	}
	
private:
	SDL_Surface* surface_;
};

typedef std::shared_ptr<surface> surface_ptr;

} // namespace:meck

#endif

