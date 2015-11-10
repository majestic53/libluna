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

#ifndef LUNA_SHADER_H_
#define LUNA_SHADER_H_

namespace LUNA {

	namespace COMP {

		typedef class _luna_shader {

			public:

				~_luna_shader(void);

				static _luna_shader *acquire(void);

				GLuint add(
					__in const std::string &input,
					__in bool is_file,
					__in GLenum type
					);

				void clear(void);

				bool contains(
					__in GLuint id
					);

				size_t decrement_reference(
					__in GLuint id
					);

				size_t increment_reference(
					__in GLuint id
					);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				size_t reference_count(
					__in GLuint id
					);

				void remove(
					__in GLuint id
					);

				size_t size(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				GLenum type(
					__in GLuint id
					);

				void uninitialize(void);

			protected:

				_luna_shader(void);

				_luna_shader(
					__in const _luna_shader &other
					);

				_luna_shader &operator=(
					__in const _luna_shader &other
					);

				static void _delete(void);

				std::map<GLuint, std::pair<GLenum, size_t>>::iterator find(
					__in GLuint id
					);

				bool m_initialized;

				static _luna_shader *m_instance;

				std::map<GLuint, std::pair<GLenum, size_t>> m_shader_map;

		} luna_shader, *luna_shader_ptr;

		typedef class _luna_shader_program {

			public:

				~_luna_shader_program(void);

				static _luna_shader_program *acquire(void);

				GLuint add(
					__in const std::vector<GLuint> &shaders
					);

				GLint attribute(
					__in GLuint id,
					__in const std::string &name
					);

				void clear(void);

				bool contains(
					__in GLuint id
					);

				size_t decrement_reference(
					__in GLuint id
					);

				size_t increment_reference(
					__in GLuint id
					);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				size_t reference_count(
					__in GLuint id
					);

				void remove(
					__in GLuint id
					);

				size_t size(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				GLint uniform(
					__in GLuint id,
					__in const std::string &name
					);

				void uninitialize(void);

				void use(
					__in_opt GLuint id = 0
					);

			protected:

				_luna_shader_program(void);

				_luna_shader_program(
					__in const _luna_shader_program &other
					);

				_luna_shader_program &operator=(
					__in const _luna_shader_program &other
					);

				static void _delete(void);

				size_t decrement_shader_reference(
					__in GLuint id
					);

				size_t increment_shader_reference(
					__in GLuint id
					);

				std::map<GLuint, std::pair<std::vector<GLuint>, size_t>>::iterator find(
					__in GLuint id
					);

				bool m_initialized;

				static _luna_shader_program *m_instance;

				std::map<GLuint, std::pair<std::vector<GLuint>, size_t>> m_shader_program_map;

		} luna_shader_program, *luna_shader_program_ptr;
	}
}

#endif // LUNA_SHADER_H_
