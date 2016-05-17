
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

#ifndef MECK_POINT_HPP
#define MECK_POINT_HPP

#include <iostream>

#include <SDL.h>

namespace meck {

class point
{
public:
	typedef int value_type;
	
	inline
	point(
		const int x = 0,
		const int y = 0
	) noexcept:
		point_ {x, y}
	{}
	
	inline
	point(
		const ::SDL_Point& rhs
	) noexcept:
		point(rhs.x, rhs.y)
	{}
	
	inline
	point(
		const point&
	) noexcept = default;
	
	inline
	point(
		point&&
	) noexcept = default;
	
	inline point&
	operator=(
		const point&
	) noexcept = default;
	
	inline point&
	operator=(
		point&&
	) noexcept = default;
	
	inline virtual
	~point() noexcept
	{}
	
	inline ::SDL_Point*
	get() noexcept {
		return &point_;
	}
	
	inline const ::SDL_Point*
	get() const noexcept {
		return &point_;
	}

	inline int
	x() const noexcept {
		return point_.x;
	}
	
	inline point&
	x(
		const int n
	) noexcept {
		point_.x = n;
		return *this;
	}
	
	inline int
	y() const noexcept {
		return point_.y;
	}
	
	inline point&
	y(
		const int n
	) noexcept {
		point_.y = n;
		return *this;
	}
	
	inline point
	operator-() const noexcept {
		return point(-x(), -y());
	}
	
	inline point
	operator+(
		const point& rhs
	) const noexcept {
		return point(
			x() + rhs.x(),
			y() + rhs.y()
		);
	}
	
	inline point
	operator-(
		const point& rhs
	) const noexcept {
		return point(
			x() - rhs.x(),
			y() - rhs.y()
		);
	}
	
	inline point
	operator/(
		const int n
	) const noexcept {
		return point(
			x() / n,
			y() / n
		);
	}
	
	inline point
	operator/(
		const point& rhs
	) const noexcept {
		return point(
			x() / rhs.x(),
			y() / rhs.y()
		);
	}
	
	inline point
	operator%(
		const int n
	) const noexcept {
		return point(
			x() % n,
			y() % n
		);
	}
	
	inline point
	operator%(
		const point& rhs
	) const noexcept {
		return point(
			x() % rhs.x(),
			y() % rhs.y()
		);
	}
	
	inline point
	operator*(
		const int n
	) const noexcept {
		return point(
			x() * n,
			y() * n
		);
	}
	
	inline point
	operator*(
		const point& rhs
	) const noexcept {
		return point(
			x() * rhs.x(),
			y() * rhs.y()
		);
	}
	
	inline point&
	operator+=(
		const point& rhs
	) noexcept {
		x(x() + rhs.x());
		y(y() + rhs.y());
		return *this;
	}
	
	inline point&
	operator-=(
		const point& rhs
	) noexcept {
		x(x() - rhs.x());
		y(y() - rhs.y());
		return *this;
	}
	
	inline point&
	operator/=(
		const int n
	) noexcept {
		x(x() / n);
		y(y() / n);
		return *this;
	}
	
	inline point&
	operator/=(
		const point& rhs
	) noexcept {
		x(x() / rhs.x());
		y(y() / rhs.y());
		return *this;
	}
	
	inline point&
	operator%=(
		const int n
	) noexcept {
		x(x() % n);
		y(y() % n);
		return *this;
	}
	
	inline point&
	operator%=(
		const point& rhs
	) noexcept {
		x(x() % rhs.x());
		y(y() % rhs.y());
		return *this;
	}
	
	inline point&
	operator*=(
		const int n
	) noexcept {
		x(x() * n);
		y(y() * n);
		return *this;
	}
	
	inline point&
	operator*=(
		const point& rhs
	) noexcept {
		x(x() * rhs.x());
		y(y() * rhs.y());
		return *this;
	}
	
	template
	<
		class Rect
	>
	inline point
	get_clamped(
		const Rect& rect
	) const noexcept {
		point pt(*this);
		pt.clamp(rect);
		return pt;
	}
	
	template
	<
		class Rect
	>
	inline point&
	clamp(
		const Rect& rect
	) noexcept {
		if (x() < rect.x())
			x(rect.x());
		if (x() > rect.x2())
			x(rect.x2());
		if (y() < rect.y())
			y(rect.y());
		if (y() > rect.y2())
			y(rect.y2());
		return *this;
	}
	
	template
	<
		class Rect
	>
	inline point
	get_wrapped(
		const Rect& rect
	) const noexcept {
		point pt(*this);
		pt.wrap(rect);
		return pt;
	}
	
	template
	<
		class Rect
	>
	inline point&
	wrap(
		const Rect& rect
	) noexcept {
		if (x() < rect.x()) {
			x(rect.x() + rect.w() - 1
				- (rect.x() - x() + rect.w() - 1) % rect.w());
		} else if (x() >= rect.x() + rect.w()) {
			x(rect.x() + (x() - rect.x() - rect.w()) % rect.w());
		}
		
		if (y() < rect.y()) {
			y(rect.y() + rect.h() - 1
				- (rect.y() - y() + rect.h() - 1) % rect.h());
		} else if (y() >= rect.y() + rect.h()) {
			y(rect.y() + (y() - rect.y() - rect.h()) % rect.h());
		}
		
		return *this;
	}
	
protected:
	::SDL_Point point_;
};

} // namespace:meck

inline bool
operator==(
	const meck::point& a,
	const meck::point& b
) noexcept {
	return a.x() == b.x() && a.y() == b.y();
}

inline bool
operator!=(
	const meck::point& a,
	const meck::point& b
) {
	return !(a == b);
}

inline bool
operator<(
	const meck::point& a,
	const meck::point& b
) {
	return a.x() == b.x()? a.y() < b.y(): a.x() < b.x();
}

std::ostream&
operator<<(
	std::ostream& stream,
	const meck::point& point
);

#endif

