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

#ifndef LUNA_TYPE_H_
#define LUNA_TYPE_H_

namespace LUNA {

	#define LUNA_HEADER "(LUNA)"

#ifndef NDEBUG
	#define LUNA_EXCEPTION_HEADER LUNA_HEADER
#else
	#define LUNA_EXCEPTION_HEADER EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		LUNA_EXCEPTION_ALLOCATED = 0,
		LUNA_EXCEPTION_EXTERNAL,
		LUNA_EXCEPTION_INITIALIZED,
		LUNA_EXCEPTION_INVALID,
		LUNA_EXCEPTION_NOT_FOUND,
		LUNA_EXCEPTION_STARTED,
		LUNA_EXCEPTION_STOPPED,
		LUNA_EXCEPTION_UNINITIALIZED,
	};

	#define LUNA_EXCEPTION_MAX LUNA_EXCEPTION_UNINITIALIZED

	static const std::string LUNA_EXCEPTION_STR[] = {
		LUNA_EXCEPTION_HEADER " Failed to allocate library",
		LUNA_EXCEPTION_HEADER " External exception",
		LUNA_EXCEPTION_HEADER " Library is initialized",
		LUNA_EXCEPTION_HEADER " Invalid callback",
		LUNA_EXCEPTION_HEADER " Event does not exist",
		LUNA_EXCEPTION_HEADER " Library is started",
		LUNA_EXCEPTION_HEADER " Library is stopped",
		LUNA_EXCEPTION_HEADER " Library is uninitialized",
		};

	#define LUNA_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > LUNA_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
		STRING_CHECK(LUNA_EXCEPTION_STR[_TYPE_]))

	#define THROW_LUNA_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(LUNA_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_LUNA_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(LUNA_EXCEPTION_STRING(_EXCEPT_), \
		_FORMAT_, __VA_ARGS__)

	class _luna_config;
	typedef _luna_config luna_config, *luna_config_ptr;

	class _luna;
	typedef _luna luna, *luna_ptr;
}

#endif // LUNA_TYPE_H_
