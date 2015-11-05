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

#ifndef LUNA_INPUT_H_
#define LUNA_INPUT_H_

namespace LUNA {

	namespace COMP {

		typedef enum {
			LUNA_INPUT_EVT_NONE = 0,
			LUNA_INPUT_EVT_UNSUPPORTED,
			LUNA_INPUT_EVT_QUIT,
		} luna_input_evt;

		typedef luna_input_evt (*luna_input_cb)(
			__in const SDL_Event &,
			__in void *
			);

		typedef std::map<SDL_EventType, std::pair<luna_input_cb, void *>> luna_input_config;

		typedef class _luna_input {

			public:

				~_luna_input(void);

				static _luna_input *acquire(void);

				void add(
					__in const SDL_EventType &type,
					__in luna_input_cb callback,
					__in_opt void *context = NULL
					);

				void clear(void);

				bool contains(
					__in SDL_EventType type
					);

				luna_input_evt handle(
					__in const SDL_Event &event
					);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				void remove(
					__in const SDL_EventType &type
					);

				void set(
					__in const luna_input_config &config
					);

				size_t size(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

			protected:

				_luna_input(void);

				_luna_input(
					__in const _luna_input &other
					);

				_luna_input &operator=(
					__in const _luna_input &other
					);

				static void _delete(void);

				luna_input_config::iterator find(
					__in const SDL_EventType &type
					);

				luna_input_config m_config;

				bool m_initialized;

				static _luna_input *m_instance;

		} luna_input, *luna_input_ptr;
	}
}

#endif // LUNA_INPUT_H_
