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

#ifndef LUNA_DISPLAY_H_
#define LUNA_DISPLAY_H_

namespace LUNA {

	namespace COMP {

		#define DISPLAY_CONFIG_DEF_HEIGHT 480
		#define DISPLAY_CONFIG_DEF_FLAGS SDL_WINDOW_OPENGL
		#define DISPLAY_CONFIG_DEF_TITLE "Untitled Window"
		#define DISPLAY_CONFIG_DEF_X SDL_WINDOWPOS_CENTERED
		#define DISPLAY_CONFIG_DEF_WIDTH 640
		#define DISPLAY_CONFIG_DEF_Y SDL_WINDOWPOS_CENTERED

		typedef class _luna_display_config {

			public:

				_luna_display_config(
					__in_opt const std::string &title = DISPLAY_CONFIG_DEF_TITLE,
					__in_opt size_t width = DISPLAY_CONFIG_DEF_WIDTH,
					__in_opt size_t height = DISPLAY_CONFIG_DEF_HEIGHT,
					__in_opt size_t x = DISPLAY_CONFIG_DEF_X,
					__in_opt size_t y = DISPLAY_CONFIG_DEF_Y,
					__in_opt uint32_t flags = DISPLAY_CONFIG_DEF_FLAGS
					);

				_luna_display_config(
					__in const _luna_display_config &other
					);

				virtual ~_luna_display_config(void);

				_luna_display_config &operator=(
					__in const _luna_display_config &other
					);

				uint32_t &flags(void);

				size_t &height(void);

				std::string &title(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				size_t &x(void);

				size_t &width(void);

				size_t &y(void);

			protected:

				uint32_t m_flags;

				size_t m_height, m_x, m_width, m_y;

				std::string m_title;

		} luna_display_config, *luna_display_config_ptr;

		typedef class _luna_display {

			public:

				~_luna_display(void);

				static _luna_display *acquire(void);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				bool is_running(void);

				void start(
					__in const luna_display_config &config
					);

				void stop(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

			protected:

				_luna_display(void);

				_luna_display(
					__in const _luna_display &other
					);

				_luna_display &operator=(
					__in const _luna_display &other
					);

				static void _delete(void);

				luna_display_config m_config;

				bool m_initialized;

				static _luna_display *m_instance;

				bool m_running;

				SDL_Window *m_window;

		} luna_display, *luna_display_ptr;

	}
}

#endif // LUNA_DISPLAY_H_
