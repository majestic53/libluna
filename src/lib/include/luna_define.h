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

#ifndef LUNA_DEFINE_H_
#define LUNA_DEFINE_H_

#include <SDL2/SDL.h>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace LUNA {

	#ifndef __in
	#define __in
	#endif // __in
	#ifndef __in_opt
	#define __in_opt
	#endif // __in_opt
	#ifndef __inout
	#define __inout
	#endif // __inout
	#ifndef __inout_opt
	#define __inout_opt
	#endif // __inout_opt
	#ifndef __out
	#define __out
	#endif // __out
	#ifndef __out_opt
	#define __out_opt
	#endif // __out_opt

	#define MS_PER_SEC 1000

	#define EMPTY "EMPTY"
	#define INVALID "INVALID"
	#define UNKNOWN "UNKNOWN"

	#define SCALAR_AS_HEX(_TYPE_, _VAL_) \
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex \
		<< (uintmax_t) ((_TYPE_) (_VAL_)) << std::dec

	#define SCALAR_INVALID(_TYPE_) ((_TYPE_) -1)

	#define _STRING_CAT(_STR_) # _STR_
	#define STRING_CAT(_STR_) _STRING_CAT(_STR_)

	#define STRING_CHECK(_STR_) (_STR_.empty() ? EMPTY : _STR_.c_str())

	#define UNREFERENCE_PARAM(_PARAM_) (void) _PARAM_

	#define VERSION_MAJ 0
	#define VERSION_MIN 1
	#define VERSION_REV 3
	#define VERSION_TICK 1545
}

#endif // LUNA_DEFINE_H_
