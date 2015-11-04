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
#include "../include/luna_type.h"

namespace LUNA {

	#define MIN_FPS 30
	#define MIN_TICK (MS_PER_SEC / MIN_FPS)

	bool _luna::m_initialized_external = false;
	_luna *_luna::m_instance = NULL;

	_luna::_luna(void) :
		m_initialized(false),
		m_instance_display(luna_display::acquire()),
		m_running(false)
	{
		std::atexit(luna::_delete);
	}

	_luna::~_luna(void)
	{

		if(m_initialized) {
			uninitialize();
		}
	}

	void 
	_luna::_delete(void)
	{
		luna::external_uninitialize();

		if(luna::m_instance) {
			delete luna::m_instance;
			luna::m_instance = NULL;
		}
	}

	_luna *
	_luna::acquire(void)
	{

		if(!luna::m_instance) {

			luna::m_instance = new luna;
			if(!luna::m_instance) {
				THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_ALLOCATED);
			}
		}

		return luna::m_instance;
	}

	luna_display_ptr 
	_luna::acquire_display(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_display;
	}

	void 
	_luna::external_initialize(void)
	{

		if(SDL_Init(SDL_INIT_EVERYTHING)) {
			THROW_LUNA_EXCEPTION_FORMAT(LUNA_EXCEPTION_EXTERNAL,
				"SDL_Init failed: %s", SDL_GetError());
		}

		luna::m_initialized_external = true;
	}

	void 
	_luna::external_uninitialize(void)
	{

		if(luna::m_initialized_external) {
			SDL_Quit();
			luna::m_initialized_external = false;
		}
	}

	void 
	_luna::initialize(void)
	{

		if(m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_INITIALIZED);
		}

		m_instance_display->initialize();

		// TODO: initialize components

		m_initialized = true;
	}

	bool 
	_luna::is_allocated(void)
	{
		return (luna::m_instance != NULL);
	}

	bool 
	_luna::is_initialized(void)
	{
		return m_initialized;
	}

	bool 
	_luna::is_running(void)
	{
		return m_running;
	}

	void 
	_luna::start(
		__in const luna_display_config &config
		)
	{
		uint32_t tick;
		SDL_Event event;

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_STARTED);
		}
		
		setup(config);

		m_running = true;
		while(m_running) {
			tick = SDL_GetTicks();

			while(SDL_PollEvent(&event)) {

				switch(event.type) {
					case SDL_QUIT:
						stop();
						break;

					// TODO: handle input
				}
			}

			// TODO: process
			// TODO: render

			if((SDL_GetTicks() - tick) < MIN_TICK) {
				SDL_Delay(MIN_TICK - (SDL_GetTicks() - tick));
			}
		}

		m_running = false;
		teardown();
	}

	void 
	_luna::setup(
		__in const luna_display_config &config
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		luna::external_initialize();
		m_instance_display->start(config);

		// TODO: setup components
	}

	void 
	_luna::stop(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		if(!m_running) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_STOPPED);
		}

		m_running = false;
	}

	void 
	_luna::teardown(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		// TODO: teardown components

		m_instance_display->stop();
		luna::external_uninitialize();
	}

	std::string 
	_luna::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << LUNA_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT")
			<< ", " << (m_running ? "STARTED" : "STOPPED");

		if(verbose) {
			result << ", PTR. 0x" << SCALAR_AS_HEX(luna_ptr, this);
		}

		result << ")";

		if(m_initialized) {
			result << std::endl << m_instance_display->to_string(verbose);

			// TODO: print components

		}

		return result.str();
	}

	void 
	_luna::uninitialize(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			stop();
		}

		m_initialized = false;

		// TODO: uninitialize components

		m_instance_display->uninitialize();
	}

	std::string 
	_luna::version(void)
	{
		return STRING_CAT(VERSION_MAJ) "." STRING_CAT(VERSION_MIN) "."
			STRING_CAT(VERSION_TICK) "." STRING_CAT(VERSION_REV);
	}
}
