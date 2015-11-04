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

#ifndef LUNA_DISPLAY_TYPE_H_
#define LUNA_DISPLAY_TYPE_H_

namespace LUNA {

	namespace COMP {

		#define LUNA_DISPLAY_HEADER "(DISPLAY)"

#ifndef NDEBUG
		#define LUNA_DISPLAY_EXCEPTION_HEADER LUNA_DISPLAY_HEADER
#else
		#define LUNA_DISPLAY_EXCEPTION_HEADER EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			LUNA_DISPLAY_EXCEPTION_ALLOCATED = 0,
			LUNA_DISPLAY_EXCEPTION_EXTERNAL,
			LUNA_DISPLAY_EXCEPTION_INITIALIZED,
			LUNA_DISPLAY_EXCEPTION_STARTED,
			LUNA_DISPLAY_EXCEPTION_STOPPED,
			LUNA_DISPLAY_EXCEPTION_UNINITIALIZED,
		};

		#define LUNA_DISPLAY_EXCEPTION_MAX LUNA_DISPLAY_EXCEPTION_UNINITIALIZED

		static const std::string LUNA_DISLAY_EXCEPTION_STR[] = {
			LUNA_DISPLAY_EXCEPTION_HEADER " Failed to allocate display component",
			LUNA_DISPLAY_EXCEPTION_HEADER " External exception",
			LUNA_DISPLAY_EXCEPTION_HEADER " Display component is initialized",
			LUNA_DISPLAY_EXCEPTION_HEADER " Display component is started",
			LUNA_DISPLAY_EXCEPTION_HEADER " Display component is stopped",
			LUNA_DISPLAY_EXCEPTION_HEADER " Display component is uninitialized",
			};

		#define LUNA_DISPLAY_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > LUNA_DISPLAY_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(LUNA_DISLAY_EXCEPTION_STR[_TYPE_]))

		#define THROW_LUNA_DISPLAY_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(LUNA_DISPLAY_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_LUNA_DISPLAY_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(LUNA_DISPLAY_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)

		class _luna_display_config;
		typedef _luna_display_config luna_display_config, *luna_display_config_ptr;

		class _luna_display;
		typedef _luna_display luna_display, *luna_display_ptr;
	}
}

#endif // LUNA_DISPLAY_TYPE_H_
