
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

#ifndef MECK_FONT_HPP
#define MECK_FONT_HPP

#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include <meck/point.hpp>
#include <meck/renderer.hpp>
#include <meck/surface.hpp>
#include <meck/texture.hpp>

#include <SDL.h>
#include <SDL_ttf.h>

#define u16string_to_u16pointer_return(Parameter, Function, ...) \
	std::vector<::Uint16> uint16_text(Parameter.length() + 1); \
	std::copy(Parameter.begin(), Parameter.end(), uint16_text.begin()); \
	return Function(uint16_text.data(), ##__VA_ARGS__); \
	/**/

namespace meck {

class font :
	private boost::noncopyable
{
public:
	explicit
	font(
		::TTF_Font* f
	) noexcept:
		font_(f)
	{}
	
	font(
		const boost::filesystem::path& file,
		const int ptsize,
		const long index = 0
	) :
		font_(::TTF_OpenFontIndex(
			file.string().c_str(),
			ptsize,
			index
		))
	{
		if (!font_)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
	}
	
	virtual
	~font() noexcept {
		::TTF_CloseFont(font_);
	}
	
	font(
		font&& rhs
	) noexcept:
		font_(rhs.font_)
	{
		rhs.font_ = nullptr;
	}
	
	font&
	operator=(
		font&& rhs
	) noexcept {
		if (this != &rhs) {
			if (font_)
				::TTF_CloseFont(font_);
			font_ = rhs.font_;
			rhs.font_ = nullptr;
		}
		return *this;
	}
	
	::TTF_Font*
	get() noexcept {
		return font_;
	}
	
	const ::TTF_Font*
	get() const noexcept {
		return font_;
	}
	
	int
	get_style() const {
		return ::TTF_GetFontStyle(font_);
	}
	
	font&
	set_style(
		const int style = TTF_STYLE_NORMAL
	) {
		::TTF_SetFontStyle(font_, style);
		return *this;
	}
	
	int
	get_outline() const {
		return ::TTF_GetFontOutline(font_);
	}
	
	font&
	set_outline(
		const int outline = 0
	) {
		::TTF_SetFontOutline(font_, outline);
		return *this;
	}
	
	int
	get_hinting() const {
		return ::TTF_GetFontHinting(font_);
	}
	
	font&
	set_hinting(
		const int hinting = TTF_HINTING_NORMAL
	) {
		::TTF_SetFontHinting(font_, hinting);
		return *this;
	}
	
	bool
	get_kerning() const {
		return static_cast<bool>(::TTF_GetFontKerning(font_));
	}
	
	font&
	set_kerning(
		const bool allowed = true
	) {
		::TTF_SetFontKerning(font_, allowed);
		return *this;
	}
	
	int
	get_height() const {
		return ::TTF_FontHeight(font_);
	}
	
	int
	get_ascent() const {
		return ::TTF_FontAscent(font_);
	}
	
	int
	get_descent() const {
		return ::TTF_FontDescent(font_);
	}
	
	int
	get_line_skip() const {
		return ::TTF_FontLineSkip(font_);
	}
	
	long
	get_num_faces() const {
		return ::TTF_FontFaces(font_);
	}
	
	bool is_fixed_width() const {
		return ::TTF_FontFaceIsFixedWidth(font_);
	}
	
	boost::optional<std::string>
	get_family_name() const {
		const char* name = ::TTF_FontFaceFamilyName(font_);
		return name?
			OPT(std::string, std::string(name)):
			OPT(std::string, boost::none);
	}
	
	boost::optional<std::string>
	get_style_name() const {
		const char* name = ::TTF_FontFaceStyleName(font_);
		return name?
			OPT(std::string, std::string(name)):
			OPT(std::string, boost::none);
	}
	
	int
	is_glyph_provided(
		const ::Uint16 ch
	) const {
		return ::TTF_GlyphIsProvided(font_, ch);
	}
	
	void
	get_glyph_metrics(
		const ::Uint16 ch,
		int& mx,
		int& Mx,
		int& my,
		int& My,
		int& advance
	) const {
		if (::TTF_GlyphMetrics(font_, ch, &mx, &Mx, &my, &My, &advance))
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
	}
	
	rect
	get_glyph_rect(
		const ::Uint16 ch
	) const {
		int mx;
		int Mx;
		int my;
		int My;
		int advance;
		get_glyph_metrics(ch, mx, Mx, my, My, advance);
		return rect(mx, my, Mx - mx, My - my);
	}
	
	int
	get_glyph_advance(
		const ::Uint16 ch
	) const {
		int mx;
		int Mx;
		int my;
		int My;
		int advance;
		get_glyph_metrics(ch, mx, Mx, my, My, advance);
		return advance;
	}
	
	point
	get_size_text(
		const std::string& text
	) const {
		int w;
		int h;
		if (::TTF_SizeText(font_, text.c_str(), &w, &h))
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return point(w, h);
	}
	
	point
	get_size_UTF8(
		const std::string& text
	) const {
		int w;
		int h;
		if (::TTF_SizeUTF8(font_, text.c_str(), &w, &h))
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return point(w, h);
	}
	
	point
	get_size_UNICODE(
		const ::Uint16* text
	) const {
		int w;
		int h;
		if (::TTF_SizeUNICODE(font_, text, &w, &h))
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return point(w, h);
	}
	
	point
	get_size_UNICODE(
		const std::u16string& text
	) const {
		u16string_to_u16pointer_return(text, get_size_UNICODE);
	}
	
	surface
	render_text_solid(
		const std::string& text,
		const ::SDL_Color fg
	) {
		::SDL_Surface* s = ::TTF_RenderText_Solid(font_, text.c_str(), fg);
		if (!s)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return surface(s);
	}
	
	surface
	render_UTF8_solid(
		const std::string& text,
		const ::SDL_Color fg
	) {
		::SDL_Surface* s = ::TTF_RenderUTF8_Solid(font_, text.c_str(), fg);
		if (!s)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return surface(s);
	}
	
	surface
	render_UNICODE_solid(
		const Uint16* text,
		const ::SDL_Color fg
	) {
		::SDL_Surface* s = ::TTF_RenderUNICODE_Solid(font_, text, fg);
		if (!s)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return surface(s);
	}
	
	surface
	render_UNICODE_solid(
		const std::u16string& text,
		const ::SDL_Color fg
	) {
		u16string_to_u16pointer_return(text, render_UNICODE_solid, fg);
	}
	
	surface
	render_glyph_solid(
		const ::Uint16 ch,
		const ::SDL_Color fg
	) {
		::SDL_Surface* s = TTF_RenderGlyph_Solid(font_, ch, fg);
		if (!s)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return surface(s);
	}
	
	texture
	render_text_solid(
		renderer& rndr,
		const std::string& text,
		const ::SDL_Color fg
	) {
		surface srfc(render_text_solid(text, fg));
		return texture(rndr, srfc);
	}
	
	texture
	render_UTF8_solid(
		renderer& rndr,
		const std::string& text,
		const ::SDL_Color fg
	) {
		surface srfc(render_UTF8_solid(text, fg));
		return texture(rndr, srfc);
	}
	
	texture
	render_UNICODE_solid(
		renderer& rndr,
		const Uint16* text,
		const ::SDL_Color fg
	) {
		surface srfc(render_UNICODE_solid(text, fg));
		return texture(rndr, srfc);
	}
	
	texture
	render_UNICODE_solid(
		renderer& rndr,
		const std::u16string& text,
		const ::SDL_Color fg
	) {
		surface srfc(render_UNICODE_solid(text, fg));
		return texture(rndr, srfc);
	}
	
	texture
	render_glyph_solid(
		renderer& rndr,
		const ::Uint16 ch,
		const ::SDL_Color fg
	) {
		surface srfc(render_glyph_solid(ch, fg));
		return texture(rndr, srfc);
	}
	
	surface
	render_text_shaded(
		const std::string& text,
		const ::SDL_Color fg,
		const ::SDL_Color bg
	) {
		::SDL_Surface* s = TTF_RenderText_Shaded(font_, text.c_str(), fg, bg);
		if (!s)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return surface(s);
	}
	
	surface
	render_UTF8_shaded(
		const std::string& text,
		const ::SDL_Color fg,
		const ::SDL_Color bg
	) {
		::SDL_Surface* s = TTF_RenderUTF8_Shaded(font_, text.c_str(), fg, bg);
		if (!s)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return surface(s);
	}
	
	surface
	render_UNICODE_shaded(
		const Uint16* text,
		const ::SDL_Color fg,
		const ::SDL_Color bg
	) {
		::SDL_Surface* s = TTF_RenderUNICODE_Shaded(font_, text, fg, bg);
		if (!s)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return surface(s);
	}
	
	surface
	render_UNICODE_shaded(
		const std::u16string& text,
		const ::SDL_Color fg,
		const ::SDL_Color bg
	) {
		u16string_to_u16pointer_return(text, render_UNICODE_shaded, fg, bg);
	}
	
	surface
	render_glyph_shaded(
		const ::Uint16 ch,
		const ::SDL_Color fg,
		const ::SDL_Color bg
	) {
		SDL_Surface* s = TTF_RenderGlyph_Shaded(font_, ch, fg, bg);
		if (!s)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return surface(s);
	}
	
	texture
	render_text_shaded(
		renderer& rndr,
		const std::string& text,
		const ::SDL_Color fg,
		const ::SDL_Color bg
	) {
		surface srfc(render_text_shaded(text, fg, bg));
		return texture(rndr, srfc);
	}
	
	texture
	render_UTF8_shaded(
		renderer& rndr,
		const std::string& text,
		const ::SDL_Color fg,
		const ::SDL_Color bg
	) {
		surface srfc(render_UTF8_shaded(text, fg, bg));
		return texture(rndr, srfc);
	}
	
	texture
	render_UNICODE_shaded(
		renderer& rndr,
		const Uint16* text,
		const ::SDL_Color fg,
		const ::SDL_Color bg
	) {
		surface srfc(render_UNICODE_shaded(text, fg, bg));
		return texture(rndr, srfc);
	}
	
	texture
	render_UNICODE_shaded(
		renderer& rndr,
		const std::u16string& text,
		const ::SDL_Color fg,
		const ::SDL_Color bg
	) {
		surface srfc(render_UNICODE_shaded(text, fg, bg));
		return texture(rndr, srfc);
	}
	
	texture
	render_glyph_shaded(
		renderer& rndr,
		const ::Uint16 ch,
		const ::SDL_Color fg,
		const ::SDL_Color bg
	) {
		surface srfc(render_glyph_shaded(ch, fg, bg));
		return texture(rndr, srfc);
	}
	
	surface
	render_text_blended(
		const std::string& text,
		const ::SDL_Color fg
	) {
		::SDL_Surface* s = TTF_RenderText_Blended(font_, text.c_str(), fg);
		if (!s)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return surface(s);
	}
	
	surface
	render_UTF8_blended(
		const std::string& text,
		const ::SDL_Color fg
	) {
		::SDL_Surface* s = TTF_RenderUTF8_Blended(font_, text.c_str(), fg);
		if (!s)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return surface(s);
	}
	
	surface
	render_UNICODE_blended(
		const Uint16* text,
		const ::SDL_Color fg
	) {
		::SDL_Surface* s = TTF_RenderUNICODE_Blended(font_, text, fg);
		if (!s)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return surface(s);
	}
	
	surface
	render_UNICODE_blended(
		const std::u16string& text,
		const ::SDL_Color fg
	) {
		u16string_to_u16pointer_return(text, render_UNICODE_blended, fg);
	}
	
	surface
	render_glyph_blended(
		const ::Uint16 ch,
		const ::SDL_Color fg
	) {
		SDL_Surface* s = TTF_RenderGlyph_Blended(font_, ch, fg);
		if (!s)
			RUNTIME_ERROR("TTF: %s", ::TTF_GetError());
		return surface(s);
	}
	
	texture
	render_text_blended(
		renderer& rndr,
		const std::string& text,
		const ::SDL_Color fg
	) {
		surface srfc(render_text_blended(text, fg));
		return texture(rndr, srfc);
	}
	
	texture
	render_UTF8_blended(
		renderer& rndr,
		const std::string& text,
		const ::SDL_Color fg
	) {
		surface srfc(render_UTF8_blended(text, fg));
		return texture(rndr, srfc);
	}
	
	texture
	render_UNICODE_blended(
		renderer& rndr,
		const Uint16* text,
		const ::SDL_Color fg
	) {
		surface srfc(render_UNICODE_blended(text, fg));
		return texture(rndr, srfc);
	}
	
	texture
	render_UNICODE_blended(
		renderer& rndr,
		const std::u16string& text,
		const ::SDL_Color fg
	) {
		surface srfc(render_UNICODE_blended(text, fg));
		return texture(rndr, srfc);
	}
	
	texture
	render_glyph_blended(
		renderer& rndr,
		const ::Uint16 ch,
		const ::SDL_Color fg
	) {
		surface srfc(render_glyph_blended(ch, fg));
		return texture(rndr, srfc);
	}
	
private:
	::TTF_Font* font_;
};

} // namespace:meck

#undef u16string_to_u16pointer_return

#endif



