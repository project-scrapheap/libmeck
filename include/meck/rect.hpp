
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

#ifndef MECK_RECT_HPP
#define MECK_RECT_HPP

#include <algorithm>

#include <boost/optional.hpp>

#include <meck/hack.hpp>
#include <meck/point.hpp>

#include <SDL.h>

namespace meck {

class rect
{
public:
	typedef int value_type;
	
	static inline rect
	from_center(
		const int cx,
		const int cy,
		const int w,
		const int h
	) noexcept {
		return rect(
			cx - w / 2,
			cy - h / 2,
			w,
			h
		);
	}
	
	static inline rect
	from_center(
		const point& center,
		const point& size
	) noexcept {
		return rect(center - size / 2, size);
	}

	static inline rect
	from_corners(
		const int x1,
		const int y1,
		const int x2,
		const int y2
	) noexcept {
		return rect(x1, y1, x2 - x1, y2 - y1);
	}
	
	static inline rect
	from_corners(
		const point& p1,
		const point& p2
	) noexcept {
		return rect(p1, p2 - p1);
	}
	
	inline explicit
	rect(
		const int x = 0,
		const int y = 0,
		const int w = 0,
		const int h = 0
	) noexcept
		: rect_ {x, y, w, h}
	{
	}
	
	inline
	rect(
		const ::SDL_Rect& rhs
	) noexcept
		: rect(rhs.x, rhs.y, rhs.w, rhs.h)
	{
	}
	
	template
	<
		class point
	>
	inline
	rect(
		const point& corner,
		const point& size
	)  noexcept
		: rect(corner.x(), corner.y(), size.x(), size.y())
	{
	}
	
	inline 
	rect(
		const rect&
	) noexcept = default;
	
	inline
	rect(
		rect&&
	) noexcept = default;
	
	inline rect&
	operator=(
		const rect&
	) noexcept = default;
	
	inline rect&
	operator=(
		rect&&
	) noexcept = default;
	
	inline virtual
	~rect() noexcept
	{}
	
	inline ::SDL_Rect*
	get() noexcept {
		return &rect_;
	}
	
	inline const ::SDL_Rect*
	get() const noexcept {
		return &rect_;
	}
	
	inline int
	x() const noexcept {
		return rect_.x;
	}
	
	inline rect&
	x(
		const int n
	) noexcept {
		rect_.x = n;
		return *this;
	}
	
	inline int
	y() const noexcept {
		return rect_.y;
	}
	
	inline rect&
	y(
		const int n
	) noexcept {
		rect_.y = n;
		return *this;
	}
	
	inline int
	w() const noexcept {
		return rect_.w;
	}
	
	inline rect&
	w(
		const int n
	) noexcept {
		rect_.w = n;
		return *this;
	}
	
	inline int
	h() const noexcept {
		return rect_.h;
	}
	
	inline rect&
	h(
		const int n
	) noexcept {
		rect_.h = n;
		return *this;
	}
	
	inline int
	x2() const noexcept {
		return x() + w() - 1;
	}
	
	inline rect&
	x2(
		const int n
	) noexcept {
		w(n - x() + 1);
		return *this;
	}
	
	inline int 
	y2() const noexcept {
		return y() + h() - 1;
	}
	
	inline rect&
	y2(
		const int n
	) noexcept {
		h(n - y() + 1);
		return *this;
	}
	
	inline point
	get_top_left() const noexcept {
		return point(x(), y());
	}
	
	inline point
	get_top_right() const noexcept {
		return point(x2(), y());
	}
	
	inline point
	get_bottom_left() const noexcept {
		return point(x(), y2());
	}
	
	inline point
	get_bottom_right() const noexcept {
		return point(x2(), y2());
	}
	
	inline point
	size() const noexcept {
		return point(w(), h());
	}
	
	inline point
	centroid() const noexcept {
		return point(x() + w() / 2, y() + h() / 2);
	}
	
	inline bool
	contains(
		const int px,
		const int py
	) const noexcept {
		return px >= x() && py >= y() &&
			px <= x2() && py <= y2();
	}
	
	inline bool
	contains(
		const point& pt
	) const noexcept {
		return contains(pt.x(), pt.y());
	}
	
	inline bool
	contains(
		const rect& r
	) const noexcept {
		return r.x() >= x() && r.y() >= y() &&
			r.x2() <= x2() && r.y2() <= y2();
	}
	
