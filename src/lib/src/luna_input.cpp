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
#include "../include/luna_input_type.h"

namespace LUNA {

	namespace COMP {

		_luna_input_config::_luna_input_config(void)
		{
			return;
		}

		_luna_input_config::_luna_input_config(
			__in const _luna_input_config &other
			) :
				m_config(other.m_config)
		{
			return;
		}

		_luna_input_config::~_luna_input_config(void)
		{
			return;
		}

		_luna_input_config &
		_luna_input_config::operator=(
			__in const _luna_input_config &other
			)
		{

			if(this != &other) {
				m_config = other.m_config;
			}

			return *this;
		}

		void 
		_luna_input_config::add(
			__in const SDL_EventType &type,
			__in luna_input_cb callback,
			__in_opt void *context
			)
		{
			std::map<SDL_EventType, std::pair<luna_input_cb, void *>>::iterator iter;

			if(!callback) {
				THROW_LUNA_INPUT_EXCEPTION_FORMAT(LUNA_INPUT_EXCEPTION_INVALID,
					"0x%x", type);
			}

			iter = m_config.find(type);
			if(iter == m_config.end()) {
				m_config.insert(std::pair<SDL_EventType, std::pair<luna_input_cb, void *>>(
					type, std::pair<luna_input_cb, void *>(callback, context)));
			} else {
				iter->second.first = callback;
				iter->second.second = context;
			}
		}

		void 
		_luna_input_config::clear(void)
		{
			m_config.clear();
		}

		bool 
		_luna_input_config::contains(
			__in SDL_EventType type
			)
		{
			return (m_config.find(type) != m_config.end());
		}

		std::map<SDL_EventType, std::pair<luna_input_cb, void *>>::iterator 
		_luna_input_config::find(
			__in const SDL_EventType &type
			)
		{
			std::map<SDL_EventType, std::pair<luna_input_cb, void *>>::iterator result;

			result = m_config.find(type);
			if(result == m_config.end()) {
				THROW_LUNA_INPUT_EXCEPTION_FORMAT(LUNA_INPUT_EXCEPTION_NOT_FOUND,
					"0x%x", type);
			}

			return result;
		}

		luna_input_evt 
		_luna_input_config::handle(
			__in const SDL_Event &event
			)
		{
			luna_input_evt result = LUNA_INPUT_EVT_NONE;
			std::map<SDL_EventType, std::pair<luna_input_cb, void *>>::iterator iter;

			iter = m_config.find((SDL_EventType) event.type);
			if(iter != m_config.end()) {
				result = iter->second.first(event, iter->second.second);
			} else {
				result = LUNA_INPUT_EVT_UNSUPPORTED;	
			}

			return result;
		}

		void 
		_luna_input_config::remove(
			__in const SDL_EventType &type
			)
		{
			m_config.erase(find(type));
		}

		size_t 
		_luna_input_config::size(void)
		{
			return m_config.size();
		}

		std::string 
		_luna_input_config::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<SDL_EventType, std::pair<luna_input_cb, void *>>::iterator iter;

			UNREFERENCE_PARAM(verbose);

			for(iter = m_config.begin(); iter != m_config.end(); ++iter) {

				if(iter != m_config.begin()) {
					result << std::endl;
				}

				result << "--- (0x" << SCALAR_AS_HEX(SDL_EventType, iter->first)
					<< ") 0x" << SCALAR_AS_HEX(luna_input_cb, iter->second.first)
					<< ", 0x" << SCALAR_AS_HEX(void *, iter->second.second);
			}

			return result.str();
		}

		_luna_input *_luna_input::m_instance = NULL;

		_luna_input::_luna_input(void) :
			m_initialized(false)
		{
			std::atexit(luna_input::_delete);
		}

		_luna_input::~_luna_input(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_luna_input::_delete(void)
		{

			if(luna_input::m_instance) {
				delete luna_input::m_instance;
				luna_input::m_instance = NULL;
			}
		}

		_luna_input *
		_luna_input::acquire(void)
		{

			if(!luna_input::m_instance) {

				luna_input::m_instance = new luna_input;
				if(!luna_input::m_instance) {
					THROW_LUNA_INPUT_EXCEPTION(LUNA_INPUT_EXCEPTION_ALLOCATED);
				}
			}

			return luna_input::m_instance;
		}

		void 
		_luna_input::add(
			__in const SDL_EventType &type,
			__in luna_input_cb callback,
			__in_opt void *context
			)
		{
			if(!m_initialized) {
				THROW_LUNA_INPUT_EXCEPTION(LUNA_INPUT_EXCEPTION_UNINITIALIZED);
			}

			m_config.add(type, callback, context);
		}

		void 
		_luna_input::clear(void)
		{

			if(!m_initialized) {
				THROW_LUNA_INPUT_EXCEPTION(LUNA_INPUT_EXCEPTION_UNINITIALIZED);
			}

			m_config.clear();
		}

		bool 
		_luna_input::contains(
			__in SDL_EventType type
			)
		{

			if(!m_initialized) {
				THROW_LUNA_INPUT_EXCEPTION(LUNA_INPUT_EXCEPTION_UNINITIALIZED);
			}

			return m_config.contains(type);
		}

		luna_input_evt 
		_luna_input::handle(
			__in const SDL_Event &event
			)
		{

			if(!m_initialized) {
				THROW_LUNA_INPUT_EXCEPTION(LUNA_INPUT_EXCEPTION_UNINITIALIZED);
			}

			return m_config.handle(event);
		}

		void 
		_luna_input::initialize(void)
		{

			if(m_initialized) {
				THROW_LUNA_INPUT_EXCEPTION(LUNA_INPUT_EXCEPTION_INITIALIZED);
			}

			m_initialized = true;
			clear();
		}

		bool 
		_luna_input::is_allocated(void)
		{
			return (luna_input::m_instance != NULL);
		}

		bool 
		_luna_input::is_initialized(void)
		{
			return m_initialized;
		}

		void 
		_luna_input::remove(
			__in const SDL_EventType &type
			)
		{

			if(!m_initialized) {
				THROW_LUNA_INPUT_EXCEPTION(LUNA_INPUT_EXCEPTION_UNINITIALIZED);
			}

			m_config.remove(type);
		}

		void 
		_luna_input::set(
			__in const luna_input_config &config
			)
		{

			if(!m_initialized) {
				THROW_LUNA_INPUT_EXCEPTION(LUNA_INPUT_EXCEPTION_UNINITIALIZED);
			}

			m_config = config;
		}

		size_t 
		_luna_input::size(void)
		{
			
			if(!m_initialized) {
				THROW_LUNA_INPUT_EXCEPTION(LUNA_INPUT_EXCEPTION_UNINITIALIZED);
			}

			return m_config.size();
		}

		std::string 
		_luna_input::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			result << LUNA_INPUT_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(luna_input_ptr, this);
			}

			result << ")";

			if(m_initialized) {
				result << std::endl << m_config.to_string(verbose);
			}

			return result.str();
		}

		void 
		_luna_input::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_LUNA_INPUT_EXCEPTION(LUNA_INPUT_EXCEPTION_UNINITIALIZED);
			}

			clear();
			m_initialized = false;
		}
	}
}
