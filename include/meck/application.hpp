
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

#ifndef MECK_APPLICATION_HPP
#define MECK_APPLICATION_HPP

#include <iostream>

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <meck/error.hpp>
#include <meck/controller.hpp>
#include <meck/keycode.hpp>
#include <meck/reactor.hpp>
#include <meck/renderer.hpp>
#include <meck/surface.hpp>
#include <meck/tick.hpp>
#include <meck/window.hpp>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#ifndef MECK_SDL_DEFAULT_INIT
#define MECK_SDL_DEFAULT_INIT \
	(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) \
	/**/
#endif

#ifndef MECK_IMG_DEFAULT_INIT
#define MECK_IMG_DEFAULT_INIT \
	(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) \
	/**/
#endif

#define APPLICATION_CONSTRUCTOR(Base, Child) \
	explicit \
	Child( \
		const ::Uint32 sdl_flags, \
		const ::Uint32 img_flags, \
		const boost::program_options::variables_map& opt_vm \
	) : \
		Base( \
			sdl_flags, \
			img_flags, \
			opt_vm \
		) \
	{} \
	/**/

#define APPLICATION_DEFAULT(Name, StartController) \
	class Name : \
		public meck::application \
	{ \
	public: \
		APPLICATION_CONSTRUCTOR(meck::application, Name) \
		\
		virtual const char* \
		get_name() const noexcept { \
			return #Name; \
		} \
		virtual meck::controller_ptr \
		get_start_controller() { \
			return meck::controller_ptr(new StartController(*this)); \
		} \
	}; \
	/**/

namespace meck {

/**
 * The application is responsible for generating events for
 * the active controller.
 */
class application
	: private boost::noncopyable
{
public:
	explicit
	application(
		const ::Uint32 sdl_flags,
		const ::Uint32 img_flags,
		const boost::program_options::variables_map& opt_vm
	);
	
	virtual
	~application() noexcept;
	
	virtual void
	init();
	
	virtual void
	reset() noexcept;
	
	virtual void
	configure();
	
	virtual void
	loop();
	
	virtual void
	misc();
	
	virtual void
	before_render();
	
	virtual void
	after_render();
	
	virtual void
	before_loop();
	
	virtual void
	after_loop();
	
	virtual void
	stop() noexcept {
		loop_ = false;
	}
	
	timer&
	get_timer() noexcept {
		return timer_;
	}
	
	interval&
	get_misc_interval() noexcept {
		return misc_interval_;
	}
	
	interval&
	get_status_interval() noexcept {
		return status_interval_;
	}
	
	boost::asio::io_service&
	get_io_service() noexcept {
		return io_service_;
	}
	
	boost::property_tree::ptree&
	get_ptree() noexcept {
		return pt_;
	}
	
	const boost::property_tree::ptree&
	get_ptree() const noexcept {
		return pt_;
	}
	
	boost::program_options::variables_map&
	get_opt_vm() noexcept {
		return opt_vm_;
	}
	
	const boost::program_options::variables_map&
	get_opt_vm() const noexcept {
		return opt_vm_;
	}
	
	virtual const char*
	get_name() const noexcept = 0;
	
	virtual controller_ptr
	get_start_controller() = 0;
	
	virtual controller_ptr
	get_current_controller();

	void
	queue_controller(
		controller_ptr ctrlr
	);
	
	window&
	get_window() {
		RUNTIME_ASSERT(window_.get());
		return *window_;
	}
	
	renderer&
	get_renderer() {
		RUNTIME_ASSERT(renderer_.get());
		return *renderer_;
	}
	
	const window&
	get_window() const {
		RUNTIME_ASSERT(window_.get());
		return *window_;
	}
	
	const renderer&
	get_renderer() const {
		RUNTIME_ASSERT(renderer_.get());
		return *renderer_;
	}
	
	window_ptr&
	get_window_ptr() {
		return window_;
	}
	
	renderer_ptr&
	get_renderer_ptr() {
		return renderer_;
	}
	
protected:
	window_ptr window_;
	renderer_ptr renderer_;
	
	bool loop_;
	bool first_config_;
	
	::Uint32 frame_delay_;
	::Uint32 frame_count_;
	::Uint32 frames_per_second_;
	::Uint32 rendered_frames_;
	::Uint32 event_tick_lim_;
	
	boost::asio::io_service io_service_;
	
	boost::property_tree::ptree pt_;
	boost::program_options::variables_map opt_vm_;
	
	timer timer_;
	interval misc_interval_;
	interval status_interval_;
	controller_queue controller_queue_;
};

/**
 * Run an application.
 */
template
<
	class Application,
	typename CharT
>
inline int
run(
	int argc,
	CharT* argv[],
	boost::program_options::options_description& opt_desc,
	const ::Uint32 sdl_flags = MECK_SDL_DEFAULT_INIT,
	const ::Uint32 img_flags = MECK_IMG_DEFAULT_INIT
) {
	try {
		boost::program_options::variables_map opt_vm;
		boost::program_options::store(
			boost::program_options::parse_command_line(
				argc,
				argv,
				opt_desc
			),
			opt_vm
		);
		
		boost::program_options::notify(opt_vm);
		Application(sdl_flags, img_flags, opt_vm).loop();
	}
	catch (const std::exception& e) {
		std::cerr
			<< format("Error: %1%", e.what())
			<< std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/**
 * Give this options_description to %run(), modified or not.
 */
inline boost::program_options::options_description
default_program_options() {
	using boost::filesystem::path;
	using boost::filesystem::current_path;
	
	boost::program_options::options_description opt_desc;
	
	const path dir_path(current_path());
	
	const path base_path_val(dir_path);
	const path resource_path_val(dir_path / "resource");
	
	opt_desc.add_options()
		("help",
			"print this help message")
		("random-seed",
			boost::program_options::value<unsigned int>()
				->default_value(static_cast<unsigned int>(std::time(nullptr))),
			"set the random generator seed")
		("base-path",
			boost::program_options::value<std::string>()
				->default_value(base_path_val.string()),
			"set the base directory path")
		("resource-path",
			boost::program_options::value<std::string>()
				->default_value(resource_path_val.string()),
			"set the resource directory path")
	;
	
	return opt_desc;
}

} // namespace:meck

inline int
Meck_PollIO(
	boost::asio::io_service* iosvc
) {
	int n = 0;
	if (iosvc) {
		n = static_cast<int>(iosvc->poll_one());
		iosvc->reset();
	}
	return n;
}

#endif

