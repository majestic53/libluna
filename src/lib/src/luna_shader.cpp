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

			m_shader_map.insert(std::pair<GLuint, std::pair<GLenum, size_t>>(result, 
				std::pair<GLenum, size_t>(type, REFERENCE_INIT)));

			return result;
		}

		void 
		_luna_shader::clear(void)
		{
			std::map<GLuint, std::pair<GLenum, size_t>>::iterator iter;

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

		size_t 
		_luna_shader::decrement_reference(
			__in GLuint id
			)
		{
			size_t result = 0;
			std::map<GLuint, std::pair<GLenum, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			iter = find(id);
			if(iter->second.second > REFERENCE_INIT) {
				result = --iter->second.second;
			} else {
				glDeleteShader(iter->first);
				m_shader_map.erase(iter);
			}

			return result;
		}

		std::map<GLuint, std::pair<GLenum, size_t>>::iterator 
		_luna_shader::find(
			__in GLuint id
			)
		{
			std::map<GLuint, std::pair<GLenum, size_t>>::iterator result;

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

		size_t 
		_luna_shader::increment_reference(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			return ++find(id)->second.second;
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

		size_t 
		_luna_shader::reference_count(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			return find(id)->second.second;
		}

		void 
		_luna_shader::remove(
			__in GLuint id
			)
		{
			std::map<GLuint, std::pair<GLenum, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			iter = find(id);
			glDeleteShader(iter->first);
			m_shader_map.erase(iter);
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
			std::map<GLuint, std::pair<GLenum, size_t>>::iterator iter;

			result << LUNA_SHADER_HEADER << " (" << (m_initialized ? "INIT" : "UNINIT");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(luna_shader_ptr, this);
			}

			result << ")";

			if(m_initialized) {

				for(iter = m_shader_map.begin(); iter != m_shader_map.end(); ++iter) {
					result << std::endl << "--- 0x" << SCALAR_AS_HEX(GLuint, iter->first)
						<< ", 0x" << SCALAR_AS_HEX(GLenum, iter->second.first)
						<< ", REF. " << iter->second.second;
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

			return find(id)->second.first;
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

		_luna_shader_program *_luna_shader_program::m_instance = NULL;

		_luna_shader_program::_luna_shader_program(void) :
			m_initialized(false)
		{
			std::atexit(luna_shader_program::_delete);
		}

		_luna_shader_program::~_luna_shader_program(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_luna_shader_program::_delete(void)
		{

			if(luna_shader_program::m_instance) {
				delete luna_shader_program::m_instance;
				luna_shader_program::m_instance = NULL;
			}
		}

		_luna_shader_program *
		_luna_shader_program::acquire(void)
		{

			if(!luna_shader_program::m_instance) {

				luna_shader_program::m_instance = new luna_shader_program;
				if(!luna_shader_program::m_instance) {
					THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_ALLOCATED);
				}
			}

			return luna_shader_program::m_instance;
		}

		GLuint 
		_luna_shader_program::add(
			__in const std::vector<GLuint> &shaders
			)
		{
			std::string err;
			GLuint result = 0;
			GLint err_length, status;
			std::vector<GLuint>::const_iterator iter;

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			result = glCreateProgram();
			if(!result) {
				THROW_LUNA_SHADER_EXCEPTION_FORMAT(LUNA_SHADER_EXCEPTION_EXTERNAL,
					"%s", "glCreateProgram failed");
			}

			for(iter = shaders.begin(); iter != shaders.end(); ++iter) {
				glAttachShader(result, *iter);
			}

			glLinkProgram(result);

			for(iter = shaders.begin(); iter != shaders.end(); ++iter) {
				glDetachShader(result, *iter);
			}

			glGetProgramiv(result, GL_LINK_STATUS, &status);
			if(status == GL_FALSE) {
				glGetProgramiv(result, GL_INFO_LOG_LENGTH, &err_length);
				err.resize(err_length + 1);
				glGetProgramInfoLog(result, err_length, NULL, (char *) &err[0]);
				glDeleteProgram(result);
				THROW_LUNA_SHADER_EXCEPTION_FORMAT(LUNA_SHADER_EXCEPTION_EXTERNAL,
					"glGetProgramiv failed: %s", err.c_str());
			}

			for(iter = shaders.begin(); iter != shaders.end(); ++iter) {
				increment_shader_reference(*iter);
			}

			m_shader_program_map.insert(std::pair<GLuint, std::pair<std::vector<GLuint>, size_t>>(
				result, std::pair<std::vector<GLuint>, size_t>(shaders, REFERENCE_INIT)));

			return result;
		}

		GLint 
		_luna_shader_program::attribute(
			__in GLuint id,
			__in const std::string &name
			)
		{
			GLint result = SCALAR_INVALID(GLint);

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			result = glGetAttribLocation(find(id)->first, (GLchar *) &name[0]);
			if(result == SCALAR_INVALID(GLint)) {
				THROW_LUNA_SHADER_EXCEPTION_FORMAT(LUNA_SHADER_EXCEPTION_EXTERNAL,
					"glGetAttribLocation failed: Attribute %s", 
					STRING_CHECK(name));
			}

			return result;
		}

		void 
		_luna_shader_program::clear(void)
		{
			std::vector<GLuint>::iterator shader_iter;
			std::map<GLuint, std::pair<std::vector<GLuint>, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			for(iter = m_shader_program_map.begin(); iter != m_shader_program_map.end(); 
					++iter) {

				for(shader_iter = iter->second.first.begin(); 
						shader_iter != iter->second.first.end();
						++shader_iter) {
					decrement_shader_reference(*shader_iter);
				}

				glDeleteProgram(iter->first);
			}

			m_shader_program_map.clear();
		}

		bool 
		_luna_shader_program::contains(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			return (m_shader_program_map.find(id) != m_shader_program_map.end());
		}

		size_t 
		_luna_shader_program::decrement_reference(
			__in GLuint id
			)
		{
			size_t result = 0;
			std::vector<GLuint>::iterator shader_iter;
			std::map<GLuint, std::pair<std::vector<GLuint>, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			iter = find(id);
			if(iter->second.second > REFERENCE_INIT) {
				result = --iter->second.second;
			} else {

				for(shader_iter = iter->second.first.begin(); 
						shader_iter != iter->second.first.end();
						++shader_iter) {
					decrement_shader_reference(*shader_iter);
				}

				glDeleteProgram(iter->first);
				m_shader_program_map.erase(iter);
			}

			return result;
		}

		size_t 
		_luna_shader_program::decrement_shader_reference(
			__in GLuint id
			)
		{
			size_t result = 0;
			luna_shader_ptr inst = NULL;

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			if(luna_shader::is_allocated()) {

				inst = luna_shader::acquire();
				if(inst && inst->is_initialized()
						&& inst->contains(id)) {
					result = inst->decrement_reference(id);
				}
			}

			return result;
		}

		std::map<GLuint, std::pair<std::vector<GLuint>, size_t>>::iterator 
		_luna_shader_program::find(
			__in GLuint id
			)
		{
			std::map<GLuint, std::pair<std::vector<GLuint>, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			iter = m_shader_program_map.find(id);
			if(iter == m_shader_program_map.end()) {
				THROW_LUNA_SHADER_EXCEPTION_FORMAT(LUNA_SHADER_EXCEPTION_NOT_FOUND,
					"0x%x", id);
			}

			return iter;
		}

		size_t 
		_luna_shader_program::increment_reference(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			return ++find(id)->second.second;
		}

		size_t 
		_luna_shader_program::increment_shader_reference(
			__in GLuint id
			)
		{
			size_t result = 0;
			luna_shader_ptr inst = NULL;

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			if(luna_shader::is_allocated()) {

				inst = luna_shader::acquire();
				if(inst && inst->is_initialized()
						&& inst->contains(id)) {
					result = inst->increment_reference(id);
				}
			}

			return result;
		}

		void 
		_luna_shader_program::initialize(void)
		{

			if(m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_INITIALIZED);
			}

			m_initialized = true;
			clear();
		}

		bool 
		_luna_shader_program::is_allocated(void)
		{
			return (luna_shader_program::m_instance != NULL);
		}

		bool 
		_luna_shader_program::is_initialized(void)
		{
			return m_initialized;
		}

		size_t 
		_luna_shader_program::reference_count(
			__in GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			return find(id)->second.second;
		}

		void 
		_luna_shader_program::remove(
			__in GLuint id
			)
		{
			std::vector<GLuint>::iterator shader_iter;
			std::map<GLuint, std::pair<std::vector<GLuint>, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			iter = find(id);

			for(shader_iter = iter->second.first.begin(); 
					shader_iter != iter->second.first.end();
					++shader_iter) {
				decrement_shader_reference(*shader_iter);
			}

			glDeleteProgram(iter->first);
			m_shader_program_map.erase(iter);
		}

		size_t 
		_luna_shader_program::size(void)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			return m_shader_program_map.size();
		}

		std::string 
		_luna_shader_program::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::vector<GLuint>::iterator shader_iter;
			std::map<GLuint, std::pair<std::vector<GLuint>, size_t>>::iterator iter;

			result << LUNA_SHADER_PROGRAM_HEADER << " (" 
				<< (m_initialized ? "INIT" : "UNINIT");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(luna_shader_program_ptr, this);
			}

			result << ")";

			if(m_initialized) {

				for(iter = m_shader_program_map.begin(); 
						iter != m_shader_program_map.end(); 
						++iter) {
					result << std::endl << "--- 0x" << SCALAR_AS_HEX(GLuint, iter->first) 
						<< ", REF. " << iter->second.second;

					for(shader_iter = iter->second.first.begin(); 
							shader_iter != iter->second.first.end();
							++shader_iter) {
						result << std::endl << "------ 0x" 
							<< SCALAR_AS_HEX(GLuint, *shader_iter);
					}
				}
			}

			return result.str();
		}

		GLint 
		_luna_shader_program::uniform(
			__in GLuint id,
			__in const std::string &name
			)
		{
			GLint result = SCALAR_INVALID(GLint);

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			result = glGetUniformLocation(find(id)->first, (GLchar *) &name[0]);
			if(result == SCALAR_INVALID(GLint)) {
				THROW_LUNA_SHADER_EXCEPTION_FORMAT(LUNA_SHADER_EXCEPTION_EXTERNAL,
					"glGetUniformLocation failed: Uniform %s", 
					STRING_CHECK(name));
			}

			return result;
		}

		void 
		_luna_shader_program::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			clear();
			m_initialized = false;
		}

		void 
		_luna_shader_program::use(
			__in_opt GLuint id
			)
		{

			if(!m_initialized) {
				THROW_LUNA_SHADER_EXCEPTION(LUNA_SHADER_EXCEPTION_UNINITIALIZED);
			}

			glUseProgram(!id ? id : find(id)->first);
		}
	}
}
