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

#ifndef LUNA_SHADER_TYPE_H_
#define LUNA_SHADER_TYPE_H_

namespace LUNA {

	namespace COMP {

		#define LUNA_SHADER_HEADER "(SHADER)"
		#define LUNA_SHADER_PROGRAM_HEADER "(SHADER_PROGRAM)"

#ifndef NDEBUG
		#define LUNA_SHADER_EXCEPTION_HEADER LUNA_SHADER_HEADER
#else
		#define LUNA_SHADER_EXCEPTION_HEADER EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			LUNA_SHADER_EXCEPTION_ALLOCATED = 0,
			LUNA_SHADER_EXCEPTION_EXTERNAL,
			LUNA_SHADER_EXCEPTION_FILE_NOT_FOUND,
			LUNA_SHADER_EXCEPTION_INITIALIZED,
			LUNA_SHADER_EXCEPTION_NOT_FOUND,
			LUNA_SHADER_EXCEPTION_UNINITIALIZED,
		};

		#define LUNA_SHADER_EXCEPTION_MAX LUNA_SHADER_EXCEPTION_UNINITIALIZED

		static const std::string LUNA_SHADER_EXCEPTION_STR[] = {
			LUNA_SHADER_EXCEPTION_HEADER " Failed to allocate shader component",
			LUNA_SHADER_EXCEPTION_HEADER " External exception",
			LUNA_SHADER_EXCEPTION_HEADER " Files does not exist",
			LUNA_SHADER_EXCEPTION_HEADER " Shader component is initialized",
			LUNA_SHADER_EXCEPTION_HEADER " Shader does not exist",
			LUNA_SHADER_EXCEPTION_HEADER " Shader component is uninitialized",
			};

		#define LUNA_SHADER_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > LUNA_SHADER_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(LUNA_SHADER_EXCEPTION_STR[_TYPE_]))

		#define THROW_LUNA_SHADER_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(LUNA_SHADER_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_LUNA_SHADER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(LUNA_SHADER_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)

		class _luna_shader;
		typedef _luna_shader luna_shader, *luna_shader_ptr;

		class _luna_shader_program;
		typedef _luna_shader_program luna_shader_program, *luna_shader_program_ptr;
	}
}

#endif // LUNA_SHADER_TYPE_H_
