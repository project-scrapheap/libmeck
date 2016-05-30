
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

#ifndef MECK_UI_BLOCK_HPP
#define MECK_UI_BLOCK_HPP

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include <meck/hack.hpp>
#include <meck/point.hpp>
#include <meck/reactor.hpp>
#include <meck/rect.hpp>
#include <meck/renderer.hpp>
#include <meck/texture.hpp>
#include <meck/ui/overlay.hpp>

#include <SDL.h>

namespace meck {
namespace ui {

class block
	: private boost::noncopyable
	, public reactor
{
public:
	enum class positioning
	{
		AUTOMATIC,
		MANUAL
	};
	
	explicit
	block(
		overlay& olay
	)
		: owner_(olay)
		, parent_(nullptr)
		, positioning_(positioning::AUTOMATIC)
		, before_(0)
		, after_(0)
		, render_shadow_rect_(false)
		, render_outer_rect_(false)
		, render_inner_rect_(true)
		, shadow_color_ {0, 0, 0, 0}
		, outer_color_ {0, 0, 0, 0}
		, inner_color_ {0, 0, 0, 0}
		, background_(nullptr)
	{}
	
	virtual
	~block() noexcept {
	}
	
	virtual void
	finalize() {
	}
	
	virtual void
	center() {
		const point centroid(get_parent_rect().centroid());
		set_position(point(
			centroid.x() - outer_size_.x() / 2,
			centroid.y() - outer_size_.y() / 2
		));
	}
	
	virtual bool
	react(
		::SDL_Event&
	) {
		return false;
	}
	
	virtual void
	think() {
	}
	
	virtual void
	render();
	
	virtual block&
	set_parent(
		block& blk
	) {
		parent_ = &blk;
		return *this;
	}
	
	virtual block&
	set_parent() {
		parent_ = nullptr;
		return *this;
	}
	
	virtual overlay&
	get_overlay() {
		return owner_;
	}
	
	virtual const overlay&
	get_overlay() const {
		return owner_;
	}
	
	virtual boost::optional<block&>
	get_parent() {
		return parent_?
			OPT(block&, *parent_):
			OPT(block&, boost::none);
	}
	
	virtual boost::optional<const block&>
	get_parent() const {
		return parent_?
			OPT(const block&, *parent_):
			OPT(const block&, boost::none);
	}
	
	virtual rect
	get_parent_rect() {
		return parent_?
			parent_->get_inner_rect():
			owner_.get_outmost_rect();
	}
	
	virtual const rect
	get_parent_rect() const {
		return parent_?
			parent_->get_inner_rect():
			owner_.get_outmost_rect();
	}
	
	virtual rect
	get_shadow_rect() {
		return shadow_rect_;
	}
	
	virtual const rect&
	get_shadow_rect() const {
		return shadow_rect_;
	}
	
	virtual rect
	get_outer_rect() {
		return outer_rect_;
	}
	
	virtual const rect&
	get_outer_rect() const {
		return outer_rect_;
	}
	
	virtual rect
	get_inner_rect() {
		return inner_rect_;
	}
	
	virtual const rect&
	get_inner_rect() const {
		return inner_rect_;
	}
	
	virtual block&
	set_inner_color(
		const ::SDL_Color& color
	) {
		inner_color_ = color;
		return *this;
	}
	
	virtual const ::SDL_Color&
	get_inner_color() const {
		return inner_color_;
	}
	
	virtual block&
	set_size(
		const point& outer
	) {
		outer_size_ = outer;
		outer_rect_.w(outer_size_.x());
		outer_rect_.h(outer_size_.y());
		update_dependent_rects();
		return *this;
	}
	
	virtual block&
	set_size(
		const int w,
		const int h
	) {
		return set_size(point(w, h));
	}
	
	virtual block&
	set_width(
		const int w
	) {
		return set_size(point(w, outer_rect_.h()));
	}
	
	virtual block&
	set_width(
		const double w_pct
	) {
		return set_size(point(
			get_parent_rect().w() * w_pct,
			get_outer_rect().h()
		));
	}
	
	virtual block&
	expand_width() {
		return set_size(point(
			get_parent_rect().w(),
			get_outer_rect().h()
		));
	}
	
	virtual block&
	set_height(
		const int h
	) {
		return set_size(point(outer_rect_.w(), h));
	}
	
	virtual block&
	set_height(
		const double h_pct
	) {
		return set_size(point(
			get_outer_rect().w(),
			get_parent_rect().h() * h_pct
		));
	}
	
	virtual block&
	expand_height() {
		return set_size(point(
			get_outer_rect().w(),
			get_parent_rect().h()
		));
	}
	
	virtual const point&
	get_border() const {
		return border_size_;
	}
	
	virtual block&
	set_border(
		const point& border
	) {
		border_size_ = border;
		update_dependent_rects();
		return *this;
	}
	
	virtual block&
	set_border(
		const int w,
		const int h
	) {
		return set_border(point(w, h));
	}
	
	/**
	 * Manual positioning is relative to the parent rectangle.
	 */
	virtual block&
	set_position(
		const point& pos
	) {
		if (positioning_ == positioning::MANUAL) {
			outer_rect_.x(get_parent_rect().x() + pos.x());
			outer_rect_.y(get_parent_rect().y() + pos.y());
		} else {
			outer_rect_.x(pos.x());
			outer_rect_.y(pos.y());
		}
		update_dependent_rects();
		return *this;
	}
	
	virtual block&
	set_position(
		const int x,
		const int y
	) {
		return set_position(point(x, y));
	}
	
	const positioning&
	get_positioning() const {
		return positioning_;
	}
	
	void
	set_positioning(
		const positioning& pos
	) {
		positioning_ = pos;
	}
	
	int
	get_before() const {
		return before_;
	}
	
	void
	set_before(
		const int before
	) {
		before_ = before;
	}
	
	int
	get_after() const {
		return after_;
	}
	
	void
	set_after(
		const int after
	) {
		after_ = after;
	}
	
protected:
	virtual void
	update_dependent_rects() {
		inner_rect_.x(outer_rect_.x() + border_size_.x());
		inner_rect_.y(outer_rect_.y() + border_size_.y());
		inner_rect_.w(outer_rect_.w() - 2 * border_size_.x());
		inner_rect_.h(outer_rect_.h() - 2 * border_size_.y());
		
		shadow_rect_.x(outer_rect_.x() - shadow_size_.x());
		shadow_rect_.y(outer_rect_.y() - shadow_size_.y());
		shadow_rect_.w(outer_rect_.w() + 2 * shadow_size_.x());
		shadow_rect_.h(outer_rect_.h() + 2 * shadow_size_.y());
	}
	
	overlay& owner_;
	block* parent_;
	
	positioning positioning_;
	
	int before_;
	int after_;
	
	point outer_size_;
	point border_size_;
	point shadow_size_;
	
	rect shadow_rect_;
	rect outer_rect_;
	rect inner_rect_;
	
	bool render_shadow_rect_;
	bool render_outer_rect_;
	bool render_inner_rect_;
	
	::SDL_Color shadow_color_;
	::SDL_Color outer_color_;
	::SDL_Color inner_color_;
	
	texture background_;
};

} // namespace:ui
} // namespace:meck

#endif

