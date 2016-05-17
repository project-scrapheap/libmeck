
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

#ifndef MECK_RENDERER_HPP
#define MECK_RENDERER_HPP

#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include <meck/error.hpp>
#include <meck/hack.hpp>
#include <meck/point.hpp>
#include <meck/rect.hpp>
#include <meck/surface.hpp>
#include <meck/texture.hpp>
#include <meck/window.hpp>

#include <SDL.h>

namespace meck {

class renderer :
	private boost::noncopyable
{
public:
	explicit
	renderer(
		::SDL_Renderer* rndr
	) noexcept:
		renderer_(rndr)
	{}
	
	renderer(
		window& win,
		const int index,
		const ::Uint32 flags
	) :
		renderer_(::SDL_CreateRenderer(
			win.get(),
			index,
			flags
		))
	{
		if (!renderer_)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	virtual
	~renderer() noexcept {
		::SDL_DestroyRenderer(renderer_);
	}
	
	renderer(
		renderer&& rhs
	) noexcept:
		renderer_(rhs.renderer_)
	{
		rhs.renderer_ = nullptr;
	}
	
	renderer&
	operator=(
		renderer&& rhs
	) noexcept {
		if (this != &rhs) {
			if (renderer_)
				::SDL_DestroyRenderer(renderer_);
			renderer_ = rhs.renderer_;
			rhs.renderer_ = nullptr;
		}
		return *this;
	}
	
	::SDL_Renderer*
	get() noexcept {
		return renderer_;
	}
	
	const ::SDL_Renderer*
	get() const noexcept {
		return renderer_;
	}
	
	renderer&
	present() {
		::SDL_RenderPresent(renderer_);
		return *this;
	}
	
	renderer&
	clear() {
		if (::SDL_RenderClear(renderer_))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	void
	get_info(
		::SDL_RendererInfo& info
	) {
		if (::SDL_GetRendererInfo(renderer_, &info) != 0)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	renderer&
	copy(
		texture& txtre,
		const boost::optional<rect>& srcrect = boost::none,
		const boost::optional<rect>& dstrect = boost::none
	) {
		if (::SDL_RenderCopy(
			renderer_,
			txtre.get(),
			srcrect? (*srcrect).get(): nullptr,
			dstrect? (*dstrect).get(): nullptr
		))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	renderer&
	copy(
		texture& txtre,
		const boost::optional<rect>& srcrect,
		const point& dstpoint
	) {
		rect dstrect(
			dstpoint.x(),
			dstpoint.y(),
			srcrect? srcrect->w(): txtre.get_width(),
			srcrect? srcrect->h(): txtre.get_height()
		);
		return copy(txtre, srcrect, dstrect);
	}
	
	renderer&
	copy(
		texture& txtre,
		const boost::optional<rect>& srcrect,
		const boost::optional<rect>& dstrect,
		const double angle,
		const boost::optional<point>& center = boost::none,
		const int flip = 0
	) {
		if (::SDL_RenderCopyEx(
			renderer_,
			txtre.get(),
			srcrect? (*srcrect).get(): nullptr,
			dstrect? (*dstrect).get(): nullptr,
			angle,
			center? (*center).get(): nullptr,
			static_cast<::SDL_RendererFlip>(flip)
		))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	renderer&
	copy(
		texture& txtre,
		const boost::optional<rect>& srcrect,
		const point& dstpoint,
		const double angle,
		const boost::optional<point>& center = boost::none,
		const int flip = 0
	) {
		rect dstrect(
			dstpoint.x(),
			dstpoint.y(),
			srcrect? (*srcrect).w(): txtre.get_width(),
			srcrect? (*srcrect).h(): txtre.get_height()
		);
		return copy(txtre, srcrect, dstrect, angle, center, flip);
	}
	
	renderer&
	fill_copy(
		texture& txtre,
		const boost::optional<rect>& srcrect = boost::none,
		const boost::optional<rect>& dstrect = boost::none,
		const point& offset = point(0, 0),
		const int flip = 0
	);
	
	renderer& set_draw_color(
		const ::Uint8 r = 0,
		const ::Uint8 g = 0,
		const ::Uint8 b = 0,
		const ::Uint8 a = 255
	) {
		if (::SDL_SetRenderDrawColor(renderer_, r, g, b, a))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	renderer&
	set_target() {
		if (::SDL_SetRenderTarget(renderer_, nullptr))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	renderer&
	set_target(texture& txtre) {
		if (::SDL_SetRenderTarget(renderer_, txtre.get()))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	renderer&
	set_draw_blend_mode(
		::SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE
	) {
		if (::SDL_SetRenderDrawBlendMode(renderer_, blend_mode))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	renderer&
	draw_point(
		const int x,
		const int y
	) {
		return draw_point(point(x, y));
	}
	
	renderer&
	draw_point(
		const point& p
	) {
		if (::SDL_RenderDrawPoint(renderer_, p.x(), p.y()))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	template
	<
		class Container
	>
	renderer&
	draw_points(
		const Container& ps
	) {
		DOWNGRADE_CONTAINER(::SDL_Point, ds, ps)
		if (::SDL_RenderDrawPoints(renderer_, ds.data(), ps.size()))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	renderer&
	draw_line(
		const int x1,
		const int y1,
		const int x2,
		const int y2
	) {
		return draw_line(point(x1, y1), point(x2, y2));
	}
	
	renderer&
	draw_line(
		const point& p1,
		const point& p2
	) {
		if (::SDL_RenderDrawLine(
			renderer_,
			p1.x(),
			p1.y(),
			p2.x(),
			p2.y()
		))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	template
	<
		class Container
	>
	renderer&
	draw_lines(
		const Container& ps
	) {
		DOWNGRADE_CONTAINER(::SDL_Point, ds, ps)
		if (::SDL_RenderDrawLines(renderer_, ds.data(), ps.size()))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	renderer&
	draw_rect(
		const int x1,
		const int y1,
		const int x2,
		const int y2
	) {
		return draw_rect(rect(
			point(x1, y1),
			point(x2 - x1, y2 - y1)
		));
	}
	
	renderer&
	draw_rect(
		const point& p1,
		const point& p2
	) {
		return draw_rect(p1.x(), p1.y(), p2.x(), p2.y());
	}
	
	renderer&
	draw_rect(
		const rect& r
	) {
		if (::SDL_RenderDrawRect(renderer_, r.get()))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	template
	<
		class Container
	>
	renderer&
	draw_rects(
		const Container& rs
	) {
		DOWNGRADE_CONTAINER(::SDL_Rect, ds, rs)
		if (::SDL_RenderDrawRects(renderer_, ds.data(), rs.size()))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	renderer&
	fill_rect(
		const int x1,
		const int y1,
		const int x2,
		const int y2
	) {
		return fill_rect(rect(
			point(x1, y1),
			point(x2 - x1, y2 - y1)
		));
	}
	
	renderer&
	fill_rect(
		const point& p1,
		const point& p2
	) {
		return fill_rect(p1.x(), p1.y(), p2.x(), p2.y());
	}
	
	renderer&
	fill_rect(
		const rect& r
	) {
		if (::SDL_RenderFillRect(renderer_, r.get()))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	template
	<
		class Container
	>
	renderer&
	fill_rects(
		const Container& rs
	) {
		DOWNGRADE_CONTAINER(::SDL_Rect, ds, rs)
		if (::SDL_RenderDrawRects(renderer_, ds.data(), rs.size()))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	void
	read_pixels(
		const boost::optional<rect>& r,
		const ::Uint32 format,
		void* pixels,
		int pitch
	) {
		if (::SDL_RenderReadPixels(
			renderer_,
			r? (*r).get(): nullptr,
			format,
			pixels,
			pitch
		))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	renderer&
	set_clip_rect(
		const boost::optional<rect>& r = boost::none
	) {
		if (::SDL_RenderSetClipRect(renderer_, r? (*r).get(): nullptr))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	renderer&
	set_logical_size(
		const int w,
		const int h
	) {
		if (::SDL_RenderSetLogicalSize(renderer_, w, h))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	renderer&
	set_scale(
		const float scaleX,
		const float scaleY
	) {
		if (::SDL_RenderSetScale(renderer_, scaleX, scaleY) != 0)
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	renderer&
	set_viewport(
		const boost::optional<rect>& r = boost::none
	) {
		if (::SDL_RenderSetViewport(renderer_, r? (*r).get(): nullptr))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return *this;
	}
	
	bool
	target_supported() const {
		return ::SDL_RenderTargetSupported(renderer_) == SDL_TRUE;
	}
	
	boost::optional<rect>
	get_clip_rect() const {
		::SDL_Rect r;
		::SDL_RenderGetClipRect(renderer_, &r);
		return ::SDL_RectEmpty(&r)?
			OPT(rect, boost::none): OPT(rect, rect(r));
	}
	
	point
	get_logical_size() const {
		int w;
		int h;
		::SDL_RenderGetLogicalSize(renderer_, &w, &h);
		return point(w, h);
	}
	
	int
	get_logical_width() const {
		return get_logical_size().x();
	}
	
	int get_logical_height() const {
		return get_logical_size().y();
	}
	
	void get_scale(
		float& scaleX,
		float& scaleY
	) const {
		scaleX = get_x_scale();
		scaleY = get_y_scale();
	}
	
	float
	get_x_scale() const {
		float scaleX;
		::SDL_RenderGetScale(renderer_, &scaleX, nullptr);
		return scaleX;
	}
	
	float
	get_y_scale() const {
		float scaleY;
		::SDL_RenderGetScale(renderer_, nullptr, &scaleY);
		return scaleY;
	}
	
	rect
	get_viewport() const {
		::SDL_Rect r;
		::SDL_RenderGetViewport(renderer_, &r);
		return rect(r);
	}
	
	::SDL_BlendMode
	get_draw_blend_mode() const {
		::SDL_BlendMode mode;
		if (::SDL_GetRenderDrawBlendMode(renderer_, &mode))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return mode;
	}
	
	void
	get_draw_color(
		::Uint8& r,
		::Uint8& g,
		::Uint8& b,
		::Uint8& a
	) const {
		if (::SDL_GetRenderDrawColor(renderer_, &r, &g, &b, &a))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	}
	
	point
	get_output_size() const {
		int w;
		int h;
		if (::SDL_GetRendererOutputSize(renderer_, &w, &h))
			RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
		return point(w, h);
	}
	
	int
	get_output_width() const {
		return get_output_size().x();
	}
	
	int
	get_output_height() const {
		return get_output_size().y();
	}
	
protected:
	::SDL_Renderer* renderer_;
};

typedef std::shared_ptr<renderer> renderer_ptr;

} // namespace:meck

#endif