	inline bool
	intersects(
		const rect& r
	) const noexcept {
#ifndef MECK_RECT_INTERSECTS_SDL2
		return !(
			r.x2() < x() ||
			r.y2() < y() ||
			r.x() > x2() ||
			r.y() > y2()
		);
#else
		return ::SDL_HasIntersection(get(), r.get()) == SDL_TRUE;
#endif
	}
	
	inline boost::optional<rect>
	get_intersection(
		const rect& r
	) const noexcept {
#ifndef MECK_RECT_INTERSECTION_SDL2
		return !intersects(r)?
			OPT(rect, boost::none):
			OPT(rect, rect::from_corners(
				std::max(x(), r.x()),
				std::max(y(), r.y()),
				std::min(x2(), r.x2()),
				std::min(y2(), r.y2())
			));
#else
		::SDL_Rect result;
		return ::SDL_IntersectRect(get(), r.get(), &result) == SDL_FALSE?
			OPT(rect, boost::none): OPT(rect, rect(result));
#endif
	}
	
	inline rect
	get_u(
		const rect& r
	) const noexcept {
		rect rr(*this);
		rr.u(r);
		return rr;
	}
	
	inline rect&
	u(
		const rect& r
	) noexcept {
		x(std::min(x(), r.x()));
		y(std::min(y(), r.y()));
		x2(std::max(x2(), r.x2()));
		y2(std::max(y2(), r.y2()));
		return *this;
	}
	
	inline rect
	get_extension(
		const unsigned int n
	) const noexcept {
		rect r(*this);
		r.extend(n);
		return r;
	}
	
	inline rect
	get_extension(
		const unsigned int nh,
		const unsigned int nv
	) const noexcept {
		rect r(*this);
		r.extend(nh, nv);
		return r;
	}

	inline rect&
	extend(
		const unsigned int n
	) noexcept {
		return extend(n, n);
	}

	inline rect&
	extend(
		const unsigned int nh,
		const unsigned int nv
	) noexcept {
		x(x() - nh);
		y(y() - nv);
		w(w() + nh * 2);
		h(h() + nv * 2);
		return *this;
	}
	
	inline bool
	intersect_line(
		int& nx1,
		int& ny1,
		int& nx2,
		int& ny2
	) const noexcept {
		return ::SDL_IntersectRectAndLine(
			get(),
			&nx1,
			&ny1,
			&nx2,
			&ny2
		) == SDL_TRUE;
	}
	
	inline bool
	intersect_line(
		point& p1,
		point& p2
	) const noexcept {
		return ::SDL_IntersectRectAndLine(
			get(),
			&p1.get()->x,
			&p1.get()->y,
			&p2.get()->x,
			&p2.get()->y
		) == SDL_TRUE;
	}
	
	inline rect
	operator+(
		const point& offset
	) const noexcept {
		return rect(
			x() + offset.x(),
			y() + offset.y(),
			w(),
			h()
		);
	}
	
	inline rect
	operator-(
		const point& offset
	) const noexcept {
		return rect(
			x() - offset.x(),
			y() - offset.y(),
			w(),
			h()
		);
	}
	
	inline rect&
	operator+=(
		const point& offset
	) noexcept {
		x(x() + offset.x());
		y(y() + offset.y());
		return *this;
	}
	
	inline rect&
	operator-=(
		const point& offset
	) noexcept {
		x(x() - offset.x());
		y(y() - offset.y());
		return *this;
	}
	
protected:
	::SDL_Rect rect_;
};

} // namespace:meck

inline bool
operator==(
	const meck::rect& a,
	const meck::rect& b
) noexcept {
	return (
		a.x() == b.x() &&
		a.y() == b.y() &&
		a.w() == b.w() &&
		a.h() == b.h()
	);
}

inline bool
operator!=(
	const meck::rect& a,
	const meck::rect& b
) noexcept {
	return !(a == b);
}

inline bool
operator<(
	const meck::rect& a,
	const meck::rect& b
) noexcept {
    if (a.x() == b.x()) {
		if (a.y() == b.y()) {
			if (a.w() == b.w()) {
				return a.h() < b.h();
			}
			return a.w() < b.w();
		}
		return a.y() < b.y();
	}
	return a.x() < b.x();
}

std::ostream&
operator<<(
	std::ostream& stream,
	const meck::rect& rect
);

#endif

