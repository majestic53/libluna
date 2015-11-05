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

#ifndef LUNA_INPUT_TYPE_H_
#define LUNA_INPUT_TYPE_H_

namespace LUNA {

	namespace COMP {

		#define LUNA_INPUT_HEADER "(INPUT)"

#ifdef NDEBUG
		#define LUNA_INPUT_EXCEPTION_HEADER LUNA_INPUT_HEADER
#else
		#define LUNA_INPUT_EXCEPTION_HEADER EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			LUNA_INPUT_EXCEPTION_ALLOCATED = 0,
			LUNA_INPUT_EXCEPTION_INITIALIZED,
			LUNA_INPUT_EXCEPTION_INVALID,
			LUNA_INPUT_EXCEPTION_NOT_FOUND,
			LUNA_INPUT_EXCEPTION_UNINITIALIZED,
		};

		#define LUNA_INPUT_EXCEPTION_MAX LUNA_INPUT_EXCEPTION_UNINITIALIZED

		static const std::string LUNA_INPUT_EXCEPTION_STR[] = {
			LUNA_INPUT_EXCEPTION_HEADER " Failed to allocate input component",
			LUNA_INPUT_EXCEPTION_HEADER " Input component is initialized",
			LUNA_INPUT_EXCEPTION_HEADER " Invalid input callback",
			LUNA_INPUT_EXCEPTION_HEADER " Input does not exist",
			LUNA_INPUT_EXCEPTION_HEADER " Input component is uninitialized",
			};

		#define LUNA_INPUT_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > LUNA_INPUT_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(LUNA_INPUT_EXCEPTION_STR[_TYPE_]))

		#define THROW_LUNA_INPUT_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(LUNA_INPUT_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_LUNA_INPUT_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(LUNA_INPUT_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)

		class _luna_input;
		typedef _luna_input luna_input, *luna_input_ptr;
	}
}

#endif // LUNA_INPUT_TYPE_H_
