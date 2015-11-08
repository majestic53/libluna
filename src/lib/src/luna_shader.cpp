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

#include <fstream>
#include "../include/luna.h"
#include "../include/luna_shader_type.h"

namespace LUNA {

	namespace COMP {

		_luna_shader *_luna_shader::m_instance = NULL;

		_luna_shader::_luna_shader(void) :
			m_initialized(false)
		{
			std::atexit(luna_shader::_delete);
		}

		_luna_shader::~_luna_shader(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_luna_shader::_delete(void)
		{

			if(luna_shader::m_instance) {
				delete luna_shader::m_instance;
				luna_shader::m_instance = NULL;
			}
		}

		_luna_shader *
		_luna_shader::acquire(void)
		{

			if(!luna_shader::m_instance) {

				luna_shader::m_instance = new luna_shader;
				if(!luna_shader::m_instance) {
					THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_ALLOCATED);
				}
			}

			return luna_shader::m_instance;
		}

		GLuint 
		_luna_shader::add(
			__in const std::string &input,
			__in bool is_file,
			__in GLenum type
			)
		{
			int length;
			GLuint result = 0;
			std::string err, source;
			GLint err_length, status;

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			result = glCreateShader(type);
			if(!result) {
				THROW_LUNA_SHADER_EXCEPTION_FORMAT(LUNA_SHADER_EXCEPTION_EXTERNAL,
					"%s", "glCreateShader failed");
			}

			if(is_file) {

				std::ifstream file(input.c_str(), std::ios::in);
				if(!file) {
					THROW_LUNA_SHADER_EXCEPTION_FORMAT(LUNA_SHADER_EXCEPTION_FILE_NOT_FOUND,
						"%s", STRING_CHECK(input));
				}

				file.seekg(0, std::ios::end);
				length = file.tellg();
				file.seekg(0, std::ios::beg);
				source.resize(length);
				file.read((char *) &source[0], length);
				file.close();
			} else {
				source = input;
			}

			const char *source_addr = source.c_str();
			glShaderSource(result, 1, (const GLchar **) &source_addr, NULL);
			glCompileShader(result);

			glGetShaderiv(result, GL_COMPILE_STATUS, &status);
			if(status == GL_FALSE) {
				glGetShaderiv(result, GL_INFO_LOG_LENGTH, &err_length);
				err.resize(err_length + 1);
				glGetShaderInfoLog(result, err_length, NULL, (char *) &err[0]);
				glDeleteShader(result);
				THROW_LUNA_SHADER_EXCEPTION_FORMAT(LUNA_SHADER_EXCEPTION_EXTERNAL,
					"glGetShaderiv failed: %s", err.c_str());
			}

			m_shader_map.insert(std::pair<GLuint, GLenum>(result, type));

			return result;
		}

		void 
		_luna_shader::clear(void)
		{
			std::map<GLuint, GLenum>::iterator iter;

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			for(iter = m_shader_map.begin(); iter != m_shader_map.end(); ++iter) {
				glDeleteShader(iter->first);
			}

			m_shader_map.clear();
		}

		bool 
		_luna_shader::contains(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			return (m_shader_map.find(id) != m_shader_map.end());
		}

		std::map<GLuint, GLenum>::iterator 
		_luna_shader::find(
			__in GLuint id
			)
		{
			std::map<GLuint, GLenum>::iterator result;

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			result = m_shader_map.find(id);
			if(result == m_shader_map.end()) {
				THROW_LUNA_SHADER_EXCEPTION_FORMAT(LUNA_SHADER_EXCEPTION_NOT_FOUND,
					"0x%x", id);
			}

			return result;
		}

		void 
		_luna_shader::initialize(void)
		{

			if(m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_INITIALIZED);
			}

			m_initialized = true;
			clear();
		}

		bool 
		_luna_shader::is_allocated(void)
		{
			return (luna_shader::m_instance != NULL);
		}

		bool 
		_luna_shader::is_initialized(void)
		{
			return m_initialized;
		}

		void 
		_luna_shader::remove(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			glDeleteShader(find(id)->first);
		}

		size_t 
		_luna_shader::size(void)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			return m_shader_map.size();
		}

		std::string 
		_luna_shader::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<GLuint, GLenum>::iterator iter;

			result << LUNA_SHADER_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(luna_shader_ptr, this);
			}

			result << ")";

			if(m_initialized) {

				for(iter = m_shader_map.begin(); iter != m_shader_map.end(); ++iter) {
					result << "\n--- 0x" << SCALAR_AS_HEX(GLuint, iter->first)
						<< ", 0x" << SCALAR_AS_HEX(GLenum, iter->second);
				}
			}

			return result.str();
		}

		GLenum 
		_luna_shader::type(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			return find(id)->second;
		}

		void 
		_luna_shader::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			clear();
			m_initialized = false;
		}
	}
}
