
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

#ifndef MECK_TICK_HPP
#define MECK_TICK_HPP

#include <SDL.h>

namespace meck {

/**
 * Simple SDL_GetTicks()-based timer.
 */
class timer
{
public:
	explicit
	timer(
		const ::Uint32 tick = ::SDL_GetTicks()
	) noexcept :
		first_tick_(tick),
		current_tick_(tick),
		last_tick_(tick),
		tick_delta_(0)
	{}
	
	inline void
	update(
	) noexcept {
		current_tick_ = ::SDL_GetTicks();
		tick_delta_ = current_tick_ - last_tick_;
		last_tick_ = current_tick_;
	}
	
	inline ::Uint32
	delta() const noexcept {
		return tick_delta_;
	}
	
	inline ::Uint32
	last() const noexcept {
		return last_tick_;
	}
	
	inline ::Uint32
	total() const noexcept {
		return last_tick_ - first_tick_;
	}

	inline double
	delta_sec() const noexcept {
		return this->delta() / 1000.0;
	}
	
	inline double
	last_sec() const noexcept {
		return this->last() / 1000.0;
	}
	
	inline double
	total_sec() const noexcept {
		return this->total() / 1000.0;
	}

private:
	::Uint32 first_tick_;
	::Uint32 current_tick_;
	::Uint32 last_tick_;
	::Uint32 tick_delta_;
};

/**
 * Helper to handle time intervals.
 */
class interval :
	private boost::noncopyable
{
public:
	explicit
	interval(
		timer& t,
		const ::Uint32 d
	) noexcept :
		timer_(t),
		tick_(t.last()),
		delta_(d)
	{}

	explicit
	interval(
		timer& t,
		const double d
	) noexcept :
		interval(t, static_cast<::Uint32>(d * 1000.0))
	{}

	explicit
	interval(
		timer& t,
		const int d
	) noexcept :
		interval(t, static_cast<::Uint32>(d))
	{}
	
	inline bool
	test_expired() noexcept {
		return timer_.last() >= tick_ + delta_;
	}

	inline bool
	expired() noexcept {
		if (this->test_expired()) {
			this->renew();
			return true;
		}
		return false;
	}

	inline void
	renew() noexcept {
		tick_ = timer_.last();
	}

	inline void
	set_delta(
		double d
	) noexcept {
		delta_ = static_cast<::Uint32>(d * 1000.0);
	}
	
	inline void
	set_delta(
		::Uint32 d
	) noexcept {
		delta_ = d;
	}
	
	inline double
	delta_sec() const noexcept {
		return delta_ / 1000.0;
	}
	
	inline ::Uint32
	delta() const noexcept {
		return delta_;
	}
	
	inline double
	remaining_sec() const noexcept {
		return this->remaining() / 1000.0;
	}
	
	inline ::Uint32
	remaining() const noexcept {
		::Sint64 diff = static_cast<::Sint32>(tick_)
			+ static_cast<::Sint32>(delta_)
			- static_cast<::Sint32>(timer_.last());
		return static_cast<::Uint32>(diff < 0? 0: diff);
	}
	
private:
	timer& timer_;
	::Uint32 tick_;
	::Uint32 delta_;
};

} // namespace:meck

#endif

