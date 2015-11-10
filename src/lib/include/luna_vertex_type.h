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

#ifndef LUNA_VERTEX_TYPE_H_
#define LUNA_VERTEX_TYPE_H_

namespace LUNA {

	namespace COMP {

		#define LUNA_VERTEX_HEADER "(VERTEX)"

#ifndef NDEBUG
		#define LUNA_VERTEX_EXCEPTION_HEADER LUNA_VERTEX_HEADER
#else
		#define LUNA_VERTEX_EXCEPTION_HEADER EXCEPTION_HEADER
#endif  // NDEBUG

		enum {
			LUNA_VERTEX_EXCEPTION_ALLOCATED = 0,
			LUNA_VERTEX_EXCEPTION_BUFFER_NOT_FOUND,
			LUNA_VERTEX_EXCEPTION_INITIALIZED,
			LUNA_VERTEX_EXCEPTION_UNINITIALIZED,
			LUNA_VERTEX_EXCEPTION_VERTEX_NOT_FOUND
		};

		#define LUNA_VERTEX_EXCEPTION_MAX LUNA_VERTEX_EXCEPTION_VERTEX_NOT_FOUND

		static const std::string LUNA_VERTEX_EXCEPTION_STR[] = {
			LUNA_VERTEX_EXCEPTION_HEADER " Failed to allocate vertex component",
			LUNA_VERTEX_EXCEPTION_HEADER " Buffer does not exist",
			LUNA_VERTEX_EXCEPTION_HEADER " Vertex component is initialized",
			LUNA_VERTEX_EXCEPTION_HEADER " Vertex component is uninitialized",
			LUNA_VERTEX_EXCEPTION_HEADER " Vertex does not exist",
			};

		#define LUNA_VERTEX_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > LUNA_VERTEX_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(LUNA_VERTEX_EXCEPTION_STR[_TYPE_]))

		#define THROW_LUNA_VERTEX_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(LUNA_VERTEX_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_LUNA_VERTEX_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(LUNA_VERTEX_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)

		class _luna_vertex;
		typedef _luna_vertex luna_vertex, *luna_vertex_ptr;
	}
}

#endif // LUNA_VERTEX_TYPE_H_
