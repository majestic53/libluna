/**
 * libluna
 * Copyright (C) 2015 David Jolly
 * ----------------------
 *
 * libluna is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libluna is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/luna.h"
#include "../include/luna_display_type.h"

namespace LUNA {

	namespace COMP {

		_luna_display_config::_luna_display_config(
			__in_opt const std::string &title,
			__in_opt size_t width,
			__in_opt size_t height,
			__in_opt size_t x,
			__in_opt size_t y,
			__in_opt uint32_t flags
			) :
				m_flags(flags),
				m_height(height),
				m_x(x),
				m_width(width),
				m_y(y),
				m_title(title)
		{
			return;
		}

		_luna_display_config::_luna_display_config(
			__in const _luna_display_config &other
			) :
				m_flags(other.m_flags),
				m_height(other.m_height),
				m_x(other.m_x),
				m_width(other.m_width),
				m_y(other.m_y),
				m_title(other.m_title)
		{
			return;
		}

		_luna_display_config::~_luna_display_config(void)
		{
			return;
		}

		_luna_display_config &
		_luna_display_config::operator=(
			__in const _luna_display_config &other
			)
		{

			if(this != &other) {
				m_flags = other.m_flags;
				m_height = other.m_height;
				m_title = other.m_title;
				m_x = other.m_x;
				m_width = other.m_width;
				m_y = other.m_y;
			}

			return *this;
		}

		void 
		_luna_display_config::clear(void)
		{
			m_flags = 0;
			m_height = 0;
			m_x = 0;
			m_width = 0;
			m_y = 0;
			m_title.clear();
		}

		uint32_t &
		_luna_display_config::flags(void)
		{
			return m_flags;
		}

		size_t &
		_luna_display_config::height(void)
		{
			return m_height;
		}

		std::string &
		_luna_display_config::title(void)
		{
			return m_title;
		}

		std::string 
		_luna_display_config::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			UNREFERENCE_PARAM(verbose);

			result << "\"" << STRING_CHECK(m_title) << "\", " << m_width << "x" << m_height
				<< " (" << m_x << "x" << m_y << "), FLG. 0x"
				<< SCALAR_AS_HEX(uint32_t, m_flags);

			return result.str();
		}

		size_t &
		_luna_display_config::x(void)
		{
			return m_x;
		}

		size_t &
		_luna_display_config::width(void)
		{
			return m_width;
		}

		size_t &
		_luna_display_config::y(void)
		{
			return m_y;
		}

		_luna_display *_luna_display::m_instance = NULL;

		_luna_display::_luna_display(void) :
			m_initialized(false),
			m_running(false),
			m_window(NULL)
		{
			std::atexit(luna_display::_delete);
		}

		_luna_display::~_luna_display(void)
		{

			if(m_initialized ) {
				uninitialize();
			}
		}

		void 
		_luna_display::_delete(void)
		{

			if(luna_display::m_instance) {
				delete luna_display::m_instance;
				luna_display::m_instance = NULL;
			}
		}

		_luna_display *
		_luna_display::acquire(void)
		{

			if(!luna_display::m_instance) {

				luna_display::m_instance = new luna_display;
				if(!luna_display::m_instance) {
					THROW_LUNA_DISPLAY_EXCEPTION(LUNA_DISPLAY_EXCEPTION_ALLOCATED);
				}
			}

			return luna_display::m_instance;
		}

		void 
		_luna_display::clear(void)
		{

			if(!m_initialized) {
				THROW_LUNA_DISPLAY_EXCEPTION(LUNA_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			m_config.clear();
		}

		void 
		_luna_display::initialize(void)
		{

			if(m_initialized) {
				THROW_LUNA_DISPLAY_EXCEPTION(LUNA_DISPLAY_EXCEPTION_INITIALIZED);
			}

			m_initialized = true;
		}

		bool 
		_luna_display::is_allocated(void)
		{
			return (luna_display::m_instance != NULL);
		}

		bool 
		_luna_display::is_initialized(void)
		{
			return m_initialized;
		}

		bool 
		_luna_display::is_running(void)
		{
			return m_running;
		}

		void 
		_luna_display::set(
			__in const luna_display_config &config
			)
		{

			if(!m_initialized) {
				THROW_LUNA_DISPLAY_EXCEPTION(LUNA_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			m_config = config;
		}

		void 
		_luna_display::start(
			__in const luna_display_config &config
			)
		{

			if(!m_initialized) {
				THROW_LUNA_DISPLAY_EXCEPTION(LUNA_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			if(m_running) {
				THROW_LUNA_DISPLAY_EXCEPTION(LUNA_DISPLAY_EXCEPTION_STARTED);
			}

			set(config);
			m_window = SDL_CreateWindow(STRING_CHECK(m_config.title()), m_config.x(), m_config.y(), 
				m_config.width(), m_config.height(), m_config.flags());
			if(!m_window) {
				THROW_LUNA_DISPLAY_EXCEPTION_FORMAT(LUNA_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_CreateWindow failed: %s", SDL_GetError());
			}

			m_running = true;
		}

		void 
		_luna_display::stop(void)
		{

			if(!m_initialized) {
				THROW_LUNA_DISPLAY_EXCEPTION(LUNA_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			if(!m_running) {
				THROW_LUNA_DISPLAY_EXCEPTION(LUNA_DISPLAY_EXCEPTION_STOPPED);
			}

			m_running = false;

			if(m_window) {
				SDL_DestroyWindow(m_window);
				m_window = NULL;
			}

			clear();
		}

		std::string 
		_luna_display::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			result << LUNA_DISPLAY_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT")
				<< ", " << (m_running ? "STARTED" : "STOPPED");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(luna_display_ptr, this);
			}

			result << ")";

			if(m_initialized && m_running) {
				result << std::endl << "--- " << m_config.to_string(verbose);
			}

			return result.str();
		}

		void 
		_luna_display::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_LUNA_DISPLAY_EXCEPTION(LUNA_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			if(m_running) {
				stop();
			}

			m_initialized = false;
		}
	}
}
