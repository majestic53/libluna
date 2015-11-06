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

	static const std::string LUNA_EVT_STR[] = {
		"DRAW", "SETUP", "START", "STOP", "TEARDOWN", "TICK",
		};

	#define LUNA_EVT_STRING(_TYPE_) \
		((_TYPE_) > LUNA_EVT_MAX ? UNKNOWN : \
		STRING_CHECK(LUNA_EVT_STR[_TYPE_]))

	_luna_config::_luna_config(void)
	{
		return;
	}

	_luna_config::_luna_config(
		__in const _luna_config &other
		) :
			m_config(other.m_config)
	{
		return;
	}

	_luna_config::~_luna_config(void)
	{
		return;
	}

	_luna_config &
	_luna_config::operator=(
		__in const _luna_config &other
		)
	{

		if(this != &other) {
			m_config = other.m_config;
		}

		return *this;
	}

	void 
	_luna_config::add(
		__in luna_evt_t type,
		__in luna_evt_cb callback,
		__in_opt void *context
		)
	{
		std::map<luna_evt_t, std::pair<luna_evt_cb, void *>>::iterator iter;

		if(!callback) {
			THROW_LUNA_EXCEPTION_FORMAT(LUNA_EXCEPTION_INVALID,
				"0x%x", type);
		}

		iter = m_config.find(type);
		if(iter == m_config.end()) {
			m_config.insert(std::pair<luna_evt_t, std::pair<luna_evt_cb, void *>>(
				type, std::pair<luna_evt_cb, void *>(callback, context)));
		} else {
			iter->second.first = callback;
			iter->second.second = context;
		}
	}

	void 
	_luna_config::clear(void)
	{
		m_config.clear();
	}

	bool 
	_luna_config::contains(
		__in luna_evt_t type
		)
	{
		return (m_config.find(type) != m_config.end());
	}

	std::map<luna_evt_t, std::pair<luna_evt_cb, void *>>::iterator 
	_luna_config::find(
		__in luna_evt_t type
		)
	{
		std::map<luna_evt_t, std::pair<luna_evt_cb, void *>>::iterator result;

		result = m_config.find(type);
		if(result == m_config.end()) {
			THROW_LUNA_EXCEPTION_FORMAT(LUNA_EXCEPTION_NOT_FOUND,
				"0x%x", type);
		}

		return result;
	}

	void 
	_luna_config::invoke(
		__in luna_evt_t type
		)
	{
		std::map<luna_evt_t, std::pair<luna_evt_cb, void *>>::iterator iter;

		iter = m_config.find(type);
		if((iter != m_config.end()) && iter->second.first) {
			iter->second.first(iter->second.second);
		}
	}

	void 
	_luna_config::remove(
		__in luna_evt_t type
		)
	{
		m_config.erase(find(type));
	}

	size_t 
	_luna_config::size(void)
	{
		return m_config.size();
	}

	std::string 
	_luna_config::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;
		std::map<luna_evt_t, std::pair<luna_evt_cb, void *>>::iterator iter;

		UNREFERENCE_PARAM(verbose);

		for(iter = m_config.begin(); iter != m_config.end(); ++iter) {

			if(iter != m_config.begin()) {
				result << std::endl;
			}

			result << "--- " << LUNA_EVT_STRING(iter->first) << ": 0x" 
				<< SCALAR_AS_HEX(luna_evt_cb, iter->second.first) << ", 0x" 
				<< SCALAR_AS_HEX(void *, iter->second.second);
		}

		return result.str();
	}

	bool _luna::m_initialized_external = false;
	_luna *_luna::m_instance = NULL;

	_luna::_luna(void) :
		m_initialized(false),
		m_instance_display(luna_display::acquire()),
		m_instance_input(luna_input::acquire()),
		m_running(false),
		m_tick(0)
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

	luna_input_ptr 
	_luna::acquire_input(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_input;
	}

	void 
	_luna::add(
		__in luna_evt_t type,
		__in luna_evt_cb callback,
		__in_opt void *context
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_config.add(type, callback, context);
	}

	void 
	_luna::clear(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_config.clear();
	}

	bool 
	_luna::contains(
		__in luna_evt_t type
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_config.contains(type);
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

		m_instance_input->initialize();
		m_instance_display->initialize();

		// TODO: initialize components

		m_initialized = true;
	}

	void 
	_luna::invoke(
		__in luna_evt_t type
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_config.invoke(type);
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
	_luna::remove(
		__in luna_evt_t type
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_config.remove(type);
	}

	void 
	_luna::set(
		__in const luna_config &config
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_config = config;
	}

	void 
	_luna::setup(
		__in const luna_config &config,
		__in const luna_display_config &display_config,
		__in const luna_input_config &input_config
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		luna::external_initialize();
		m_instance_input->set(input_config);
		m_instance_display->start(display_config);

		// TODO: setup components

		set(config);
		m_config.invoke(LUNA_EVT_SETUP);
	}

	size_t 
	_luna::size(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_config.size();
	}

	void 
	_luna::start(
		__in const luna_config &config,
		__in const luna_display_config &display_config,
		__in const luna_input_config &input_config
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
		
		setup(config, display_config, input_config);
		m_tick = 0;
		m_running = true;
		m_config.invoke(LUNA_EVT_START);

		while(m_running) {
			tick = SDL_GetTicks();

			while(SDL_PollEvent(&event)) {

				switch(event.type) {
					case SDL_QUIT:
						stop();
						break;
					default:

						switch(m_instance_input->handle(event)) {
							case LUNA_INPUT_EVT_QUIT:
								stop();
								break;
							default:
								break;
						}
						break;
				}
			}

			m_config.invoke(LUNA_EVT_TICK);

			if((SDL_GetTicks() - tick) < MIN_TICK) {
				SDL_Delay(MIN_TICK - (SDL_GetTicks() - tick));
			}

			m_config.invoke(LUNA_EVT_DRAW);
			++m_tick;
		}

		m_running = false;
		m_tick = 0;
		teardown();
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
		
		m_config.invoke(LUNA_EVT_STOP);
		m_running = false;
		m_tick = 0;
	}

	void 
	_luna::teardown(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}
		
		m_config.invoke(LUNA_EVT_TEARDOWN);
		clear();

		// TODO: teardown components

		m_instance_display->stop();
		m_instance_input->clear();
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
			result << std::endl << ", TICK. " << m_tick 
				<< std::endl << m_config.to_string(verbose)
				<< std::endl << m_instance_display->to_string(verbose)
				<< std::endl << m_instance_input->to_string(verbose);

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
		m_instance_input->uninitialize();
	}

	std::string 
	_luna::version(void)
	{
		return STRING_CAT(VERSION_MAJ) "." STRING_CAT(VERSION_MIN) "."
			STRING_CAT(VERSION_TICK) "." STRING_CAT(VERSION_REV);
	}
}
