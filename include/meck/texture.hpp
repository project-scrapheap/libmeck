
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

#ifndef MECK_TEXTURE_HPP
#define MECK_TEXTURE_HPP

#include <algorithm>
#include <mutex>

#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include <meck/error.hpp>
#include <meck/point.hpp>
#include <meck/rect.hpp>

#include <SDL.h>

namespace meck {

class renderer;
class surface;

class texture :
	private boost::noncopyable
{
public:
	class lock_guard :
		private boost::noncopyable
	{
	public:
		typedef texture mutex_type;
		
		inline explicit
		lock_guard(
			mutex_type& m,
			boost::optional<rect> r = boost::none
		) :
			m_(m),
			pixels_(nullptr),
			pitch_(0)
		{
			m_.lock(&pixels_, &pitch_, r);
		}
		
		inline
    	~lock_guard() {
    		m_.unlock();
    	}
    	
    	inline void*
    	get_pixels() {
    		return pixels_;
    	}
    	
    	inline int
    	get_pitch() {
    		return pitch_;
    	}
    	
    	inline ::Uint32
    	get_format() {
    		return m_.get_format();
    	}
    	
	private:
		mutex_type& m_;
		void* pixels_;
		int pitch_;
	};
	
	typedef lock_guard lock_guard_type;
	
	explicit
	texture(
		::SDL_Texture* txtr
	) noexcept:
		texture_(txtr)
	{}
	
	texture(
		renderer& rndr,
		const ::Uint32 format,
		const int access,
		const int w,
		const int h
	);
	
	texture(
		renderer& rndr,
		const boost::filesystem::path& filename
	);
	
	texture(
		renderer& rndr,
		surface& srfc
	);
	
	virtual
	~texture() noexcept {
		::SDL_DestroyTexture(texture_);
	}
	
	texture(
		texture&& rhs
	) noexcept:
		texture_(rhs.texture_)
	{
		rhs.texture_ = nullptr;
	}
	
	texture&
	operator=(
		texture&& rhs
	) noexcept {
		if (this != &rhs) {
			if (texture_)
				::SDL_DestroyTexture(texture_);
			texture_ = rhs.texture_;
			rhs.texture_ = nullptr;
		}
		return *this;
	}
	
	inline void
	lock(
		void** pixels,
		int* pitch,
		boost::optional<rect> r = boost::none
	) {
		if (::SDL_LockTexture(get(), r? (*r).get(): nullptr, pixels, pitch))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	inline void
	unlock() {
		::SDL_UnlockTexture(texture_);
	}
	
	::SDL_Texture*
	get() noexcept {
		return texture_;
	}
	
	const ::SDL_Texture*
	get() const noexcept {
		return texture_;
	}
	
	texture&
	update(
		const boost::optional<rect>& r,
		const void* pixels,
		const int pitch
	);
	
	texture& update(
		const boost::optional<rect>& r,
		surface& srfc
	);
	
	texture&
	update_YUV(
		const boost::optional<rect>& r,
		const Uint8* yplane,
		const int ypitch,
		const Uint8* uplane,
		const int upitch,
		const Uint8* vplane,
		const int vpitch
	);
	
	texture&
	set_blend_mode(
		const ::SDL_BlendMode& blend_mode = SDL_BLENDMODE_NONE
	) {
		if (::SDL_SetTextureBlendMode(texture_, blend_mode))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	texture&
	set_alpha_mod(
		const ::Uint8 alpha = 255
	) {
		if (::SDL_SetTextureAlphaMod(texture_, alpha))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	texture&
	set_color_mod(
		const ::Uint8 r = 255,
		const ::Uint8 g = 255,
		const ::Uint8 b = 255
	) {
		if (::SDL_SetTextureColorMod(texture_, r, g, b))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	::Uint32
	get_format() const {
		::Uint32 format;
		if (::SDL_QueryTexture(
			texture_,
			&format,
			nullptr,
			nullptr,
			nullptr
		))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return format;
	}
	
	int
	get_access() const {
		int access;
		if (::SDL_QueryTexture(
			texture_,
			nullptr,
			&access,
			nullptr,
			nullptr
		))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return access;
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
	get_size() const {
		int w;
		int h;
		if (::SDL_QueryTexture(
			texture_,
			nullptr,
			nullptr,
			&w,
			&h
		))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return point(w, h);
	}
	
	::Uint8
	get_alpha_mod() const {
		::Uint8 alpha;
		if (::SDL_GetTextureAlphaMod(texture_, &alpha) != 0)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return alpha;
	}
	
	::SDL_BlendMode
	get_blend_mode() const {
		::SDL_BlendMode mode;
		if (::SDL_GetTextureBlendMode(texture_, &mode) != 0)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return mode;
	}
	
	void
	get_color_mod(
		::Uint8& r,
		::Uint8& g,
		::Uint8& b
	) const {
		if (::SDL_GetTextureColorMod(texture_, &r, &g, &b) != 0)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
protected:
	::SDL_Texture* texture_;
};

} // namespace:meck

#endif

