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
#include "../include/luna_vertex_type.h"

namespace LUNA {

	namespace COMP {

		_luna_vertex *_luna_vertex::m_instance = NULL;

		_luna_vertex::_luna_vertex(void) :
			m_initialized(false)
		{
			std::atexit(luna_vertex::_delete);
		}

		_luna_vertex::~_luna_vertex(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_luna_vertex::_delete(void)
		{

			if(luna_vertex::m_instance) {
				delete luna_vertex::m_instance;
				luna_vertex::m_instance = NULL;
			}
		}

		_luna_vertex *
		_luna_vertex::acquire(void)
		{

			if(!luna_vertex::m_instance) {

				luna_vertex::m_instance = new luna_vertex;
				if(!luna_vertex::m_instance) {
					THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_ALLOCATED);
				}
			}

			return luna_vertex::m_instance;
		}

		GLuint 
		_luna_vertex::add_buffer(
			__in GLenum target,
			__in size_t count
			)
		{
			GLuint result = 0;

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			glGenBuffers(count, &result);
			glBindBuffer(target, result);
			m_buffer_map.insert(std::pair<GLuint, std::pair<size_t, size_t>>(result, 
				std::pair<size_t, size_t>(count, REFERENCE_INIT)));

			return result;
		}

		GLuint 
		_luna_vertex::add_vertex(
			__in size_t count
			)
		{
			GLuint result = 0;

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			glGenVertexArrays(count, &result);
			glBindVertexArray(result);
			m_vertex_map.insert(std::pair<GLuint, std::pair<size_t, size_t>>(result, 
				std::pair<size_t, size_t>(count, REFERENCE_INIT)));

			return result;
		}

		void 
		_luna_vertex::bind_buffer(
			__in GLenum target,
			__in_opt GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			glBindBuffer(target, !id ? id : find_buffer(id)->first);
		}

		void 
		_luna_vertex::bind_vertex(
			__in_opt GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			glBindVertexArray(!id ? id : find_vertex(id)->first);
		}

		size_t 
		_luna_vertex::buffer_count(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			return find_buffer(id)->second.first;
		}

		void 
		_luna_vertex::clear(void)
		{
			std::map<GLuint, std::pair<size_t, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			for(iter = m_buffer_map.begin(); iter != m_buffer_map.end(); ++iter) {
				glDeleteBuffers(iter->second.first, &iter->first);
			}

			m_buffer_map.clear();

			for(iter = m_vertex_map.begin(); iter != m_vertex_map.end(); ++iter) {
				glDeleteVertexArrays(iter->second.first, &iter->first);
			}

			m_vertex_map.clear();
		}

		bool 
		_luna_vertex::contains_buffer(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			return (m_buffer_map.find(id) != m_buffer_map.end());
		}

		bool 
		_luna_vertex::contains_vertex(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			return (m_vertex_map.find(id) != m_vertex_map.end());
		}

		size_t 
		_luna_vertex::decrement_buffer(
			__in GLuint id
			)
		{
			size_t result = 0;
			std::map<GLuint, std::pair<size_t, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			iter = find_buffer(id);
			if(iter->second.second > REFERENCE_INIT) {
				result = --iter->second.second;
			} else {
				glDeleteBuffers(iter->second.first, &iter->first);
				m_buffer_map.erase(iter);
			}

			return result;
		}

		size_t 
		_luna_vertex::decrement_vertex(
			__in GLuint id
			)
		{
			size_t result = 0;
			std::map<GLuint, std::pair<size_t, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			iter = find_vertex(id);
			if(iter->second.second > REFERENCE_INIT) {
				result = --iter->second.second;
			} else {
				glDeleteVertexArrays(iter->second.first, &iter->first);
				m_vertex_map.erase(iter);
			}

			return result;
		}

