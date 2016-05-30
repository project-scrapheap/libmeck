
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

#ifndef MECK_UI_CONTAINER_HPP
#define MECK_UI_CONTAINER_HPP

#include <vector>

#include <boost/noncopyable.hpp>

#include <meck/point.hpp>
#include <meck/reactor.hpp>
#include <meck/rect.hpp>
#include <meck/texture.hpp>
#include <meck/ui/block.hpp>
#include <meck/ui/overlay.hpp>

#include <SDL.h>

namespace meck {
namespace ui {

class container
	: public block
{
public:
	typedef std::vector<block*> child_vector_type;
	typedef child_vector_type::iterator iterator_type;
	typedef child_vector_type::const_iterator const_iterator_type;
	
	explicit
	container(
		overlay& olay
	);
	
	virtual bool
	react(
		::SDL_Event& event
	) {
		bool reacted = false;
		for (auto it : children_)
			reacted = reacted || it->react(event);
		return reacted;
	}
	
	virtual void
	think() {
		for (auto it : children_)
			it->think();
	}
	
	virtual void
	render() {
		block::render();
		for (auto it : children_)
			it->render();
	}
	
	virtual void
	add(
		block& blk
	) {
		blk.set_parent(*this);
		children_.push_back(&blk);
	}
	
	virtual void
	remove(
		block& blk
	) {
		auto it = std::find(children_.begin(), children_.end(), &blk);
		RUNTIME_ASSERT(it != children_.end());
		(*it)->set_parent();
		children_.erase(it);
	}
	
	iterator_type
	begin() {
		return children_.begin();
	}
	
	iterator_type
	end() {
		return children_.end();
	}
	
	const_iterator_type
	begin() const {
		return children_.begin();
	}
	
	const_iterator_type
	end() const {
		return children_.end();
	}
	
protected:
	child_vector_type children_;
	point tracker_;
};

class vert_container
	: public container
{
public:
	explicit
	vert_container(
		overlay& olay
	)
		: container(olay)
	{}
	
	virtual void
	finalize() {
		int max_width = 0;
		point relative(inner_rect_.x(), inner_rect_.y());
		for (auto it : children_) {
			if (it->get_positioning() == positioning::MANUAL) {
				it->finalize();
				continue;
			} else {
				relative.y(relative.y() + it->get_before());
				it->set_position(relative);
				it->finalize();
				relative.y(relative.y()
					+ it->get_outer_rect().h()
					+ it->get_after()
				);
				max_width = std::max(
					max_width,
					it->get_outer_rect().w()
				);
			}
		}
		if (!outer_rect_.w())
			set_width(max_width);
	}
	
};

class horz_container
	: public container
{
public:
	explicit
	horz_container(
		overlay& olay
	)
		: container(olay)
	{}
	
	virtual void
	finalize() {
		int max_height = 0;
		point relative(inner_rect_.x(), inner_rect_.y());
		for (auto it : children_) {
			if (it->get_positioning() == positioning::MANUAL) {
				it->finalize();
				continue;
			} else {
				relative.x(relative.x() + it->get_before());
				it->set_position(relative);
				it->finalize();
				relative.x(relative.x()
					+ it->get_outer_rect().w()
					+ it->get_after()
				);
				max_height = std::max(
					max_height,
					it->get_outer_rect().h()
				);
			}
		}
		if (!outer_rect_.h())
			set_height(max_height);
	}
	
};

} // namespace:ui
} // namespace:meck

#endif

