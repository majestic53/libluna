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

#ifndef LUNA_H_
#define LUNA_H_

//#define NDEBUG

#ifndef LUNA
#define LUNA libluna
#endif // LUNA

#include "luna_define.h"
#include "luna_exception.h"

using namespace LUNA;

#ifndef COMP
#define COMP component
#endif // COMP

#include "luna_display.h"
#include "luna_input.h"

using namespace LUNA::COMP;

namespace LUNA {

	typedef enum {
		LUNA_EVT_SETUP = 0,
		LUNA_EVT_START,
		LUNA_EVT_STOP,
		LUNA_EVT_TEARDOWN,
	} luna_evt_t;

	#define LUNA_EVT_MAX LUNA_EVT_TEARDOWN

	typedef void (*luna_evt_cb)(
		__in void *
		);

	typedef std::map<luna_evt_t, std::pair<luna_evt_cb, void *>> luna_config;

	typedef class _luna {

		public:

			~_luna(void);

			static _luna *acquire(void);

			luna_display_ptr acquire_display(void);

			luna_input_ptr acquire_input(void);

			void initialize(void);

			static bool is_allocated(void);

			bool is_initialized(void);

			bool is_running(void);

			void start(
				__in const luna_config &config,
				__in const luna_display_config &display_config,
				__in const luna_input_config &input_config
				);

			void stop(void);

			std::string to_string(
				__in_opt bool verbose = false
				);

			void uninitialize(void);

			static std::string version(void);

		protected:

			_luna(void);

			_luna(
				__in const _luna &other
				);

			_luna &operator=(
				__in const _luna &other
				);

			static void _delete(void);

			static void external_initialize(void);

			static void external_uninitialize(void);

			void invoke(
				__in luna_evt_t type
				);

			void setup(
				__in const luna_config &config,
				__in const luna_display_config &display_config,
				__in const luna_input_config &input_config
				);

			void teardown(void);

			bool m_initialized;

			static bool m_initialized_external;

			static _luna *m_instance;

			luna_config m_config;

			luna_display_ptr m_instance_display;

			luna_input_ptr m_instance_input;

			bool m_running;

	} luna, *luna_ptr;
}

#endif // LUNA_H_