		std::map<GLuint, std::pair<size_t, size_t>>::iterator 
		_luna_vertex::find_buffer(
			__in GLuint id
			)
		{
			std::map<GLuint, std::pair<size_t, size_t>>::iterator result;

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			result = m_buffer_map.find(id);
			if(result == m_buffer_map.end()) {
				THROW_LUNA_VERTEX_EXCEPTION_FORMAT(LUNA_VERTEX_EXCEPTION_BUFFER_NOT_FOUND,
					"0x%x", id);
			}

			return result;
		}

		std::map<GLuint, std::pair<size_t, size_t>>::iterator 
		_luna_vertex::find_vertex(
			__in GLuint id
			)
		{
			std::map<GLuint, std::pair<size_t, size_t>>::iterator result;

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			result = m_vertex_map.find(id);
			if(result == m_buffer_map.end()) {
				THROW_LUNA_VERTEX_EXCEPTION_FORMAT(LUNA_VERTEX_EXCEPTION_VERTEX_NOT_FOUND,
					"0x%x", id);
			}

			return result;
		}

		size_t 
		_luna_vertex::increment_buffer(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			return ++find_buffer(id)->second.second;
		}

		size_t 
		_luna_vertex::increment_vertex(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			return ++find_vertex(id)->second.second;
		}

		void 
		_luna_vertex::initialize(void)
		{

			if(m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_INITIALIZED);
			}

			m_initialized = true;
			clear();
		}

		bool 
		_luna_vertex::is_allocated(void)
		{
			return (luna_vertex::m_instance != NULL);
		}

		bool 
		_luna_vertex::is_initialized(void)
		{
			return m_initialized;
		}

		size_t 
		_luna_vertex::reference_count_buffer(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			return find_buffer(id)->second.second;
		}

		size_t 
		_luna_vertex::reference_count_vertex(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			return find_vertex(id)->second.second;
		}

		void 
		_luna_vertex::remove_buffer(
			__in GLuint id
			)
		{
			std::map<GLuint, std::pair<size_t, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			iter = find_buffer(id);
			glDeleteBuffers(iter->second.first, &iter->first);
			m_buffer_map.erase(iter);
		}

		void 
		_luna_vertex::remove_vertex(
			__in GLuint id
			)
		{
			std::map<GLuint, std::pair<size_t, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			iter = find_vertex(id);
			glDeleteVertexArrays(iter->second.first, &iter->first);
			m_vertex_map.erase(iter);
		}

		void 
		_luna_vertex::set_buffer_data(
			__in GLenum target,
			__in const void *data,
			__in size_t length,
			__in GLenum usage
			)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			glBufferData(target, length, data, usage);
		}

		size_t 
		_luna_vertex::size(void)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			return (m_buffer_map.size() + m_vertex_map.size());
		}

		std::string 
		_luna_vertex::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<GLuint, std::pair<size_t, size_t>>::iterator iter;

			result << LUNA_VERTEX_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(luna_vertex_ptr, this);
			}

			result << ")";

			if(m_initialized) {

				if(!m_buffer_map.empty()) {
					result << std::endl << "--- BUFFER:";

					for(iter = m_buffer_map.begin(); iter != m_buffer_map.end(); ++iter) {
						result << std::endl << "------ 0x" << SCALAR_AS_HEX(GLuint, iter->first)
							<< ", COUNT: " << iter->second.first << ", REF. " 
							<< iter->second.second;
					}
				}

				if(!m_vertex_map.empty()) {
					result << std::endl << "--- VERTEX:";

					for(iter = m_vertex_map.begin(); iter != m_vertex_map.end(); ++iter) {
						result << std::endl << "------ 0x" << SCALAR_AS_HEX(GLuint, iter->first)
							<< ", COUNT: " << iter->second.first << ", REF. " 
							<< iter->second.second;
					}
				}
			}

			return result.str();
		}

		void 
		_luna_vertex::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			clear();
			m_initialized = false;
		}

		size_t 
		_luna_vertex::vertex_count(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_VERTEX_EXCEPTION(LUNA_VERTEX_EXCEPTION_UNINITIALIZED);
			}

			return find_vertex(id)->second.first;
		}
	}
}
