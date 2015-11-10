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

#ifndef LUNA_VERTEX_H_
#define LUNA_VERTEX_H_

namespace LUNA {

	namespace COMP {

		typedef class _luna_vertex {

			public:

				~_luna_vertex(void);

				static _luna_vertex *acquire(void);

				GLuint add_buffer(
					__in GLenum target,
					__in size_t count
					);

				GLuint add_vertex(
					__in size_t count
					);

				void bind_buffer(
					__in GLenum target,
					__in_opt GLuint id = 0
					);

				void bind_vertex(
					__in_opt GLuint id = 0
					);

				size_t buffer_count(
					__in GLuint id
					);

				void clear(void);

				bool contains_buffer(
					__in GLuint id
					);

				bool contains_vertex(
					__in GLuint id
					);

				size_t decrement_buffer(
					__in GLuint id
					);

				size_t decrement_vertex(
					__in GLuint id
					);

				size_t increment_buffer(
					__in GLuint id
					);

				size_t increment_vertex(
					__in GLuint id
					);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				size_t reference_count_buffer(
					__in GLuint id
					);

				size_t reference_count_vertex(
					__in GLuint id
					);

				void remove_buffer(
					__in GLuint id
					);

				void remove_vertex(
					__in GLuint id
					);

				void set_buffer_data(
					__in GLenum target,
					__in const void *data,
					__in size_t length,
					__in GLenum usage
					);

				size_t size(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

				size_t vertex_count(
					__in GLuint id
					);

			protected:

				_luna_vertex(void);

				_luna_vertex(
					__in const _luna_vertex &other
					);

				_luna_vertex &operator=(
					__in const _luna_vertex &other
					);

				static void _delete(void);

				std::map<GLuint, std::pair<size_t, size_t>>::iterator find_buffer(
					__in GLuint id
					);

				std::map<GLuint, std::pair<size_t, size_t>>::iterator find_vertex(
					__in GLuint id
					);

				std::map<GLuint, std::pair<size_t, size_t>> m_buffer_map;

				bool m_initialized;

				static _luna_vertex *m_instance;

				std::map<GLuint, std::pair<size_t, size_t>> m_vertex_map;

		} luna_vertex, *luna_vertex_ptr;
	}
}

#endif // LUNA_VERTEX_H_
