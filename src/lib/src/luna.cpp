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
		"SETUP", "START", "STOP", "TEARDOWN",
		};

	#define LUNA_EVT_MAX LUNA_EVT_TEARDOWN

	#define LUNA_EVT_STRING(_TYPE_) \
		((_TYPE_) > LUNA_EVT_MAX ? "USER" : \
		STRING_CHECK(LUNA_EVT_STR[_TYPE_]))

	_luna_draw_config::_luna_draw_config(
		__in_opt luna_draw_cb callback,
		__in_opt void *context
		) :
			m_callback(callback),
			m_context(context)
	{
		return;
	}

	_luna_draw_config::_luna_draw_config(
		__in const _luna_draw_config &other
		) :
			m_callback(other.m_callback),
			m_context(other.m_context)
	{
		return;
	}

	_luna_draw_config::~_luna_draw_config(void)
	{
		return;
	}

	_luna_draw_config &
	_luna_draw_config::operator=(
		__in const _luna_draw_config &other
		)
	{

		if(this != &other) {
			m_callback = other.m_callback;
			m_context = other.m_context;
		}

		return *this;
	}

	void 
	_luna_draw_config::clear(void)
	{
		m_callback = NULL;
		m_context = NULL;
	}

	void 
	_luna_draw_config::invoke(
		__in SDL_Window *window,
		__in SDL_GLContext screen
		)
	{

		if(m_callback) {

			if(!LUNA_SUCCESS(m_callback(window, screen, m_context))) {
				THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_DRAW);
			}

			SDL_GL_SwapWindow(window);
		}
	}

	void 
	_luna_draw_config::set(
		__in luna_draw_cb callback,
		__in_opt void *context
		)
	{

		if(!callback) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_INVALID);
		}

		m_callback = callback;
		m_context = context;
	}

	std::string 
	_luna_draw_config::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		UNREFERENCE_PARAM(verbose);

		result << "--- DRAW: 0x" << SCALAR_AS_HEX(luna_draw_cb, m_callback) 
			<< ", 0x" << SCALAR_AS_HEX(void *, m_context);

		return result.str();
	}

	_luna_event_config::_luna_event_config(void)
	{
		return;
	}

	_luna_event_config::_luna_event_config(
		__in const _luna_event_config &other
		) :
			m_config(other.m_config)
	{
		return;
	}

	_luna_event_config::~_luna_event_config(void)
	{
		return;
	}

	_luna_event_config &
	_luna_event_config::operator=(
		__in const _luna_event_config &other
		)
	{

		if(this != &other) {
			m_config = other.m_config;
		}

		return *this;
	}

	void 
	_luna_event_config::add(
		__in uint32_t type,
		__in luna_evt_cb callback,
		__in_opt void *context
		)
	{
		std::map<uint32_t, std::pair<luna_evt_cb, void *>>::iterator iter;

		if(!callback) {
			THROW_LUNA_EXCEPTION_FORMAT(LUNA_EXCEPTION_INVALID,
				"0x%x", type);
		}

		iter = m_config.find(type);
		if(iter == m_config.end()) {
			m_config.insert(std::pair<uint32_t, std::pair<luna_evt_cb, void *>>(
				type, std::pair<luna_evt_cb, void *>(callback, context)));
		} else {
			iter->second.first = callback;
			iter->second.second = context;
		}
	}

	void 
	_luna_event_config::clear(void)
	{
		m_config.clear();
	}

	bool 
	_luna_event_config::contains(
		__in uint32_t type
		)
	{
		return (m_config.find(type) != m_config.end());
	}

	std::map<uint32_t, std::pair<luna_evt_cb, void *>>::iterator 
	_luna_event_config::find(
		__in uint32_t type
		)
	{
		std::map<uint32_t, std::pair<luna_evt_cb, void *>>::iterator result;

		result = m_config.find(type);
		if(result == m_config.end()) {
			THROW_LUNA_EXCEPTION_FORMAT(LUNA_EXCEPTION_NOT_FOUND,
				"0x%x", type);
		}

		return result;
	}

	void 
	_luna_event_config::invoke(
		__in uint32_t type
		)
	{
		std::map<uint32_t, std::pair<luna_evt_cb, void *>>::iterator iter;

		iter = m_config.find(type);
		if((iter != m_config.end()) && iter->second.first) {

			if(!LUNA_SUCCESS(iter->second.first(iter->second.second))) {
				THROW_LUNA_EXCEPTION_FORMAT(LUNA_EXCEPTION_EVENT,
					"%s (0x%x)", LUNA_EVT_STRING(type), type);
			}
		}
	}

	void 
	_luna_event_config::remove(
		__in uint32_t type
		)
	{
		m_config.erase(find(type));
	}

	size_t 
	_luna_event_config::size(void)
	{
		return m_config.size();
	}

	std::string 
	_luna_event_config::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;
		std::map<uint32_t, std::pair<luna_evt_cb, void *>>::iterator iter;

		UNREFERENCE_PARAM(verbose);

		for(iter = m_config.begin(); iter != m_config.end(); ++iter) {

			if(iter != m_config.begin()) {
				result << std::endl;
			}

			result << "------ " << LUNA_EVT_STRING(iter->first) << ": 0x" 
				<< SCALAR_AS_HEX(luna_evt_cb, iter->second.first) << ", 0x" 
				<< SCALAR_AS_HEX(void *, iter->second.second);
		}

		return result.str();
	}

	_luna_tick_config::_luna_tick_config(
		__in_opt luna_tick_cb callback,
		__in_opt void *context
		) :
			m_callback(callback),
			m_context(context)
	{
		return;
	}

	_luna_tick_config::_luna_tick_config(
		__in const _luna_tick_config &other
		) :
			m_callback(other.m_callback),
			m_context(other.m_context)
	{
		return;
	}

	_luna_tick_config::~_luna_tick_config(void)
	{
		return;
	}

	_luna_tick_config &
	_luna_tick_config::operator=(
		__in const _luna_tick_config &other
		)
	{

		if(this != &other) {
			m_callback = other.m_callback;
			m_context = other.m_context;
		}

		return *this;
	}

	void 
	_luna_tick_config::clear(void)
	{
		m_callback = NULL;
		m_context = NULL;
	}

	void 
	_luna_tick_config::invoke(
		__in SDL_Window *window,
		__in SDL_GLContext screen,
		__in uint32_t tick
		)
	{

		if(m_callback) {

			if(!LUNA_SUCCESS(m_callback(window, screen, tick, m_context))) {
				THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_TICK);
			}
		}
	}

	void 
	_luna_tick_config::set(
		__in luna_tick_cb callback,
		__in_opt void *context
		)
	{

		if(!callback) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_INVALID);
		}

		m_callback = callback;
		m_context = context;
	}

	std::string 
	_luna_tick_config::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		UNREFERENCE_PARAM(verbose);

		result << "--- TICK: 0x" << SCALAR_AS_HEX(luna_tick_cb, m_callback) 
			<< ", 0x" << SCALAR_AS_HEX(void *, m_context);

		return result.str();
	}

	bool _luna::m_initialized_external = false;
	_luna *_luna::m_instance = NULL;

	_luna::_luna(void) :
		m_initialized(false),
		m_instance_display(luna_display::acquire()),
		m_instance_input(luna_input::acquire()),
		m_instance_shader(luna_shader::acquire()),
		m_instance_shader_program(luna_shader_program::acquire()),
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

	luna_shader_ptr 
	_luna::acquire_shader(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_shader;
	}

	luna_shader_program_ptr 
	_luna::acquire_shader_program(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_shader_program;
	}

	void 
	_luna::add_event(
		__in uint32_t type,
		__in luna_evt_cb callback,
		__in_opt void *context
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_event_config.add(type, callback, context);
	}

	GLuint 
	_luna::add_shader(
		__in const std::string &input,
		__in bool is_file,
		__in GLenum type
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_shader->add(input, is_file, type);
	}

	GLuint 
	_luna::add_shader_program(
		__in const std::vector<GLuint> &ids
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_shader_program->add(ids);
	}

	void 
	_luna::clear_draw(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_draw_config.clear();
	}

	void 
	_luna::clear_events(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_event_config.clear();
	}

	void 
	_luna::clear_shaders(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_instance_shader->clear();		
	}

	void 
	_luna::clear_shader_programs(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_instance_shader_program->clear();
	}

	void 
	_luna::clear_tick(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_tick_config.clear();
	}

	bool 
	_luna::contains_event(
		__in uint32_t type
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_event_config.contains(type);
	}

	bool 
	_luna::contains_shader(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_shader->contains(id);
	}

	bool 
	_luna::contains_shader_program(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_shader_program->contains(id);
	}

	size_t 
	_luna::event_count(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_event_config.size();
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

		m_instance_shader->initialize();
		m_instance_shader_program->initialize();
		m_instance_input->initialize();
		m_instance_display->initialize();

		// TODO: initialize components

		m_initialized = true;
	}

	void 
	_luna::invoke_event(
		__in uint32_t type
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_event_config.invoke(type);
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
	_luna::remove_event(
		__in uint32_t type
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_event_config.remove(type);
	}

	void 
	_luna::remove_shader(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_instance_shader->remove(id);
	}

	void 
	_luna::remove_shader_program(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_shader_program->remove(id);
	}

	void 
	_luna::set_draw(
		__in const luna_draw_config &config
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_draw_config = config;
	}

	void 
	_luna::set_events(
		__in const luna_event_config &config
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_event_config = config;
	}

	void 
	_luna::set_tick(
		__in const luna_tick_config &config
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		m_tick_config = config;
	}

	void 
	_luna::setup(
		__in const luna_draw_config &draw_config,
		__in const luna_tick_config &tick_config,
		__in_opt const luna_event_config &event_config,
		__in_opt const luna_display_config &display_config,
		__in_opt const luna_input_config &input_config
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		luna::external_initialize();
		m_instance_shader->clear();
		m_instance_shader_program->clear();
		m_instance_input->set(input_config);
		m_instance_display->start(display_config);

		// TODO: setup components

		set_events(event_config);
		set_draw(draw_config);
		set_tick(tick_config);
		m_event_config.invoke(LUNA_EVT_SETUP);
	}

	size_t 
	_luna::shader_count(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_shader->size();
	}

	GLint 
	_luna::shader_program_attribute(
		__in GLuint id,
		__in const std::string &name
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_shader_program->attribute(id, name);
	}

	size_t 
	_luna::shader_program_count(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_shader_program->size();
	}

	GLint 
	_luna::shader_program_uniform(
		__in GLuint id,
		__in const std::string &name
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_shader_program->uniform(id, name);
	}

	GLenum 
	_luna::shader_type(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_shader->type(id);
	}

	void 
	_luna::start(
		__in const luna_draw_config &draw_config,
		__in const luna_tick_config &tick_config,
		__in_opt const luna_event_config &event_config,
		__in_opt const luna_display_config &display_config,
		__in_opt const luna_input_config &input_config
		)
	{
		uint32_t tick;
		SDL_Event event;
		SDL_Window *window = NULL;
		SDL_GLContext context = NULL;

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_STARTED);
		}
		
		setup(draw_config, tick_config, event_config, display_config, input_config);
		m_tick = 0;
		m_running = true;
		window = m_instance_display->window();
		context = m_instance_display->context();
		m_event_config.invoke(LUNA_EVT_START);

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

			m_tick_config.invoke(window, context, m_tick);

			if((SDL_GetTicks() - tick) < MIN_TICK) {
				SDL_Delay(MIN_TICK - (SDL_GetTicks() - tick));
			}

			m_draw_config.invoke(window, context);
			++m_tick;
		}

		m_running = false;
		context = NULL;
		window = NULL;
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
		
		m_event_config.invoke(LUNA_EVT_STOP);
		m_running = false;
		m_tick = 0;
	}

	void 
	_luna::teardown(void)
	{

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}
		
		clear_tick();
		clear_draw();
		m_event_config.invoke(LUNA_EVT_TEARDOWN);
		clear_events();

		// TODO: teardown components

		m_instance_display->stop();
		m_instance_input->clear();
		m_instance_shader_program->clear();
		m_instance_shader->clear();
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
			result << ", TICK. " << m_tick;

			if(m_event_config.size()) {
				result << std::endl << "--- EVENTS:" << std::endl 
					<< m_event_config.to_string(verbose);
			}
				
			result << std::endl << m_draw_config.to_string(verbose)
				<< std::endl << m_tick_config.to_string(verbose)
				<< std::endl << m_instance_display->to_string(verbose)
				<< std::endl << m_instance_input->to_string(verbose)
				<< std::endl << m_instance_shader->to_string(verbose)
				<< std::endl << m_instance_shader_program->to_string(verbose);

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
		m_instance_shader_program->uninitialize();
		m_instance_shader->uninitialize();
	}

	std::string 
	_luna::version(void)
	{
		return STRING_CAT(VERSION_MAJ) "." STRING_CAT(VERSION_MIN) "."
			STRING_CAT(VERSION_TICK) "." STRING_CAT(VERSION_REV);
	}
}
