
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

namespace meck {

application::application(
	const ::Uint32 sdl_flags,
	const ::Uint32 img_flags,
	const boost::program_options::variables_map& opt_vm
) :
	loop_(true),
	first_config_(true),
	frame_delay_(0),
	frame_count_(0),
	frames_per_second_(0),
	rendered_frames_(0),
	event_tick_lim_(10),
	opt_vm_(opt_vm),
	misc_interval_(timer_, 250),
	status_interval_(timer_, 1000)
{
	if (::SDL_Init(sdl_flags) < 0)
		RUNTIME_ERROR("SDL: %s", ::SDL_GetError());
	
	if (::IMG_Init(img_flags) < 0)
		RUNTIME_ERROR("IMG: %s", ::IMG_GetError());
	
	if (::TTF_Init() < 0)
		RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
	
	const int oa_freq = MIX_DEFAULT_FREQUENCY;
	const int oa_format = MIX_DEFAULT_FORMAT;
	if (::Mix_OpenAudio(oa_freq, oa_format, 2, 2048) < 0)
		RUNTIME_ERROR("Mix: %s", ::Mix_GetError());
}

application::~application() noexcept {
	reset();
	
	::Mix_Quit();
	::TTF_Quit();
	::IMG_Quit();
	::SDL_Quit();
}

void
application::init() {
}

void
application::reset() noexcept {
	pt_.clear();
	
	renderer_.reset();
	window_.reset();
}

void
application::configure() {
	reset();
	
	boost::property_tree::ini_parser::read_ini(
		opt_vm_["base-path"].as<std::string>() + "/config.ini",
		pt_
	);
	
	const unsigned int seed = pt_.get<unsigned int>("general.random_seed")?
		pt_.get<unsigned int>("general.random_seed"):
		opt_vm_["random-seed"].as<unsigned int>();
	
	std::srand(seed);
	
	const point pos(
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED
	);
	
	const point size(
		pt_.get<int>("graphics.width"),
		pt_.get<int>("graphics.height")
	);
	
	if (size.x() < 1 || size.y() < 1)
		RUNTIME_ERROR("invalid window dimensions: w=%1%, h=%2%",
			size.x(), size.y());
	
	window_.reset(new window(
		"meck",
		pos,
		size,
		SDL_WINDOW_SHOWN
	));
	
	const ::Uint32 mode = pt_.get<std::size_t>("graphics.fullscreen");
	const ::Uint32 modes[] = {
		0,
		SDL_WINDOW_FULLSCREEN,
		SDL_WINDOW_FULLSCREEN_DESKTOP
	};
	
	if (mode >= sizeof(modes) / sizeof(::Uint32))
		RUNTIME_ERROR("unknown screen mode index %1%", mode);
	
	window_->set_fullscreen(modes[mode]);
	
	const bool accelerated = pt_.get<bool>("graphics.accelerated");
	const bool vsync = pt_.get<bool>("graphics.vsync");
	
	renderer_.reset(new renderer(*window_, -1, 0
		| (accelerated? SDL_RENDERER_ACCELERATED: SDL_RENDERER_SOFTWARE)
		| (vsync? SDL_RENDERER_PRESENTVSYNC: 0)
	));
}

void
application::loop() {
	init();
	configure();
	
	first_config_ = false;
	
	queue_controller(get_start_controller());
	
	before_loop();

	::SDL_Event event;

	while (loop_) {
		timer_.update();
		
		if (controller_queue_.empty() || !controller_queue_.front().get())
			RUNTIME_ERROR("controller queue empty or front is NULL");
		
		controller_queue_.front()->prepare();
		
		int poll_io = 0;
		int poll_evt = 0;
		
		while ((
				(poll_io = ::Meck_PollIO(&io_service_)) ||
				(poll_evt = ::SDL_PollEvent(&event))
			) && ::SDL_GetTicks() - timer_.last() <= event_tick_lim_
		) {
			if (!poll_evt) {
				continue;
			} else if (
				event.type == SDL_QUIT ||
				event.type == SDL_WINDOWEVENT_CLOSE
			) {
				stop();
				break;
			} else if (controller_queue_.front()->react(event)) {
				break;
			}
		}
		
		controller_queue_.front()->think();
		
		before_render();
		controller_queue_.front()->render();
		after_render();
		
		if (misc_interval_.expired())
			misc();
		
		if (controller_queue_.size() > 1)
			controller_queue_.pop();
	}

	after_loop();
}

void
application::misc() {
}

void
application::before_render() {
	renderer_->clear();
}

void
application::after_render() {
    renderer_->present();
	
	frame_count_++;
	frames_per_second_ = static_cast<::Uint32>(
		++rendered_frames_ / timer_.total_sec()
	);
	
	if (status_interval_.test_expired()) {
		::SDL_SetWindowTitle(
			window_->get(),
			format("%1% [approx_fps=%2%, avg_fps=%3%, frames=%4%, ctrlr=%5%]",
				get_name(),
				frame_count_,
				frames_per_second_,
				rendered_frames_,
				controller_queue_.front()->get_name()
			).c_str()
		);

		frame_count_ = 0;
		status_interval_.renew();
	}

	if (frame_delay_)
		::SDL_Delay(frame_delay_);
}

void
application::before_loop() {
}

void
application::after_loop() {
}

controller_ptr
application::get_current_controller() {
	return controller_queue_.front();
}

void
application::queue_controller(
	controller_ptr ctrlr
) {
	controller_queue_.push(ctrlr);
	ctrlr->init();
}

} // namespace:meck

